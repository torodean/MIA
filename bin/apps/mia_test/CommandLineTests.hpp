/**
 * @file CommandLineTests.hpp
 * @author Antonius Torode
 * @date 11/23/2026
 * @brief Various tests for the command line features.
 */
#pragma once

/**
 * @brief Tests running a command which prints the directory structure of this test app.
 *
 * Uses paths.hpp to get the folder of this test file, then runs 'ls -lh' on that
 * folder through command_line::command() and prints the listing.
 *
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testCommandLineDirectoryStructure(bool verboseMode);

/**
 * @brief Tests running a command which returns the date.
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testCommandLineDate(bool verboseMode);
