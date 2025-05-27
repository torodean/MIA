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


MIAApplication::MIAApplication() :
    verboseOpt("-v", "--verbose", "Enable verbose output.",
        CommandOption::commandOptionType::boolOption),
    helpOpt("-h", "--help", "Show this help message",
        CommandOption::commandOptionType::boolOption),
    logFileOpt("-l", "--logfile", "Set a custom logfile. Default = " +
        paths::getDefaultLogDirToUse() + "/" + logger::DEFAULT_LOG_FILE,
        CommandOption::commandOptionType::stringOption)
{ }


void MIAApplication::initialize(int argc, char* argv[])
{
    try
    {    
        verboseOpt.getOptionVal<bool>(argc, argv, verboseMode);
        helpOpt.getOptionVal<bool>(argc, argv, helpRequested);
        
        // Load (optionally if specified) the custom log file. 
        std::string customLogFile;
        logFileOpt.getOptionVal<std::string>(argc, argv, customLogFile);
        if (!customLogFile.empty())
            logger.setLogFile(customLogFile);
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
        std::exit(0); // Call exit after the --help flag is used. 
    }
}


void MIAApplication::printHelp() const
{
    std::cout << "Base MIA application options:" << std::endl
              << verboseOpt.getHelp() << std::endl
              << helpOpt.getHelp()  << std::endl
              << logFileOpt.getHelp()  << std::endl
              << std::endl;
}
