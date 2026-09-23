/**
 * @file SystemSoundTests.cpp
 * @author Antonius Torode
 * @date 9/17/26
 * @brief Implements the features in the SystemSoundTests header file.
 */
 
#include <vector>
#include <algorithm>
#include <iostream>

// Include the associated header file.
#include "SystemSoundTests.hpp"
// Used for making system related sounds.
#include "SystemSounds.hpp"
// Used for system return codes.
#include "Constants.hpp"
// Used for playing sounds from a file.
#include "SoundsFromFile.hpp"
// Used for path detection and file finding.
#include "Paths.hpp"
// Used for timing capabilities.
#include "Timing.hpp"

int testSystemBeeps(bool verboseMode)
{
	// Ensure all sounds succeeded for return code.
	std::vector<bool> soundChecks;
	
	// Zelda area-unlock jingle.
    soundChecks.push_back(system_sounds::beep(784, 100));
    soundChecks.push_back(system_sounds::beep(740, 100));
    soundChecks.push_back(system_sounds::beep(622, 100));
    soundChecks.push_back(system_sounds::beep(440, 200));
    soundChecks.push_back(system_sounds::beep(415, 100));
    soundChecks.push_back(system_sounds::beep(659, 100));
    soundChecks.push_back(system_sounds::beep(831, 100));
    soundChecks.push_back(system_sounds::beep(1047, 500));
	
	// Return true unless any of the soundChecks is false.
	if (std::any_of(soundChecks.begin(), soundChecks.end(),
	    [](bool success){return !success;}))
		return constants::FAILURE;
	return constants::SUCCESS;
}


int testPlayingSoundFromFile(bool verboseMode)
{
	// Constructs the test file location.
	std::string thisFolder = paths::getCppFileDirAtCompileTime(__FILE__);
	std::string testFileName = "test_sound_short.mp3";
	std::string fullFilePath = thisFolder + "/" + testFileName;

/*
 * On a Cygwin system, the file paths are of the form '/cygdrive/s/...',
 * however, windows expects a windows specific filesystem form of 'S:\...'.
 * This will convert it before calling the files.
 */
#if defined(__CYGWIN__)
	fullFilePath = paths::cygwinPathToWindowsPath(fullFilePath);
#endif
	
	// Plays the sound from the test file.
	if (verboseMode)
		std::cout << "Playing sound from file: " << fullFilePath << std::endl;
	bool status = audio::playSoundFromFile(fullFilePath);
	if (verboseMode && !status)
		std::cout << "FAILED playing sound from file!" << std::endl;
	
	// Returns based on the status of the audio library call.
	return status ? constants::SUCCESS : constants::FAILURE;
}


int testPlayingSoundFromFileWithStop(bool verboseMode)
{
	// Constructs the test file location.
	std::string thisFolder = paths::getCppFileDirAtCompileTime(__FILE__);
	std::string testFileName = "test_sound_long.mp3";
	std::string fullFilePath = thisFolder + "/" + testFileName;

/*
 * On a Cygwin system, the file paths are of the form '/cygdrive/s/...',
 * however, windows expects a windows specific filesystem form of 'S:\...'.
 * This will convert it before calling the files.
 */
#if defined(__CYGWIN__)
	fullFilePath = paths::cygwinPathToWindowsPath(fullFilePath);
#endif
	
	// Plays the sound from the test file.
	if (verboseMode)
		std::cout << "Playing sound from file: " << fullFilePath << std::endl;
	bool status = audio::playSoundFromFileAsync(fullFilePath);
	if (!status)
	{
		if (verboseMode)
			std::cout << "FAILED playing sound from file!" << std::endl;
		return constants::FAILURE;
	}
	
	// Wait 5 seconds then stop the file.
	if (verboseMode)
		std::cout << "Sleeping for 5 seconds..." << std::endl;
    for (int i=5; i>0; i--)
    {
        if (verboseMode)
            std::cout << i << "..." << std::endl;
        timing::sleepSeconds(1);
    }

	// Stop the file.
	if (verboseMode)
		std::cout << "Stopping the sound!" << std::endl;
	status = audio::stopSound(0, verboseMode);
	if (!status)
	{
		if (verboseMode)
			std::cout << "FAILED stopping sound from file!" << std::endl;
		return constants::FAILURE;
	}
	
	// If it made it this far, it's a success.
	return constants::SUCCESS;
}


int testPlayingSoundFromFileWithFade(bool verboseMode)
{
	// Constructs the test file location.
	std::string thisFolder = paths::getCppFileDirAtCompileTime(__FILE__);
	std::string testFileName = "test_sound_long.mp3";
	std::string fullFilePath = thisFolder + "/" + testFileName;

/*
 * On a Cygwin system, the file paths are of the form '/cygdrive/s/...',
 * however, windows expects a windows specific filesystem form of 'S:\...'.
 * This will convert it before calling the files.
 */
#if defined(__CYGWIN__)
	fullFilePath = paths::cygwinPathToWindowsPath(fullFilePath);
#endif
	
	// Plays the sound from the test file.
	if (verboseMode)
		std::cout << "Playing sound from file: " << fullFilePath << std::endl;
	bool status = audio::playSoundFromFileAsync(fullFilePath);
	if (!status)
	{
		if (verboseMode)
			std::cout << "FAILED playing sound from file!" << std::endl;
		return constants::FAILURE;
	}
	
	// Wait 5 seconds then fade out over 5 seconds.
	if (verboseMode)
		std::cout << "Sleeping for 5 seconds..." << std::endl;
    for (int i=5; i>0; i--)
    {
        if (verboseMode)
            std::cout << i << "..." << std::endl;
        timing::sleepSeconds(1);
    }

	// Fade the sound out over 5 seconds.
	if (verboseMode)
		std::cout << "Fading out over 5 seconds..." << std::endl;
	status = audio::stopSound(5000, verboseMode);
	if (!status)
	{
		if (verboseMode)
			std::cout << "FAILED stopping sound from file!" << std::endl;
		return constants::FAILURE;
	}
	
	// If it made it this far, it's a success.
	return constants::SUCCESS;
}
