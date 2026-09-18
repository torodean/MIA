/**
 * @file SystemSoundTests.hpp
 * @author Antonius Torode
 * @date 9/17/26
 * @brief Various tests for system sound-related features.
 */
#pragma once

/**
 * @brief Plays a system beep with various settings.
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS.
 */
int testSystemBeeps(bool verboseMode);

/**
 * @brief Tests playing a sound from a file.
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testPlayingSoundFromFile(bool verboseMode);

/**
 * @brief Tests playing a sound from a file which is stopped after 5 seconds.
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testPlayingSoundFromFileWithStop(bool verboseMode);

/**
 * @brief Tests playing a sound from a file which is faded out over 5 seconds
 *        when stopping.
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testPlayingSoundFromFileWithFade(bool verboseMode);
