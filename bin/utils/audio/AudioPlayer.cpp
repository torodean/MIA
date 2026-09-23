/**
 * @file AudioPlayer.cpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief Implements the methods from the associated header file.
 */

#include <iostream>
#include <algorithm>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <cstdarg>
#include <cstdio>

// The associated header file.
#include "AudioPlayer.hpp"
// Used for audio file type checks.
#include "SoundsFromFile.hpp"
// Used for file and metadata handling.
#include "FileUtils.hpp"
// Used for error returns.
#include "MIAException.hpp"
#include "Error.hpp"
// Used for thread sleeps.
#include "Timing.hpp"

#if defined(IS_WINDOWS)
    #include <Windows.h>
    #include <mmsystem.h>
#elif defined(IS_LINUX)
    #include <vlc/vlc.h>
#endif

namespace audio
{
#if defined(IS_WINDOWS)

    /**
     * @brief Defines values used to generate unique MCI aliases for each playback.
     *
     * Every playback opens its own MCI device with a unique alias. This keeps multiple
     * AudioPlayer instances (and repeated plays of the same instance) from colliding on
     * an already-open device.
     */
    namespace audio_player
    {
        std::atomic<uint64_t> nextAliasId = 0;
    } // namespace audio_player


    /**
     * @brief Stops and closes the MCI device tied to the given alias.
     *
     * @param soundAlias The alias of the device to stop and close.
     */
    static void closeAudioDevice(const std::string& soundAlias)
    {
        MCIERROR error = mciSendString(("stop " + soundAlias).c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char errorText[256];
            mciGetErrorString(error, errorText, sizeof(errorText));
            std::cerr << "Failed to stop sound file: " << errorText << std::endl;
        }
        error = mciSendString(("close " + soundAlias).c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char errorText[256];
            mciGetErrorString(error, errorText, sizeof(errorText));
            std::cerr << "Failed to close sound file: " << errorText << std::endl;
        }
    }

#elif defined(IS_LINUX)

    /**
     * @brief A no-op libVLC logging callback.
     *
     * libVLC writes its internal diagnostics (demux probing errors, etc.)
     * directly to stderr through its own logging. This callback silences that
     * output so the audio player does not pollute application output with
     * messages which do not affect playback.
     *
     * @param level The libVLC log level of this message (unused).
     * @param fmt The printf-style format string (unused).
     */
    static void vlcLogCallback(void*, int, const libvlc_log_t*, const char*, va_list)
    {
        // Intentionally silent.
    }

#endif


    AudioPlayer::AudioPlayer()
    {
        setTaskName("AudioPlayer");

    #if defined(IS_LINUX)
        // Create an instance of vlc to use.
        vlcInstance = libvlc_new(0, nullptr);
        if (!vlcInstance)
        { // Check for errors.
            MIA_THROW(error::VLC_Failed_To_Create_Instance, "In AudioPlayer Construction.");
        }

        // Silence the libVLC internal logging (e.g. demux probing noise).
        libvlc_log_set(vlcInstance, vlcLogCallback, nullptr);
    #endif
    }


    AudioPlayer::AudioPlayer(const std::string& fileName)
    {
        setTaskName("AudioPlayer");

    #if defined(IS_LINUX)
        // Create an instance of vlc to use.
        vlcInstance = libvlc_new(0, nullptr);
        if (!vlcInstance)
        { // Check for errors.
            MIA_THROW(error::VLC_Failed_To_Create_Instance, "In AudioPlayer Construction.");
        }

        // Silence the libVLC internal logging (e.g. demux probing noise).
        libvlc_log_set(vlcInstance, vlcLogCallback, nullptr);
    #endif

        setAudioFile(fileName);
    }


    AudioPlayer::~AudioPlayer()
    {
    #if defined(IS_LINUX)
        // Join the worker thread (which stops the playback) before releasing VLC.
        stop();

        if (vlcInstance)
            libvlc_release(vlcInstance);

        vlcInstance = nullptr;
    #endif
    }


    bool AudioPlayer::setAudioFile(const std::string& fileName)
    {
        // Check if the file is a supported type before storing it.
        files::FileMetaData fileMetaData = files::getFileMetaData(fileName);
        if (!isASupportedType(fileMetaData))
        {
            std::cerr << "ERROR: Unsupported file type specified!" << std::endl;
            return false;
        }

        std::lock_guard<std::mutex> lock(playlistMutex);
        playlist.setList({fileMetaData});

        return true;
    }


    bool AudioPlayer::setPlaylist(const std::vector<std::string>& fileNames)
    {
        // Parse all of the entries first so an unsupported file doesn't clear the playlist.
        std::vector<files::FileMetaData> newPlaylist;
        for (const std::string& fileName : fileNames)
        {
            files::FileMetaData fileMetaData = files::getFileMetaData(fileName);
            if (!isASupportedType(fileMetaData))
            {
                std::cerr << "WARNING: Skipping unsupported file type: " << fileName << std::endl;
                continue;
            }
            newPlaylist.push_back(fileMetaData);
        }

        if (newPlaylist.empty())
        {
            std::cerr << "ERROR: No supported files were specified for the playlist!" << std::endl;
            return false;
        }

        std::lock_guard<std::mutex> lock(playlistMutex);
        playlist.setList(newPlaylist);

        return true;
    }


    bool AudioPlayer::addToPlaylist(const std::string& fileName)
    {
        // Check if the file is a supported type before storing it.
        files::FileMetaData fileMetaData = files::getFileMetaData(fileName);
        if (!isASupportedType(fileMetaData))
        {
            std::cerr << "ERROR: Unsupported file type specified!" << std::endl;
            return false;
        }

        std::lock_guard<std::mutex> lock(playlistMutex);
        playlist.add(fileMetaData);

        return true;
    }


    void AudioPlayer::clearPlaylist()
    {
        std::lock_guard<std::mutex> lock(playlistMutex);
        playlist.clear();
    }


    bool AudioPlayer::nextAudio()
    {
        return playNextTrack();
    }


    bool AudioPlayer::previousAudio()
    {
        return playPreviousTrack();
    }


    bool AudioPlayer::playNextTrack()
    {
        { // Advance the playlist.
            std::lock_guard<std::mutex> lock(playlistMutex);

            /*
             * When the audio file was changed mid-play, the track which is
             * playing is no longer in the playlist, so advancing it would
             * move from the (already-current) new cursor. Detect this case
             * and play the track at the cursor instead of advancing it.
             */
            if (playlist.current().fullFilePath != playingFilePath)
            {
                if (playlist.current().type == files::FileType::Unknown)
                {
                    std::cerr << "WARNING: The playlist is empty or finished!" << std::endl;
                    return false;
                }
            }
            else if (!playlist.advance(playlistShuffle, playlistLoop))
            {
                std::cerr << "WARNING: The playlist is empty or finished!" << std::endl;
                return false;
            }
        }

        // Restart playback on the newly selected track.
        return restartAudio();
    }


    bool AudioPlayer::playPreviousTrack()
    {
        { // Use the playlist memory to go back to the previous track.
            std::lock_guard<std::mutex> lock(playlistMutex);
            if (!playlist.back())
            {
                std::cerr << "WARNING: No previously played track to go back to!"
                          << std::endl;
                return false;
            }
        }

        // Restart playback on the newly selected track.
        return restartAudio();
    }


    bool AudioPlayer::isAudioPlaying() const
    {
        return audioPlaying.load();
    }


    bool AudioPlayer::startAudio()
    {
        // If the audio is already playing, do nothing.
        if (isAudioPlaying())
            return true;

        // Join the thread from a previous playback which finished on its own.
        stop();

        // Check if a valid audio track was set before starting the thread.
        {
            std::lock_guard<std::mutex> lock(playlistMutex);
            if (playlist.current().type == files::FileType::Unknown)
            {
                std::cerr << "ERROR: No audio file was specified to play!" << std::endl;
                return false;
            }
        }

        // Reset the flags so the background task runs the new playback.
        stopRequested = false;
        audioPlaying = true;
        start();

        return true;
    }


    bool AudioPlayer::restartAudio()
    {
        stopAudio();
        return startAudio();
    }


    bool AudioPlayer::stopAudio(uint32_t fadeOutMs)
    {
        // Nothing to stop when no audio is playing.
        if (!audioPlaying)
            return true;

        /*
         * For the zero-fade case, stop and join the worker thread right away
         * for a clean shutdown.
         */
        if (fadeOutMs == 0)
        {
            audioPlaying = false;
            stopRequested = true;
            stop();
            return true;
        }

    #if defined(IS_WINDOWS)
        { // The 'setaudio .. volume to ..' command is not supported for non-mp3 types.
            std::lock_guard<std::mutex> lock(playlistMutex);
            if (playlist.current().type != files::FileType::Mp3)
            {
                audioPlaying = false;
                stopRequested = true;
                stop();
                std::cerr << "WARNING: The fade out sound option is unsupported for"
                          << " non-mp3 audio files!" << std::endl;
                return true;
            }
        }
    #endif

        /*
         * For a real fade, set the fade-specific flags and let the worker thread
         * perform the volume ramp + stop. The fade runs asynchronously in the
         * worker thread, so this returns before the fade completes. The worker
         * thread is joined by a later stop()/startAudio() call or destruction.
         */
        fadeRequested = true;
        fadeTimeMS = fadeOutMs;
        stopRequested = true;

        return true;
    }


    void AudioPlayer::setRepeatAudioTrack(bool repeat)
    {
        repeatAudioTrack = repeat;
    }


    bool AudioPlayer::getRepeatAudioTrack() const
    {
        return repeatAudioTrack.load();
    }


    void AudioPlayer::setPlaylistLoop(bool loop)
    {
        playlistLoop = loop;
    }


    bool AudioPlayer::getPlaylistLoop() const
    {
        return playlistLoop.load();
    }


    void AudioPlayer::setPlaylistShuffle(bool shuffle)
    {
        playlistShuffle = shuffle;
    }


    bool AudioPlayer::getPlaylistShuffle() const
    {
        return playlistShuffle.load();
    }


    void AudioPlayer::setVolume(uint32_t volume)
    {
        // Clamp the volume to the supported 0 - 100 range.
        this->volume = std::min(volume, 100u);

        /*
         * A live playback picks the new volume up on the next pass of its poll
         * loop (the fade and volume tracking is done in the run() worker).
         */
    }


    uint32_t AudioPlayer::getVolume() const
    {
        return volume.load();
    }


    void AudioPlayer::start()
    {
        threading::BackgroundTask::start();
    }


    void AudioPlayer::stop()
    {
        threading::BackgroundTask::stop();
    }


    void AudioPlayer::run()
    {
        // Don't start a new playback if a stop was already requested.
        if (stopRequested)
            return;

        // Grab the track metadata while holding the mutex so it can safely change during play.
        files::FileMetaData fileMetaData;
        {
            std::lock_guard<std::mutex> lock(playlistMutex);
            fileMetaData = playlist.current();
            playingFilePath = fileMetaData.fullFilePath;
        }

        if (fileMetaData.type == files::FileType::Unknown)
        {
            std::cerr << "No audio file was specified to play!" << std::endl;
            audioPlaying = false;
            stopRequested = true;
            return;
        }

    /*
     * Windows uses the MCI API to play the audio.
     */
    #if defined(IS_WINDOWS)

        // Mark this playback as active.
        audioPlaying = true;
        // Each playback gets its own MCI device alias to avoid collisions.
        const std::string soundAlias = "MIA_AUDIO_" + std::to_string(audio_player::nextAliasId++);

        // Setup the correct command to run based on the file type.
        std::string openCommand;
        if (fileMetaData.type == files::FileType::Mp3)
            openCommand = "open \"" + fileMetaData.fullFilePath + "\" type mpegvideo alias " + soundAlias;
        else if (fileMetaData.type == files::FileType::Wav)
            openCommand = "open \"" + fileMetaData.fullFilePath + "\" type waveaudio alias " + soundAlias;
        else
        {
            /*
             * The playlist setters only store supported types, so this should never
             * reach this point. I am including it here as a quick-check for if a dev
             * forgets to update isASupportedType() when adding more file support.
             */
            audioPlaying = false;
            stopRequested = true;
            return;
        }

        // Run the command.
        MCIERROR error = mciSendString(openCommand.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char errorText[256];
            mciGetErrorString(error, errorText, sizeof(errorText));
            std::cerr << "Failed to open sound file " << fileMetaData.fullFilePath
                      << ": " << errorText << std::endl;
            audioPlaying = false;
            stopRequested = true;
            return;
        }

        // Play the audio.
        error = mciSendString(("play " + soundAlias).c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char errorText[256];
            mciGetErrorString(error, errorText, sizeof(errorText));
            std::cerr << "Failed to play sound file: " << errorText << std::endl;
            closeAudioDevice(soundAlias);
            audioPlaying = false;
            stopRequested = true;
            return;
        }

        /*
         * The MCI volume range is 0 - 1000, so scale the 0 - 100 setting for
         * the playback.
         */
        const uint32_t mciVolume = volume.load() * 10;
        std::string volumeCommand = "setaudio " + soundAlias +
                                    " volume to " + std::to_string(mciVolume);
        error = mciSendString(volumeCommand.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char errorText[256];
            mciGetErrorString(error, errorText, sizeof(errorText));
            std::cerr << "Failed to set sound volume: " << errorText << std::endl;
        }

        // Track the volume so live changes can be detected in the loop below.
        uint32_t currentVolume = volume.load();

        // Continue while the audio is playing until a stop condition is met.
        while (audioPlaying.load())
        {
            // Check the status of the audio being played.
            char status[32] = {};
            mciSendString(("status " + soundAlias + " mode").c_str(),
                          status, sizeof(status), NULL);
            if (std::string(status) == "stopped")
                break; // The audio finished playing on its own.

            // Apply a live volume change to the current playback.
            if (volume.load() != currentVolume && !fadeRequested)
            {
                currentVolume = volume.load();
                std::string volumeCommand = "setaudio " + soundAlias +
                                            " volume to " + std::to_string(currentVolume * 10);
                MCIERROR volumeError = mciSendString(volumeCommand.c_str(), NULL, 0, NULL);
                if (volumeError != 0)
                {
                    char errorText[256];
                    mciGetErrorString(volumeError, errorText, sizeof(errorText));
                    std::cerr << "Failed to set sound volume: " << errorText << std::endl;
                }
            }

            // Handle the fade out by continually reducing the volume.
            if (fadeRequested)
            {
                const uint32_t steps = std::max(1u, fadeTimeMS.load() / 25); // 25ms steps.
                const uint32_t stepTime = fadeTimeMS / steps;
                // The MCI volume range is 0 - 1000, so scale the 0 - 100 setting.
                const uint32_t mciVolume = volume.load() * 10;

                for (uint32_t i = steps; i > 0; --i)
                {
                    uint32_t fadeVolume = (mciVolume * (i - 1)) / steps;

                    std::string fadeCommand = "setaudio " + soundAlias +
                                              " volume to " + std::to_string(fadeVolume);
                    MCIERROR fadeError = mciSendString(fadeCommand.c_str(), NULL, 0, NULL);
                    if (fadeError != 0)
                    {
                        char errorText[256];
                        mciGetErrorString(fadeError, errorText, sizeof(errorText));
                        std::cerr << "Failed to set sound volume: " << errorText << std::endl;
                        break;
                    }

                    timing::sleepMilliseconds(stepTime);
                }

                // The fade is done, so trigger the stop.
                fadeRequested = false;
                fadeTimeMS = 0;
                audioPlaying = false;
                break;
            }

            // A stop was requested without a fade (e.g. through destruction), so exit.
            if (stopRequested)
            {
                audioPlaying = false;
                break;
            }

            // Sleep between polls.
            timing::sleepMilliseconds(5);
        }

        // Stop and close the audio device for a clean exit.
        closeAudioDevice(soundAlias);

    /*
     * Linux uses the vlc API to play the audio. Each playback gets its own
     * media player (like the MCI alias on Windows), since a media player
     * which was stopped cannot reliably play a second track. The Playlist
     * class tracks everything else.
     */
    #elif defined(IS_LINUX)

        // Load the current track into a new media player.
        libvlc_media_t* media = libvlc_media_new_path(vlcInstance,
                                                      fileMetaData.fullFilePath.c_str());
        if (!media)
        {
            std::cerr << "Failed to create VLC media for " << fileMetaData.fullFilePath
                      << std::endl;
            audioPlaying = false;
            stopRequested = true;
            return;
        }

        libvlc_media_player_t* vlcPlayer = libvlc_media_player_new_from_media(media);
        libvlc_media_release(media);
        if (!vlcPlayer)
        {
            std::cerr << "Failed to create VLC media player for "
                      << fileMetaData.fullFilePath << std::endl;
            audioPlaying = false;
            stopRequested = true;
            return;
        }

        // libVLC starts a media player at volume 0, so set the playback volume.
        libvlc_audio_set_volume(vlcPlayer, static_cast<int>(volume.load()));

        // Start playing the media.
        if (libvlc_media_player_play(vlcPlayer) == -1)
        { // Check for errors, then end the task.
            std::cerr << "Failed to play sound file " << fileMetaData.fullFilePath
                      << std::endl;
            libvlc_media_player_release(vlcPlayer);
            audioPlaying = false;
            stopRequested = true;
            return;
        }

        // Track the volume so live changes can be detected in the loop below.
        uint32_t currentVolume = volume.load();

        // Continue while the audio is playing until a stop condition is met.
        while (true)
        {
            // Check the state of the audio being played.
            const libvlc_state_t state = libvlc_media_player_get_state(vlcPlayer);
            if (state == libvlc_Ended || state == libvlc_Error)
                break; // The audio finished playing on its own or failed.

            // Apply a live volume change to the current playback.
            if (volume.load() != currentVolume && !fadeRequested)
            {
                currentVolume = volume.load();
                libvlc_audio_set_volume(vlcPlayer, static_cast<int>(currentVolume));
            }

            // Handle the fade out by continually reducing the volume.
            if (fadeRequested)
            {
                const uint32_t steps = std::max(1u, fadeTimeMS.load() / 25); // 25ms steps.
                const uint32_t stepTime = fadeTimeMS / steps;
                const uint32_t startVolume = volume.load();

                for (uint32_t i = steps; i > 0; --i)
                {
                    const int fadeVolume = static_cast<int>((startVolume * (i - 1)) / steps);
                    libvlc_audio_set_volume(vlcPlayer, fadeVolume);
                    timing::sleepMilliseconds(stepTime);
                }

                // The fade is done, so stop and release this playback.
                fadeRequested = false;
                fadeTimeMS = 0;
                audioPlaying = false;
                libvlc_media_player_stop(vlcPlayer);
                libvlc_media_player_release(vlcPlayer);
                return;
            }

            // A stop was requested (e.g. through stopAudio() or destruction), so exit.
            if (stopRequested)
            {
                audioPlaying = false;
                libvlc_media_player_stop(vlcPlayer);
                libvlc_media_player_release(vlcPlayer);
                return;
            }

            // Sleep between polls.
            timing::sleepMilliseconds(5);
        }

        // This playback is done, so release its media player.
        libvlc_media_player_release(vlcPlayer);

    #endif

        // The playback is done, so end the task unless there is more to play.
        if (stopRequested)
        {
            audioPlaying = false;
            return;
        }

        // Repeat the same track when enabled, otherwise advance to the next track.
        if (repeatAudioTrack)
            return;

        {
            std::lock_guard<std::mutex> lock(playlistMutex);
            if (playlist.advance(playlistShuffle, playlistLoop))
                return; // There is another track to play.
        }

        // The playlist is finished, so end the task.
        audioPlaying = false;
        stopRequested = true;
    }
} // namespace audio
