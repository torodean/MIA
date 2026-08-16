/**
 * @file Maple.cpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief TODO
 */

// The pairing header file which defines the class methods for Maple.
#include "Maple.hpp"
// Used for error handling and configuration.
#include "Paths.hpp"
#include "MIAException.hpp"
// Used for the ConfigType
#include "Constants.hpp"


namespace maple
{
    Maple::Maple() :
        configOpt("-c", "--configFile", "Specify the config file to use (default = " +
                  paths::getDefaultConfigDirToUse() + "/" + defaultConfigFile,
                  CommandOption::commandOptionType::STRING_OPTION),
        testOpt("-t", "--test", 
                "Enables test mode. This mode will allow various testing during development.",
                CommandOption::commandOptionType::BOOL_OPTION)
    {}
    

    void Maple::initialize(int argc, char* argv[])
    {
        try
        {
            MIAApplication::initialize(argc, argv);
            
            // Set the values from the command line arguments.
            testOpt.getOptionVal<bool>(argc, argv, testMode);
            sequenceNameOpt.getOptionVal<std::string>(argc, argv, sequenceName);
            
            // Set and load the config file.
            std::string configFile = defaultConfigFile;
            configOpt.getOptionVal<std::string>(argc, argv, configFile);
            // handles config.initialize().
            config.setConfigFileName(configFile, constants::ConfigType::KEY_VALUE);
        }
        catch (const error::MIAException& ex)
        {
            std::cerr << "Error during Maple::initialize: " << ex.what() << std::endl;
        }
        
        if (!loadConfig())
        {
            // TODO - handle error case.
        }
    }
    

    int Maple::run()
    {}


    void Maple::printHelp() const
    {
        MIAApplication::printHelp();
    
        // This is a dump of the help messages used by the various command options.
        std::cout << "Maple specific options:" << std::endl
                  << configOpt.getHelp() << std::endl
                  << testOpt.getHelp() << std::endl
                  << std::endl;
    }


    bool Maple::loadConfig()
    {}
} // namespace Maple
