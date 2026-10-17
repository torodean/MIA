/**
 * @file Check.cpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief Implementation of the Check application class.
 */

// The associated header file.
#include "Check.hpp"

#include <iostream>

// Used for error handling.
#include "Paths.hpp"
#include "MIAException.hpp"

#include "Constants.hpp"

// Used for testing purposes.
#include "Timing.hpp"

namespace check
{
    Check::Check() : 
        config(defaultConfigFile, constants::ConfigType::KEY_VALUE),
        configFileOpt("-c", "--config", "Specify a config file to use (default = " +
                                    paths::getDefaultConfigDirToUse() + defaultConfigFile,
                                    CommandOption::commandOptionType::STRING_OPTION),
        module(defaultModuleName, __FILE__),
        listener(module, queue)
    { };


    void Check::initialize(int argc, char* argv[])
    {
        try
        {    
            MIAApplication::initialize(argc, argv);

            std::string configFile = defaultConfigFile;
            configFileOpt.getOptionVal<std::string>(argc, argv, configFile);
            config.setConfigFileName(configFile, constants::ConfigType::KEY_VALUE); // handles config.initialize().
        }
        catch (const error::MIAException& ex)
        {
            std::cerr << "Error during Check::initialize: " << ex.what() << std::endl;
        }

        loadConfig();
    }


    void Check::loadConfig()
    {
        try
        { // Load configuration here.
            configValues = constructConfig(config);
            
            /*
             * The verboseMode can be overwridden from the config file. The desired behavior
             * here is to enable verbose mode if it is on anywhere (command line or config).
             */
            configValues.verboseMode = configValues.verboseMode || getVerboseMode();
        }
        catch (error::MIAException& ex)
        {
            std::cerr << "Error loading in configuration file value(s): " << ex.what();
            // TODO - rethrow here.
        }

        if (configValues.verboseMode)
        { // Optionally print the config values after it is loaded.
            config.dumpConfigMap();
        }
    }


    void Check::printHelp() const
    {
        MIAApplication::printHelp();

        // This is a dump of the help messages used by the various command options.
        std::cout << "Check specific options:" << std::endl
                  << configFileOpt.getHelp() << std::endl
                  << std::endl;
    }


    int Check::run()
    {
        std::cout << "Loading Check app!" << std::endl;

        // Start the UI listener.
        listener.start();
        
        module.call("setVerbose", configValues.verboseMode);
        module.call("setBoardState", positionsToStateStrings(configValues.startingPosition));
        module.call("createUI", configValues.boardSizeX, configValues.boardSizeY);
        
        while (listener.isRunning())
        {
            std::cout << "Listener is Running, sleeping for 1 second!" << std::endl;
            timing::sleepSeconds(1);
            listener.rethrowExceptionIfAny();
            // Read whatever events the listener has polled so far.
            ui::EventStorage storage = queue.getAllEvents();

            for (const ui::Event& event : storage.getEvents())
            {
                // TODO - handle events here.
                std::cout << "Event received: " << event.getName() << std::endl;
            }
        }
        return constants::SUCCESS;
    }
} // namespace check
