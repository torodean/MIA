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


MIAApplication::MIAApplication() :
    verboseOpt("-v", "--verbose", "Enable verbose output.",
        CommandOption::commandOptionType::boolOption),
    helpOpt("-h", "--help", "Show this help message",
        CommandOption::commandOptionType::boolOption)
{ }


void MIAApplication::initialize(int argc, char* argv[])
{
    try
    {    
        verboseOpt.getOptionVal<bool>(argc, argv, verboseMode);
        helpOpt.getOptionVal<bool>(argc, argv, helpRequested);
    }
    catch (const error::MIAException& ex)
    {
        std::cerr << "Error during MIAApplication::initialize: " << ex.what() << std::endl;
        throw; // This shouldn't throw a MIAException so if it does, it indicates something very wrong. Thus, throw...
    }

    if (helpRequested)
    {
        printHelp();
        std::exit(0); // Call exit after the --help flag is used. 
    }
}


void MIAApplication::printHelp() const
{
    std::cout << "Base MIA application options:" << std::endl
              << verboseOpt.getHelp() << std::endl
              << helpOpt.getHelp()  << std::endl
              << std::endl;
}
