/**
 * @file SystemSoundTests.hpp
 * @author Antonius Torode
 * @date 9/17/26
 * @brief Various tests for system sound-related features.
 */
#pragma once


/**
 * Plays a system beep with various settings.
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS.
 */
int testSystemBeeps(bool verboseMode);


/**
 * Tests playing a sound from a file.
 * @param verboseMode Whether or not to enable verbose output.
 * returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testPlayingSoundFromFile(bool verboseMode);

/**
 * Tests playing a sound from a file. This tests that an audio file can be 
 * stopped.
 * @param verboseMode Whether or not to enable verbose output.
 * returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testPlayingSoundFromFileWithStop(bool verboseMode);

/**
 * Tests playing a sound from a file. This tests that an audio file can be 
 * faded out when stopping.
 * @param verboseMode Whether or not to enable verbose output.
 * returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testPlayingSoundFromFileWithFade(bool verboseMode);