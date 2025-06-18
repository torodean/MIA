/**
 * File: MIATemplate.cpp
 * Author: Antonius Torode
 * Date: 05/24/2025
 * Description: Implementation of the MIATemplate app.
 */

#include <iostream>

// The associated header file.
#include "MIATemplate.hpp"

// Used for error handling.
#include "Paths.hpp"
#include "MIAException.hpp"

#include "Constants.hpp"
// Used for changing terminal colors.
#include "TerminalColors.hpp"

MIATemplate::MIATemplate() : 
    config(defaultConfigFile, constants::ConfigType::KEY_VALUE),
    configFileOpt("-c", "--config", "Specify a config file to use (default = " +
                                paths::getDefaultConfigDirToUse() + "/MIATemplate.MIA)",
                                CommandOption::commandOptionType::stringOption),
    testOpt("-t", "--test", "A test command option.",
                                CommandOption::commandOptionType::boolOption)                         
{ };


void MIATemplate::initialize(int argc, char* argv[])
{
    try
    {    
        MIAApplication::initialize(argc, argv);

        // Set the values from the command line arguments.
        testOpt.getOptionVal<bool>(argc, argv, testMode);
        
        std::string configFile = defaultConfigFile;
        configFileOpt.getOptionVal<std::string>(argc, argv, configFile);
        config.setConfigFileName(configFile, constants::ConfigType::KEY_VALUE); // handles config.initialize().
    }
    catch (const error::MIAException& ex)
    {
        std::cerr << "Error during MIATemplate::initialize: " << ex.what() << std::endl;
    }
    
    loadConfig();
}

void MIATemplate::loadConfig()
{
    LOG_METHOD_CALL(); // Used for testing log file calls.
    try
    {
        // Load configuration here.
        configFileVals.boolValue = config.getBool("MyBoolValue");
        configFileVals.intValue = config.getInt("myIntValue");
        configFileVals.doubleValue = config.getDouble("myDoubleValue");
        configFileVals.stringValue = config.getString("myStringValue");
        configFileVals.listValue = config.getVector("myListValue", ',');
    }
    catch (error::MIAException& ex)
    {
        std::cerr << "Error loading in configuration file value(s): " 
        << ex.what() 
        << "Continuing with default values."
        << std::endl;
    }
    
    // Optionally print the config values after it is loaded.
    if (getVerboseMode())
        config.dumpConfigMap();
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

void MIATemplate::TemplateConfig::printConfigValues() const 
{
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << "Printing values read in from the configuration file!" << std::endl;
    std::cout << "boolValue: " << std::boolalpha << boolValue << std::endl;
    std::cout << "intValue: " << intValue << std::endl;
    std::cout << "doubleValue: " << std::setprecision(10) << doubleValue << std::endl;
    std::cout << "stringValue: " << stringValue << std::endl;
    std::cout << "listValue: ";
    for (const auto& item : listValue) 
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
}


int MIATemplate::run()
{
    LOG_METHOD_CALL(); // Used for testing log file calls.
    std::cout << "Loading MIATemplate app!" << std::endl;
    
    if(testMode)
    {
        std::cout << "Test mode running!" << std::endl;
        configFileVals.printConfigValues();
        log("MIATemplate: Logging a test message!", true);
        if (getVerboseMode())
            std::cout << "Verbose mode enabled!" << std::endl; // This line used for testing verbose mode.
        std::cout << "Test mode finished! Exiting!" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Press ENTER to continue..." << std::endl;
        std::cin.get(); // Waits for ENTER key
        
        // This success method demonstrates the optional TerminalColors.
        std::cout << terminal_colors::green << "Success! Well done." << terminal_colors::reset << std::endl;
        configFileVals.printConfigValues();
        std::cout << "Exiting." << std::endl;
        return 0;
    }
    return 0;
}
