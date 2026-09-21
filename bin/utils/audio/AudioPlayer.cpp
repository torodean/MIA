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

// The associated header file.
#include "AudioPlayer.hpp"
// Used for audio file type checks.
#include "SoundsFromFile.hpp"
// Used for file and metadata handling.
#include "FileUtils.hpp"
// Used for preprocessor macros.
#include "Constants.hpp"
// Used for error returns.
#include "MIAException.hpp"
#include "Error.hpp"
// Used for thread sleeps.
#include "Timing.hpp"

#if defined(IS_WINDOWS)
    #include <Windows.h>
    #include <mmsystem.h>
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
#endif


    AudioPlayer::AudioPlayer()
    {
        setTaskName("AudioPlayer");

    #if defined(IS_LINUX)
        std::lock_guard<std::mutex> lock(vlcPlayerMutex);
        
        // Create an instance of vlc to use.
        vlcInstance = libvlc_new(0, nullptr);
        if (!vlcInstance)
        { // Check for errors.
            MIA_THROW(VLC_Failed_To_Create_Instance, "In AudioPlayer Construction.");
        }
    #endif
    }


    AudioPlayer::AudioPlayer(const std::string& fileName)
    {
        setTaskName("AudioPlayer");
        setAudioFile(fileName);

    #if defined(IS_LINUX)
        std::lock_guard<std::mutex> lock(vlcPlayerMutex);
        
        // Create an instance of vlc to use.
        vlcInstance = libvlc_new(0, nullptr);
        if (!vlcInstance)
        { // Check for errors.
            MIA_THROW(VLC_Failed_To_Create_Instance, "In AudioPlayer Construction.");
        }
        
        // Create a vlc media object with the sound file loaded.
        libvlc_media_t* media = libvlc_media_new_path(vlcInstance, fileName.c_str());
        if (!media)
        { // Check for errors.
            libvlc_release(vlcInstance);
            MIA_THROW(VLC_Failed_To_Create_Media, "In AudioPlayer Construction.");
        }
        
        // Create the player to actually play the media.
        vlcPlayer = libvlc_media_player_new_from_media(media);
        libvlc_media_release(media);
        if (!vlcPlayer)
        { // Check for errors.
            libvlc_release(vlcInstance);
            MIA_THROW(VLC_Failed_To_Create_Media, "In AudioPlayer Construction.");
        }
    #endif
    }


    bool AudioPlayer::setAudioFile(const std::string& fileName)
    {
        // Check if the file is a supported type before storing it.
        files::FileMetaData fileMetaData = files::getFileMetaData(fileName);
        if (!isASupportedType(fileMetaData))
        {
            std::cerr << "Unsupported file type specified!" << std::endl;
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
                std::cerr << "Skipping unsupported file type: " << fileName << std::endl;
                continue;
            }
            newPlaylist.push_back(fileMetaData);
        }

        if (newPlaylist.empty())
        {
            std::cerr << "No supported files were specified for the playlist!" << std::endl;
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
            std::cerr << "Unsupported file type specified!" << std::endl;
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
        {
            std::lock_guard<std::mutex> lock(playlistMutex);
            if (!playlist.advance(playlistShuffle, playlistLoop))
            {
                std::cerr << "The playlist is empty or finished!" << std::endl;
                return false;
            }
        }

        // Restart playback on the newly selected track.
        return restartAudio();
        return false;
    }


    bool AudioPlayer::playPreviousTrack()
    {
        {
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
        return false;
    }


    bool AudioPlayer::isAudioPlaying() const
    {
    #if defined(IS_WINDOWS)
        return audioPlaying.load();
    #elif defined(IS_LINUX)
        return vlcPlayer;
    #endif
    }


    bool AudioPlayer::startAudio()
    {
    #if defined(IS_WINDOWS)

        // If the audio is already playing, do nothing.
        if (audioPlaying)
            return true;

        // Join the thread from a previous playback which finished on its own.
        stop();

        // Check if a valid audio track was set before starting the thread.
        {
            std::lock_guard<std::mutex> lock(playlistMutex);
            if (playlist.current().type == files::FileType::Unknown)
            {
                std::cerr << "No audio file was specified to play!" << std::endl;
                return false;
            }
        }

        // Reset the flags so the background task runs the new playback.
        stopRequested = false;
        audioPlaying = true;
        start();
    
    #elif defined(IS_LINUX)
    
        std::lock_guard<std::mutex> lock(vlcPlayerMutex);
    
        if (vlcPlayer)
            return true;   // Already playing means automatic success.
            
        // Start playing the media.
        if (libvlc_media_player_play(vlcPlayer) == -1)
        { // Check for errors, then cleanup accordingly.
            std::cerr << "ERROR: Failed to start playback!" << std::endl;

            libvlc_media_player_release(vlcPlayer);
            libvlc_release(vlcInstance);
            return false;
        }
    
    #endif
    
        return true;
    }


    bool AudioPlayer::restartAudio()
    {        
        stopAudio();
        return startAudio();
    }


    bool AudioPlayer::stopAudio(uint32_t fadeOutMS)
    {
    #if defined(IS_WINDOWS)

        // Nothing to stop when no audio is playing.
        if (!audioPlaying)
            return true;
        
        /*
         * For a real fade, set the fade-specific flags and let the audio thread perform
         * the volume ramp + stop. For the zero-fade case just signal the worker.
         */
        if (fadeOutMS == 0)
        {
            audioPlaying = false;
        }
        else
        {
            std::lock_guard<std::mutex> lock(playlistMutex);

            // The 'setaudio .. volume to ..' command is not supported for non-mp3 types.
            if (playlist.current().type != files::FileType::Mp3)
            {
                audioPlaying = false;
                std::cerr << "WARNING: The fade out sound option is unsupported for"
                          << " non-mp3 audio files!" << std::endl;
            }
            else
            {
                fadeRequested = true;
                fadeTimeMS = fadeOutMS;
            }
            fadeRequested = true;
            fadeTimeMS = fadeOutMS;
        }

        // Stop and join the audio thread for a clean device close.
        stop();
        
    #elif defined(IS_LINUX)
    
        std::lock_guard<std::mutex> lock(vlcPlayerMutex);
        
        if (!vlcPlayer)
            return true;   // Nothing playing means automatic success.
            
        if (fadeOutMs > 0)
        {
            // Simple linear fade-out (volume goes from 100 → 0)
            const uint32_t steps = std::max(1u, fadeOutMs / 25); // 25ms steps.
            const uint32_t stepTime = fadeOutMs / steps;

            for (int i = steps; i >= 0; --i)
            {
                int volume = (i * 100) / steps;
                libvlc_audio_set_volume(vlcPlayer, volume);
                timing::sleepMilliseconds(stepTime);
            }
        }

        // Stop playback and release resources
        libvlc_media_player_stop(vlcPlayer);
        libvlc_media_player_release(vlcPlayer);
        libvlc_release(vlcInstance);

        vlcPlayer = nullptr;
        vlcInstance = nullptr;

        return true;
        
    #endif
    
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
    #if defined(IS_LINUX)
        // Set the playlistloop
        if (loop)
        {
            if (libvlc_media_list_player_set_playback_mode(vlcPlayer, libvlc_playback_mode_loop) == -1)
            { // Check for errors
                std::cerr << "ERROR: Failed to enable playlist looping!" << std::endl;
                return false; // Return and don't update playlistLoop.
            }
        }
    #endif
        playlistLoop = loop;
        return true;
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
        }

        if (fileMetaData.type == files::FileType::Unknown)
        {
            std::cerr << "No audio file was specified to play!" << std::endl;
        #if defined(IS_WINDOWS)
            audioPlaying = false;
        #endif
            stopRequested = true;
            return;
        }

    /*
     * Windows uses the MCI APIC to play the audio.
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

        // Continue while the audio is playing until a stop condition is met.
        while (audioPlaying.load())
        {
            // Check the status of the audio being played.
            char status[32] = {};
            mciSendString(("status " + soundAlias + " mode").c_str(),
                          status, sizeof(status), NULL);
            if (std::string(status) == "stopped")
                break; // The audio finished playing on its own.

            // Handle the fade out by continually reducing the volume.
            if (fadeRequested)
            {
                const uint32_t steps = std::max(1u, fadeTimeMS.load() / 25); // 25ms steps.
                const uint32_t stepTime = fadeTimeMS / steps;

                for (uint32_t i = steps; i > 0; --i)
                {
                    uint32_t volume = (1000 * (i - 1)) / steps;

                    std::string fadeCommand = "setaudio " + soundAlias +
                                              " volume to " + std::to_string(volume);
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
        
    #elif defined(IS_LINUX)
    
        MIA_THROW(error::ErrorCode::Windows_Only_Feature, "AudioPlayer run() method.");
        
    #endif

        // The playback is done, so end the task unless there is more to play.
        if (stopRequested)
        {
        #if defined(IS_WINDOWS)
            audioPlaying = false;
        #endif
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
    #if defined(IS_WINDOWS)
        audioPlaying = false;
    #endif
        stopRequested = true;
    }
} // namespace audio
