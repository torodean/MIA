/**
 * File: MIASequencer.cpp
 * Author: Antonius Torode
 * Date: 05/24/2025
 * Description: Implementation of the MIASequencer app.
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>

// The associated header file.
#include "MIASequencer.hpp"

// Used for error handling.
#include "Paths.hpp"
#include "MIAException.hpp"
// Used for the ConfigType.
#include "Constants.hpp"

using std::string;
using std::remove;
using std::ifstream;
using std::stoi;
using std::vector;
using std::cout;
using std::endl;
using std::pair;


MIASequencer::MIASequencer() : 
    config(defaultConfigFile, constants::ConfigType::RAW_LINES),
    configFileOpt("-c", "--config", "Specify a config file to use (default = " +
                                paths::getDefaultConfigDirToUse() + "/MIASequences.MIA)",
                                CommandOption::commandOptionType::stringOption),
    testOpt("-t", "--test", "Enables test mode. This mode will only output the sequence to terminal.",
                                CommandOption::commandOptionType::boolOption)
{ };


void MIASequencer::initialize(int argc, char* argv[])
{
    try
    {    
        MIAApplication::initialize(argc, argv);

        // Set the values from the command line arguments.
        testOpt.getOptionVal<bool>(argc, argv, testMode);
        
        std::string configFile = defaultConfigFile;
        configFileOpt.getOptionVal<std::string>(argc, argv, configFile);
        config.setConfigFileName(configFile, constants::ConfigType::RAW_LINES); // handles config.initialize().
    }
    catch (const error::MIAException& ex)
    {
        std::cerr << "Error during MIASequencer::initialize: " << ex.what() << std::endl;
    }
    
    loadConfig();
}

void MIASequencer::loadConfig()
{
    // TODO
}

void MIATemplate::printHelp() const
{
    MIAApplication::printHelp();
    
    // This is a dump of the help messages used by the various command options.
    std::cout << "MIATemplate specific options:" << std::endl
              << configFileOpt.getHelp() << std::endl
              << testOpt.getHelp() << std::endl
              << std::endl;
}


int MIATemplate::run()
{
    // TODO
    return 0;
}
