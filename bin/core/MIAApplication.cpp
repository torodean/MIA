/**
 * File: BasicApplication.cpp
 * Author: Antonius Torode
 * Creation Date: 05/22/2025
 * Description: Implements the BasicApplication base class methods.
 *              Handles common initialization such as parsing command line
 *              arguments for verbose mode.
 */

#include <iostream>
#include <string>

// The associated header file.
#include "MIAApplication.hpp"
// Used for exception handling.
#include "MIAException.hpp"
// Used for parsing command options.
#include "CommandParser.hpp"


void MIAApplication::initialize(int argc, char* argv[])
{
    try
    {
        command_parser::parseBoolFlag(argc, argv, "-v", "--verbose", verboseMode);
        command_parser::parseBoolFlag(argc, argv, "-h", "--help", helpRequested);
    }
    catch (const error::MIAException& ex)
    {
        // Handle unexpected error during flag parsing (shouldn't happen with parseBoolFlag)
        std::cerr << "Error during MIAApplication::initialize: " << ex.what() << std::endl;
        throw;
    }

    if (helpRequested)
    {
        printHelp();
        return; // TODO - Signal to caller that help was requested, app should exit
    }
}


void MIAApplication::printHelp() const
{
    std::cout << "Base application options:\n"
              << "  -v, --verbose    Enable verbose output\n"
              << "  -h, --help       Show this help message\n";
}
