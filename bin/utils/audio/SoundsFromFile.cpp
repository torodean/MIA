/**
 * @file SoundsFromFile.cpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief Implements the methods from the associated header file.
 */
 
#include <iostream>
#include <thread>
#include <atomic>
#include <algorithm>

// The associated header file.
#include "SoundsFromFile.hpp"
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
#elif defined(IS_LINUX)
    #include <vlc/vlc.h>
#endif

namespace audio
{
    namespace
    {
        /**
         * @brief Synchronization state for waiting on VLC media playback completion.
         *
         * Used together with libvlc_event_attach() so that a calling thread can
         * block until the media player reports that playback has finished.
         */
        struct PlaybackState
        {
            std::mutex mutex;
            std::condition_variable condition;
            bool finished = false;
        };


        /**
         * @brief libVLC event callback that signals playback completion.
         *
         * Attached to the @c libvlc_MediaPlayerEndReached event. When that event
         * fires, the callback sets @c PlaybackState::finished and notifies any
         * thread waiting on the associated condition variable.
         *
         * @param event The libVLC event that was raised.
         * @param userData Pointer to a @c PlaybackState instance (must not be null).
         */
        void mediaPlayerEventCallback(const libvlc_event_t* event, void* userData)
        {
            if (event->type != libvlc_MediaPlayerEndReached)
                return;

            auto* state = static_cast<PlaybackState*>(userData);

            {
                std::lock_guard<std::mutex> lock(state->mutex);
                state->finished = true;
            }

            state->condition.notify_one();
        }
    } // namespace


    bool isASupportedType(const files::FileMetaData& data,
                          bool verboseMode)
    {
        if (data.type == files::FileType::Mp3) return true;
    #if defined(IS_WINDOWS)
        if (data.type == files::FileType::Wav) return true;
    #endif
        return false;
    }


    bool isASupportedType(const std::string& fileName,
                          bool verboseMode)
    {
        files::FileMetaData data = files::getFileMetaData(fileName, verboseMode);
        return isASupportedType(data, verboseMode);
    }


    bool playSoundFromFile(const std::string& fileName)
    {
        files::FileMetaData fileMetaData = files::getFileMetaData(fileName);
        if (!isASupportedType(fileMetaData))
        {
            std::cerr << "Unsupported file type specified!" << std::endl;
            return false;
        }

    #if defined(IS_WINDOWS)

        // Setup the correct command to run based on the file type.
        std::string command;
        if (fileMetaData.type == files::FileType::Mp3)
            command = "open \"" + fileMetaData.fullFilePath + "\" type mpegvideo alias mp3";
        else if (fileMetaData.type == files::FileType::Wav)
            command = "open \"" + fileMetaData.fullFilePath + "\" type waveaudio alias wav";
        else
        {
            /*
             * The isASupportedType(fileMetaData) should catch any types
             * which are not supported, so this should never reach this point. I am including it
             * here as a quick-check for if a dev forgets to update isASupportedType() when
             * adding more file support.
             */
            return false;
        }

        // Run the command.
        MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
        if (error != 0)
        {
            char errorText[256];
            mciGetErrorString(error, errorText, sizeof(errorText));
            std::cerr << "Failed to open sound file " << fileMetaData.fullFilePath 
                      << ": " << errorText << std::endl;
            return false;
        }
        
        if (fileMetaData.type == files::FileType::Mp3)
            error = mciSendString("play mp3 wait", NULL, 0, NULL);
        else if (fileMetaData.type == files::FileType::Wav)
            error = mciSendString("play wav wait", NULL, 0, NULL);
        if (error != 0)
        {
            char errorText[256];
            mciGetErrorString(error, errorText, sizeof(errorText));
            std::cerr << "Failed to play sound file: " << errorText << std::endl;
            return false;
        }

        // All appeared to have succeeded.
        return true;
        
    #elif defined(IS_LINUX)
    
        // Create an instance of vlc to use.
        libvlc_instance_t* instance = libvlc_new(0, nullptr);
        if (!instance)
        { // Check for errors.
            std::cerr << "Failed to create VLC instance!" << std::endl;
            return false;
        }

        // Create a vlc media object with the sound file loaded.
        libvlc_media_t* media = libvlc_media_new_path(instance, fileName.c_str());
        if (!media)
        { // Check for errors.
            std::cerr << "Failed to create VLC media!" << std::endl;
            libvlc_release(instance);
            return false;
        }    
        
        // Create the player to actually play the media.
        libvlc_media_player_t* player = libvlc_media_player_new_from_media(media);
        libvlc_media_release(media);
        if (!player)
        {
            std::cerr << "Failed to create VLC media player!" << std::endl;
            libvlc_release(instance);
            return false;
        }
        
        // Force full volume.
        libvlc_audio_set_volume(player, 100);

        // Create an eventManager to listen for events.
        PlaybackState playbackState;
        libvlc_event_manager_t* eventManager = libvlc_media_player_event_manager(player);
        libvlc_event_attach(eventManager,                 // The event manager to use.
                            libvlc_MediaPlayerEndReached, // The end of the media has been reached.
                            mediaPlayerEventCallback,     // The callback which sets 'finished.'
                            &playbackState);              // Storage that holds 'finished.'

        // Start playing the media.
        if (libvlc_media_player_play(player) == -1)
        { // Check for errors, then cleanup accordingly.
            std::cerr << "Failed to start playback!" << std::endl;

            libvlc_event_detach(eventManager,
                                libvlc_MediaPlayerEndReached,
                                mediaPlayerEventCallback,
                                &playbackState);

            libvlc_media_player_release(player);
            libvlc_release(instance);
            return false;
        }

        { // Wait until the audio is finished playing before exiting.
            std::unique_lock<std::mutex> lock(playbackState.mutex);

            playbackState.condition.wait(lock, [&playbackState]
            {
                return playbackState.finished;
            });
        }

        // Audio has ended. Detach the event callback and release all VLC objects.
        libvlc_event_detach(eventManager,
                            libvlc_MediaPlayerEndReached,
                            mediaPlayerEventCallback,
                            &playbackState);

        libvlc_media_player_release(player);
        libvlc_release(instance);

        return true;
        
    #endif
    
        return false;
    }


    /**
     * @brief Defines threading values used to track which audio file is
     *        currently being played and other audio variables.
     *
     * This allows the stopSound() method to stop the thread playing the audio.
     */
    namespace audio_thread
    {
    #if defined(IS_WINDOWS)
    
        std::thread soundThread;
        std::atomic<bool> soundPlaying = false;
        std::atomic<bool> fadeRequested = false;
        std::atomic<uint32_t> fadeTimeMS = 0;
        std::atomic<files::FileType> currentFileType = files::FileType::Unknown;

        /**
         * @brief Returns an alias based on the file type. Used in various commands
         * for the windows-specific MCI calls.
         */
        std::string getSoundAlias()
        {
            std::string soundAlias;
            if (audio_thread::currentFileType == files::FileType::Mp3)
                soundAlias = "mp3";
            else if (audio_thread::currentFileType == files::FileType::Wav)
                soundAlias = "wav";
            
            return soundAlias;
        }
        
    #elif defined(IS_LINUX)
        
        libvlc_instance_t* vlcInstance = nullptr;
        libvlc_media_player_t* vlcPlayer = nullptr;
        std::mutex vlcPlayerMutex;   // protect access to the above
        
    #endif
    } // namespace audio_thread


    bool playSoundFromFileAsync(const std::string& fileName)
    {
    #if defined(IS_WINDOWS)
    
        // Check if the file is a supported type.
        files::FileMetaData fileMetaData = files::getFileMetaData(fileName);
        if (!isASupportedType(fileMetaData))
        {
            std::cerr << "Unsupported file type specified!" << std::endl;
            return false;
        }
        
        // Check if a sound is already playing.
        if (audio_thread::soundPlaying)
        {
            std::cerr << "A sound is already playing!" << std::endl;
            return false;
        }

        if (audio_thread::soundThread.joinable())
        {
            audio_thread::soundThread.join();
        }

        audio_thread::soundPlaying = true;
        audio_thread::currentFileType = fileMetaData.type;

        audio_thread::soundThread = std::thread([fileMetaData]()
        {
            // Setup the correct command to run based on the file type.
            std::string command;
            if (fileMetaData.type == files::FileType::Mp3)
                command = "open \"" + fileMetaData.fullFilePath + "\" type mpegvideo alias mp3";
            else if (fileMetaData.type == files::FileType::Wav)
                command = "open \"" + fileMetaData.fullFilePath + "\" type waveaudio alias wav";
            else
            {
                /*
                 * The isASupportedType(fileMetaData) should catch any types
                 * which are not supported, so this should never reach this point. I am including it
                 * here as a quick-check for if a dev forgets to update isASupportedType() when
                 * adding more file support.
                 */
                return;
            }            
            // Run the command.
            MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
            if (error != 0)
            {
                char errorText[256];
                mciGetErrorString(error, errorText, sizeof(errorText));
                std::cerr << "Failed to open sound file " << fileMetaData.fullFilePath 
                          << ": " << errorText << std::endl;
                audio_thread::soundPlaying = false;
                return;
            }
            
            // Get the file type of the currently playing audio for the audio commands.
            std::string soundAlias = audio_thread::getSoundAlias();

            // Play the audio.
            command = "play " + soundAlias;
            error = mciSendString(command.c_str(), NULL, 0, NULL);
            if (error != 0)
            {
                char errorText[256];
                mciGetErrorString(error, errorText, sizeof(errorText));
                std::cerr << "Failed to play sound file: " << errorText << std::endl;
            }
            
            // Continue while no stop condition is defined.
            while (audio_thread::soundPlaying.load())
            {          
                // Check the status of the audio being played.
                char status[32] = {};
                mciSendString(("status " + soundAlias + " mode").c_str(), 
                              status, sizeof(status), NULL);
                if (std::string(status) == "stopped") {
                    audio_thread::soundPlaying = false;
                    break;
                }
                
                // Handle the fade out by adjusting the volume to zero.
                if (audio_thread::fadeRequested)
                {
                    // Fade out by continually reducing the volume.
                    const uint32_t steps = std::max(1u, audio_thread::fadeTimeMS.load() / 25); // 25ms steps.
                    const uint32_t stepTime = audio_thread::fadeTimeMS / steps;

                    for (uint32_t i = steps; i > 0; --i)
                    {
                        uint32_t volume = (1000 * (i - 1)) / steps;

                        std::string command = "setaudio " + soundAlias + 
                                              " volume to " + std::to_string(volume);
                        MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
                        if (error != 0)
                        {
                            char errorText[256];
                            mciGetErrorString(error, errorText, sizeof(errorText));
                            std::cerr << "Failed to set sound volume: "
                                << errorText << std::endl;
                            return;
                        }

                        timing::sleepMilliseconds(stepTime);
                    }
                    
                    audio_thread::fadeRequested = false; // Reset value.
                    audio_thread::fadeTimeMS = 0; // Reset value.
                    audio_thread::soundPlaying = false; // Trigger the stop.
                } // if (audio_thread::fadeRequested)
                    
                // Sleep between polls.
                timing::sleepMilliseconds(5);
            }
            
            // Stop the audio.            
            std::string stopCommand = "stop " + soundAlias;
            error = mciSendString(stopCommand.c_str(), NULL, 0, NULL); 
            if (error != 0)
            {
                char errorText[256];
                mciGetErrorString(error, errorText, sizeof(errorText));
                std::cerr << "Failed to stop sound file: " << errorText << std::endl;
            }
            // Close the audio.
            std::string closeCommand = "close " + soundAlias;
            error = mciSendString(closeCommand.c_str(), NULL, 0, NULL);
            if (error != 0)
            {
                char errorText[256];
                mciGetErrorString(error, errorText, sizeof(errorText));
                std::cerr << "Failed to close sound file: " << errorText << std::endl;
            }
            
            audio_thread::soundPlaying = false;
        });

        return true;
        
    #elif defined(IS_LINUX)

        std::lock_guard<std::mutex> lock(audio_thread::vlcPlayerMutex);
        
        // Create an instance of vlc to use.
        audio_thread::vlcInstance = libvlc_new(0, nullptr);
        if (!audio_thread::vlcInstance)
        { // Check for errors.
            std::cerr << "Failed to create VLC instance!" << std::endl;
            return false;
        }

        // Create a vlc media object with the sound file loaded.
        libvlc_media_t* media = libvlc_media_new_path(audio_thread::vlcInstance, fileName.c_str());
        if (!media)
        { // Check for errors.
            std::cerr << "Failed to create VLC media!" << std::endl;
            libvlc_release(audio_thread::vlcInstance);
            return false;
        }
        
        // Create the player to actually play the media.
        audio_thread::vlcPlayer = libvlc_media_player_new_from_media(media);
        libvlc_media_release(media);
        if (!audio_thread::vlcPlayer)
        { // Check for errors.
            std::cerr << "Failed to create VLC media player!" << std::endl;
            libvlc_release(audio_thread::vlcInstance);
            return false;
        }
        
        // Force full volume.
        libvlc_audio_set_volume(audio_thread::vlcPlayer, 100);

        // Start playing the media.
        if (libvlc_media_player_play(audio_thread::vlcPlayer) == -1)
        { // Check for errors, then cleanup accordingly.
            std::cerr << "Failed to start playback!" << std::endl;

            libvlc_media_player_release(audio_thread::vlcPlayer);
            libvlc_release(audio_thread::vlcInstance);
            return false;
        }

        // Success. Playback should continue in the background.
        return true;
        
    #endif
        return false;
    }


    bool stopSound(uint32_t fadeOutMs, bool verboseMode)
    {
    #if defined(IS_WINDOWS)
    
        // Return false if no sound is playing.
        if (!audio_thread::soundPlaying)
            return true;
        
        /* 
         * For a real fade, set the atomic fade-specific flags and let the audio thread 
         * perform the volume ramp + stop. For the zero-fade case just signal the worker.
         */
        if (fadeOutMs == 0)
            audio_thread::soundPlaying = false;
        else
        {
            if (audio_thread::currentFileType != files::FileType::Mp3)
            { // The 'setaudio .. volume to ..' command is not supported for other types.
                audio_thread::soundPlaying = false;
                if (verboseMode)
                    std::cerr << "WARNING: The fade out sound option is unsupported for"
                              << " non-mp3 audio files!" << std::endl;
            }
            audio_thread::fadeRequested = true;
            audio_thread::fadeTimeMS = fadeOutMs;
        }
        
        if (audio_thread::soundThread.joinable())
            audio_thread::soundThread.join();   // Wait for clean close.

        return true;
        
    #elif defined(IS_LINUX)

        std::lock_guard<std::mutex> lock(audio_thread::vlcPlayerMutex);
        
        if (!audio_thread::vlcPlayer)
            return true;   // Nothing playing means automatic success.
            
        if (fadeOutMs > 0)
        {
            // Simple linear fade-out (volume goes from 100 → 0)
            const uint32_t steps = std::max(1u, fadeOutMs / 25); // 25ms steps.
            const uint32_t stepTime = fadeOutMs / steps;

            for (int i = steps; i >= 0; --i)
            {
                int volume = (i * 100) / steps;
                libvlc_audio_set_volume(audio_thread::vlcPlayer, volume);
                timing::sleepMilliseconds(stepTime);
            }
        }

        // Stop playback and release resources
        libvlc_media_player_stop(audio_thread::vlcPlayer);
        libvlc_media_player_release(audio_thread::vlcPlayer);
        libvlc_release(audio_thread::vlcInstance);

        audio_thread::vlcPlayer = nullptr;
        audio_thread::vlcInstance = nullptr;

        if (verboseMode)
            std::cout << "Sound stopped." << std::endl;

        return true;

    #endif
        return false;
    }
} // namespace audio
