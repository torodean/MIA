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
#include "Error.hpp"
// Used for parsing command options.
#include "CommandParser.hpp"
// Used for default path locations.
#include "Paths.hpp"
#include "Logger.hpp"
#include "Constants.hpp"


MIAApplication::MIAApplication() :
    verboseOpt("-v", "--verbose", "Enable verbose output.",
        CommandOption::commandOptionType::BOOL_OPTION),
    debugOpt("-d", "--debug", "Enable debug output at a specified level.",
        CommandOption::commandOptionType::UNSIGNED_INT_OPTION),
    helpOpt("-h", "--help", "Show this help message",
        CommandOption::commandOptionType::BOOL_OPTION),
    logFileOpt("-l", "--logfile", "Set a custom logfile. Default = " +
        paths::getDefaultLogDirToUse() + "/" + logger::DEFAULT_LOG_FILE,
        CommandOption::commandOptionType::STRING_OPTION)
{ }


void MIAApplication::initialize(int argc, char* argv[])
{
    try
    {
        // Set the executable name.
        executableName = argv[0];
        
        // Set the command option parameters for verbose and help.
        verboseOpt.getOptionVal<bool>(argc, argv, context.verboseMode);
        debugOpt.getOptionVal<unsigned int>(argc, argv, context.debugLevel);
        helpOpt.getOptionVal<bool>(argc, argv, helpRequested);
        
        // Load (optionally if specified) the custom log file. 
        std::string customLogFile;
        logFileOpt.getOptionVal<std::string>(argc, argv, customLogFile);
        if (!customLogFile.empty())
            context.logger.setLogFile(customLogFile);
    }
    catch (const error::MIAException& ex)
    {
        // This shouldn't ever throw a MIAException since it's simple and part of the base app, 
        // but throw an exception just in case... 
        std::string err = "Error during MIAApplication::initialize!";
        throw error::MIAException(error::ErrorCode::Catastrophic_Failure, err); 
    }

    if (helpRequested)
    {
        printHelp();
        std::exit(constants::SUCCESS); // Call exit after the --help flag is used. 
    }
}


void MIAApplication::printHelp() const
{
    std::cout << "Usage: " << executableName << " [args]" << std::endl
              << "Base MIA application options:" << std::endl
              << verboseOpt.getHelp() << std::endl
              << debugOpt.getHelp() << std::endl
              << helpOpt.getHelp()  << std::endl
              << logFileOpt.getHelp()  << std::endl
              << std::endl;
}
