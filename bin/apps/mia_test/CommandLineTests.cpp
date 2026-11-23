/**
 * @file CommandLineTests.cpp
 * @author Antonius Torode
 * @date 11/23/2026
 * @brief Implements the features in the CommandLineTests header file.
 */

#include <iostream>

// Include the associated header file.
#include "CommandLineTests.hpp"
// Used for running command line commands.
#include "CommandLine.hpp"
// Used for system return codes.
#include "Constants.hpp"
// Used for path detection and file finding.
#include "Paths.hpp"

int testCommandLineDirectoryStructure(bool verboseMode)
{
	std::string thisFolder = paths::getCppFileDirAtCompileTime(__FILE__);

	std::string listingCommand = "ls -lh " + thisFolder;

	if (verboseMode)
		std::cout << "Running command: " << listingCommand << std::endl;
	command_line::CommandData data = command_line::command(listingCommand);

	std::cout << data.stdout << std::endl;

	if (!data.isSuccess())
	{
		std::cout << "FAILED listing the directory structure of: " << thisFolder << std::endl;
		if (verboseMode)
			std::cout << data.stderr << std::endl;
		return constants::FAILURE;
	}
	return constants::SUCCESS;
}


int testCommandLineDate(bool verboseMode)
{
	if (verboseMode)
		std::cout << "Running command: date" << std::endl;
	command_line::CommandData data = command_line::command("date");

	// The date output ends with a newline.
	std::cout << data.stdout;

	if (!data.isSuccess())
	{
		std::cout << "FAILED running the date command." << std::endl;
		if (verboseMode)
			std::cout << data.stderr << std::endl;
		return constants::FAILURE;
	}
	return constants::SUCCESS;
}
