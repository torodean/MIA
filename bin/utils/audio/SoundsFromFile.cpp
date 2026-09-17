/**
 * @file SoundsFromFile.cpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief Implements the methods from the associated header file.
 */
 
#include <iostream>

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

namespace sounds
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
} // namespace sounds