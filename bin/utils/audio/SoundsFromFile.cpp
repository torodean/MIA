/**
 * @file SoundsFromFile.cpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief Implements the methods from the associated header file.
 */
 
#include <iostream>
#include <thread>
#include <atomic>

// The associated header file.
#include "SoundsFromFile.hpp"
// Used for preprocessor macros.
#include "Constants.hpp"
// Used for error returns.
#include "MIAException.hpp"
#include "Error.hpp"

#if defined(IS_WINDOWS)
	#include <Windows.h>
	#include <mmsystem.h>
#endif

namespace audio
{
	bool playSoundFromFile(const std::string& fileName)
	{
#if defined(IS_WINDOWS)
		std::string fileEnding = fileName.substr(fileName.size()-4, fileName.size());
		if (fileEnding != ".mp3")
		{
			std::cerr << "Unsupported file type specified!" << std::endl;
			return false;
		}

		std::string command = "open \"" + fileName + "\" type mpegvideo alias mp3";

		MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
		if (error != 0)
		{
			char errorText[256];
			mciGetErrorString(error, errorText, sizeof(errorText));
			std::cerr << "Failed to open sound file: " << errorText << std::endl;
			return false;
		}

		error = mciSendString("play mp3 wait", NULL, 0, NULL);
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


	namespace audio_thread
	{
		std::thread soundThread;
		std::atomic<bool> soundPlaying = false;
	}	


	bool playSoundFromFileAsync(const std::string& fileName)
	{
	#if defined(IS_WINDOWS)
		std::string fileEnding = fileName.substr(fileName.size()-4, fileName.size());
		if (fileEnding != ".mp3")
		{
			std::cerr << "Unsupported file type specified!" << std::endl;
			return false;
		}

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

		audio_thread::soundThread = std::thread([fileName]()
		{
			std::string command = "open \"" + fileName + "\" type mpegvideo alias mp3";

			MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
			if (error != 0)
			{
				char errorText[256];
				mciGetErrorString(error, errorText, sizeof(errorText));
				std::cerr << "Failed to open sound file: " << errorText << std::endl;
				audio_thread::soundPlaying = false;
				return;
			}

			error = mciSendString("play mp3 wait", NULL, 0, NULL);
			if (error != 0)
			{
				char errorText[256];
				mciGetErrorString(error, errorText, sizeof(errorText));
				std::cerr << "Failed to play sound file: " << errorText << std::endl;
			}

			mciSendString("close mp3", NULL, 0, NULL);
			audio_thread::soundPlaying = false;
		});

		return true;
	#else
		MIA_THROW(error::ErrorCode::Windows_Only_Feature,
				  "This method is not yet supported on Linux.");
	#endif
		return false;
	}


	bool stopSound(uint32_t fadeOutMs)
	{
	#if defined(IS_WINDOWS)
		// Return false if no sound is playing.
		if (!audio_thread::soundPlaying)
			return true;

		// Stop it immediately if there is no fade out.
		if (fadeOutMs == 0)
		{
			MCIERROR error = mciSendString("stop mp3", NULL, 0, NULL);
			if (error != 0)
			{
				char errorText[256];
				mciGetErrorString(error, errorText, sizeof(errorText));
				std::cerr << "Failed to stop sound: " << errorText << std::endl;
				return false;
			}

			return true;
		}

		// Fade out by continually reducing the volume.
		std::thread([fadeOutMs]()
		{
			const uint32_t steps = 20;
			const uint32_t stepTime = fadeOutMs / steps;

			for (uint32_t i = steps; i > 0 && audio_thread::soundPlaying; --i)
			{
				uint32_t volume = (1000 * (i - 1)) / steps;

				std::string command = "setaudio mp3 volume to " + std::to_string(volume);

				MCIERROR error = mciSendString(command.c_str(), NULL, 0, NULL);
				if (error != 0)
				{
					char errorText[256];
					mciGetErrorString(error, errorText, sizeof(errorText));
					std::cerr << "Failed to set sound volume: "
						<< errorText << std::endl;
					return;
				}

				std::this_thread::sleep_for(
					std::chrono::milliseconds(stepTime));
			}

			if (audio_thread::soundPlaying)
			{
				MCIERROR error = mciSendString("stop mp3", NULL, 0, NULL);

				if (error != 0)
				{
					char errorText[256];
					mciGetErrorString(error, errorText, sizeof(errorText));
					std::cerr << "Failed to stop sound: "
						<< errorText << std::endl;
				}
			}
		}).detach();

		return true;
	#else
		MIA_THROW(error::ErrorCode::Windows_Only_Feature,
				  "This method is not yet supported on Linux.");
	#endif
		return false;
	}
} // namespace audio