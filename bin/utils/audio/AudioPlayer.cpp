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
    
#elif defined(IS_LINUX)

    void AudioPlayer::rebuildVlcMediaList()
    {
        libvlc_media_list_lock(vlcPlaylist);

        // Easiest reliable clear: release + recreate.
        libvlc_media_list_release(vlcPlaylist);
        vlcPlaylist = libvlc_media_list_new(vlcInstance);
        libvlc_media_list_player_set_media_list(vlcPlayer, vlcPlaylist);

        for (const auto& meta : playlist.getTracks())
        {
            libvlc_media_t* media = libvlc_media_new_path(vlcInstance, meta.fullFilePath.c_str());
            if (media)
            {
                libvlc_media_list_add_media(vlcPlaylist, media);
                libvlc_media_release(media);
            }
        }

        libvlc_media_list_unlock(vlcPlaylist);
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
        
        vlcPlaylist  = libvlc_media_list_new(vlcInstance);

        if (!vlcPlaylist)
        {
            // clean up whatever succeeded.
            libvlc_release(vlcInstance);
            MIA_THROW(error::VLC_Failed_To_Create_Media, "In AudioPlayer Construction.");
        }
        
        vlcPlayer = libvlc_media_list_player_new(vlcInstance);

        if (!vlcPlayer)
        {
            // clean up whatever succeeded
            if (vlcPlaylist)
                libvlc_media_list_release(vlcPlaylist);
            libvlc_release(vlcInstance);
            MIA_THROW(error::VLC_Failed_To_Create_Player, "In AudioPlayer Construction.");
        }

        libvlc_media_list_player_set_media_list(vlcPlayer, vlcPlaylist);
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
        
        vlcPlaylist  = libvlc_media_list_new(vlcInstance);

        if (!vlcPlaylist)
        {
            // clean up whatever succeeded.
            libvlc_release(vlcInstance);
            MIA_THROW(error::VLC_Failed_To_Create_Media, "In AudioPlayer Construction.");
        }
        
        vlcPlayer = libvlc_media_list_player_new(vlcInstance);

        if (!vlcPlayer)
        {
            // clean up whatever succeeded
            if (vlcPlaylist)
                libvlc_media_list_release(vlcPlaylist);
            libvlc_release(vlcInstance);
            MIA_THROW(error::VLC_Failed_To_Create_Player, "In AudioPlayer Construction.");
        }

        libvlc_media_list_player_set_media_list(vlcPlayer, vlcPlaylist);
        
    #endif
    
        setAudioFile(fileName);
    }


    AudioPlayer::~AudioPlayer()
    {
    #if defined(IS_LINUX)
        std::lock_guard<std::mutex> lock(vlcMutex);
        
        // Stop and release the resources.
        if (vlcPlayer)
        { 
            libvlc_media_list_player_stop(vlcPlayer); 
            libvlc_media_list_player_release(vlcPlayer); 
        }
        if (vlcPlaylist)
            libvlc_media_list_release(vlcPlaylist);
        if (vlcInstance)
            libvlc_release(vlcInstance);
            
        vlcPlayer = nullptr;
        vlcPlaylist  = nullptr;
        vlcInstance   = nullptr;
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

    #if defined(IS_LINUX)
        rebuildVlcMediaList();
    #endif

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

    #if defined(IS_LINUX)
        rebuildVlcMediaList();
    #endif

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

    #if defined(IS_LINUX)
        rebuildVlcMediaList();
    #endif

        return true;
    }


    void AudioPlayer::clearPlaylist()
    {
        std::lock_guard<std::mutex> lock(playlistMutex);
        playlist.clear();

    #if defined(IS_LINUX)
        rebuildVlcMediaList();
    #endif
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
        size_t newIndex = 0;
        { // Advance the playlist.
            std::lock_guard<std::mutex> lock(playlistMutex);
            if (!playlist.advance(playlistShuffle, playlistLoop))
            {
                std::cerr << "The playlist is empty or finished!" << std::endl;
                return false;
            }
            // Needed for linux implementation below.
            newIndex = playlist.getCurrentIndex();
        }

    #if defined(IS_WINDOWS)
    
        // Restart playback on the newly selected track.
        return restartAudio();
        
    #elif defined(IS_LINUX)
    
        std::lock_guard<std::mutex> lock(vlcMutex);
        
        if (libvlc_media_list_player_play_item_at_index(vlcPlayer, static_cast<int>(newIndex)) == -1)
            return false;
            
        // Stop current playback cleanly.
        libvlc_media_list_player_stop(vlcPlayer);
        timing::sleepMilliseconds(25); // Give VLC time to release the old media.
    
        // Play the index our Playlist selected.
        if (libvlc_media_list_player_play_item_at_index(vlcPlayer, static_cast<int>(newIndex)) == -1)
            return false;

        // Restore volume
        libvlc_media_player_t* mediaPlayer = libvlc_media_list_player_get_media_player(vlcPlayer);
        if (mediaPlayer)
            libvlc_audio_set_volume(mediaPlayer, 100);
        return true;
        
    #endif
        return false;
    }


    bool AudioPlayer::playPreviousTrack()
    {
        size_t newIndex = 0;
        { // Use the playlist memory to go back to the previous track.
            std::lock_guard<std::mutex> lock(playlistMutex);
            if (!playlist.back())
            {
                std::cerr << "WARNING: No previously played track to go back to!"
                          << std::endl;
                return false;
            }
            newIndex = playlist.getCurrentIndex();
        }

    #if defined (IS_WINDOWS)

        // Restart playback on the newly selected track.
        return restartAudio();
        
    #elif defined(IS_LINUX)
    
        std::lock_guard<std::mutex> lock(vlcMutex);
        
        if (libvlc_media_list_player_play_item_at_index(vlcPlayer, static_cast<int>(newIndex)) == -1)
            return false;
            
        // Stop current playback cleanly.
        libvlc_media_list_player_stop(vlcPlayer);
        timing::sleepMilliseconds(25); // Give VLC time to release the old media.
    
        // Play the index our Playlist selected.
        if (libvlc_media_list_player_play_item_at_index(vlcPlayer, static_cast<int>(newIndex)) == -1)
            return false;

        // Restore volume
        libvlc_media_player_t* mediaPlayer = libvlc_media_list_player_get_media_player(vlcPlayer);
        if (mediaPlayer)
            libvlc_audio_set_volume(mediaPlayer, 100);
        return true;
        
    #endif
        return false;
    }


    bool AudioPlayer::isAudioPlaying() const
    {
    #if defined(IS_WINDOWS)
        return audioPlaying.load();
    #elif defined(IS_LINUX)
        std::lock_guard<std::mutex> lock(vlcMutex);
        return vlcPlayer && libvlc_media_list_player_is_playing(vlcPlayer);
    #endif
        return false;
    }


    void AudioPlayer::applyPlaybackMode()
    {
    #if defined(IS_LINUX)
        // Must be called while holding vlcMutex
        if (repeatAudioTrack)
            libvlc_media_list_player_set_playback_mode(vlcPlayer, libvlc_playback_mode_repeat);
        else if (playlistLoop)
            libvlc_media_list_player_set_playback_mode(vlcPlayer, libvlc_playback_mode_loop);
        else
            libvlc_media_list_player_set_playback_mode(vlcPlayer, libvlc_playback_mode_default);
            
        // Make sure volume is restored (important when looping).
        libvlc_media_player_t* mediaPlayer = libvlc_media_list_player_get_media_player(vlcPlayer);
        if (mediaPlayer)
            libvlc_audio_set_volume(mediaPlayer, 100);
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

        /*
         * Reset the flags so the background task runs the new playback. For
         * Linux, this is a mostly idle background task.
         */
        stopRequested = false;
        audioPlaying = true;
        start();
    
    #elif defined(IS_LINUX)
    
        std::lock_guard<std::mutex> lock(vlcMutex);

        if (libvlc_media_list_player_is_playing(vlcPlayer))
            return true;   // Already playing.

        { // Make sure the media list is up-to-date.
            std::lock_guard<std::mutex> plock(playlistMutex);
            rebuildVlcMediaList();
        }

        libvlc_media_list_player_play(vlcPlayer);
        
        // Give VLC time to catch up...
        //timing::sleepMilliseconds(50);

        // Apply current loop / repeat settings.
        applyPlaybackMode();
        
        // Force volume to maximum (in case it was left at 0)
        libvlc_media_player_t* mediaPlayer = libvlc_media_list_player_get_media_player(vlcPlayer);
        if (mediaPlayer)
            libvlc_audio_set_volume(mediaPlayer, 100);// Optional debug
        
        return true;
    
    #endif
        return true;
    }


    bool AudioPlayer::restartAudio()
    {        
    #if defined(IS_WINDOWS)

        stopAudio();
        return startAudio();

    #elif defined(IS_LINUX)

        std::lock_guard<std::mutex> lock(vlcMutex);

        if (!vlcPlayer)
            return false;

        // Get the media player that is currently playing.
        libvlc_media_player_t* mediaPlayer = libvlc_media_list_player_get_media_player(vlcPlayer);
        if (!mediaPlayer)
            return false;

        // Seek to the beginning and make sure it’s playing
        libvlc_media_player_set_time(mediaPlayer, 0); // Go back to the start.
        libvlc_media_player_play(mediaPlayer); // Make sure it's playing.

        // Restore volume just in case
        libvlc_audio_set_volume(mediaPlayer, 100);

        return true;

    #endif
    }


    bool AudioPlayer::stopAudio(uint32_t fadeOutMs)
    {
    #if defined(IS_WINDOWS)

        // Nothing to stop when no audio is playing.
        if (!audioPlaying)
            return true;
        
        /*
         * For a real fade, set the fade-specific flags and let the audio thread perform
         * the volume ramp + stop. For the zero-fade case just signal the worker.
         */
        if (fadeOutMs == 0)
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
                fadeTimeMS = fadeOutMs;
            }
            fadeRequested = true;
            fadeTimeMS = fadeOutMs;
        }

        // Stop and join the audio thread for a clean device close.
        stopRequested = true;
        stop();

    #elif defined(IS_LINUX)
    
        std::lock_guard<std::mutex> lock(vlcMutex);
        
        if (!vlcPlayer || !libvlc_media_list_player_is_playing(vlcPlayer))
            return true;   // Nothing playing means automatic success.
            
        if (fadeOutMs > 0)
        {
            // Need the underlying media player for volume control
            libvlc_media_player_t* player = libvlc_media_list_player_get_media_player(vlcPlayer);
            
            if (player)
            {
                // Simple linear fade-out (volume goes from 100 → 0)
                const uint32_t steps = std::max(1u, fadeOutMs / 25); // 25ms steps.
                const uint32_t stepTime = fadeOutMs / steps;

                for (int i=steps; i>=0; --i)
                {
                    int volume = (i * 100) / steps;
                    libvlc_audio_set_volume(player, volume);
                    timing::sleepMilliseconds(stepTime);
                }
            }
        }

        // Stop playback and release resources
        libvlc_media_list_player_stop(vlcPlayer);
        
    #endif

        return true;
    }


    void AudioPlayer::setRepeatAudioTrack(bool repeat)
    {
        repeatAudioTrack = repeat;
        
        #if defined(IS_LINUX)
            std::lock_guard<std::mutex> lock(vlcMutex);
            if (libvlc_media_list_player_is_playing(vlcPlayer))
                applyPlaybackMode();
        #endif
    }


    bool AudioPlayer::getRepeatAudioTrack() const
    {
        return repeatAudioTrack.load();
    }


    void AudioPlayer::setPlaylistLoop(bool loop)
    {
        playlistLoop = loop;
        
        #if defined(IS_LINUX)
            std::lock_guard<std::mutex> lock(vlcMutex);
            if (libvlc_media_list_player_is_playing(vlcPlayer))
                applyPlaybackMode();
        #endif
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
    #if defined(IS_WINDOWS)
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
            audioPlaying = false;
            stopRequested = true;
            return;
        }

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
    #endif
    }
} // namespace audio
