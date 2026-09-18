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
#endif

namespace audio
{
    bool isASupportedType(const files::FileMetaData& data,
                          bool verboseMode)
    {
        if (data.type == files::FileType::Mp3 || data.type == files::FileType::Wav )
            return true;
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
    #if defined(IS_WINDOWS)
        files::FileMetaData fileMetaData = files::getFileMetaData(fileName);
        if (!isASupportedType(fileMetaData))
        {
            std::cerr << "Unsupported file type specified!" << std::endl;
            return false;
        }

        // Setup the correct command to run based on the file type.
        std::string command;
        if (fileMetaData.type == files::FileType::Mp3)
            command = "open \"" + fileMetaData.fullFilePath + "\" type mpegvideo alias mp3";
        else if (fileMetaData.type == files::FileType::Wav)
            command = "open \"" + fileMetaData.fullFilePath + "\" type waveaudio alias wav";
        else
        {
            /*
             * The isASupportedType(fileMetaData) should catch any types which_open_mode
             * are not supported, so this should never reach this point. I am including it
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
    #else
        MIA_THROW(error::ErrorCode::Windows_Only_Feature,
                  "This method is not yet supported on Linux.");
    #endif
        return false;
    }


    /**
     * This namespace defines threading values used to track which audio
     * file is currently being played. This allows the stopSound() method
     * to stop the thread playing the audio.
     */
    namespace audio_thread
    {
        std::thread soundThread;
        std::atomic<bool> soundPlaying = false;
        std::atomic<bool> fadeRequested = false;
        std::atomic<uint32_t> fadeTimeMS = 0;
        std::atomic<files::FileType> currentFileType = files::FileType::Unknown;
        
        /**
         * Helper to return an alias based on the file type. Used in various commands.
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
                 * The isASupportedType(fileMetaData) should catch any types which_open_mode
                 * are not supported, so this should never reach this point. I am including it
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
                    audio_thread::fadeTimeMS = 0; // Reset 
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
    #else
        MIA_THROW(error::ErrorCode::Windows_Only_Feature,
                  "This method is not yet supported on Linux.");
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
    #else
        MIA_THROW(error::ErrorCode::Windows_Only_Feature,
                  "This method is not yet supported on Linux.");
    #endif
        return false;
    }
} // namespace audio