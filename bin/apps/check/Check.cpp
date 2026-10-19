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
#include "MIAException.hpp"
// Used for file paths.
#include "Paths.hpp"
// Used for return codes.
#include "Constants.hpp"
// Used for testing purposes.
#include "Timing.hpp"
// Used for python interface results.
#include "PythonResult.hpp"
// Used for handling events.
#include "EventHandler.hpp"

namespace check
{
    Check::Check() : 
        config(defaultConfigFile, constants::ConfigType::KEY_VALUE),
        configFileOpt("-c", "--config", "Specify a config file to use (default = " +
                                    paths::getDefaultConfigDirToUse() + defaultConfigFile,
                                    CommandOption::commandOptionType::STRING_OPTION),
        module(defaultModuleName, __FILE__),
        listener(module, queue, "getAllEvents", 10) // 10 ms UI pollrate.
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
                              
        // Pass the runtime context to the board state to enable logging and verbosity.
        boardState.setRuntimeContext(getContext());
        
        // Load config values into the board state.
        boardState.initialize(configValues.boardSizeX,
                              configValues.boardSizeY,
                              configValues.startingPosition);
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
        std::cout << "WARNING!! THIS APP IS IN DEV!" << std::endl;
        std::cout << "Loading Check app!" << std::endl;

        // Start the UI listener.
        listener.start();
        
        // Set the python module settings.
        module.call("setVerbose", configValues.verboseMode);
        
        // Create the UI.
        module.call("createUI", configValues.boardSizeX, configValues.boardSizeY);
        
        // Register pieces.
        module.call("registerPiece", "king", "\u265A");
        timing::sleepMilliseconds(20);
        module.call("registerPiece", "queen", "\u265B");
        timing::sleepMilliseconds(20);
        module.call("registerPiece", "pawn", "\u265F");
        timing::sleepMilliseconds(20);
        module.call("registerPiece", "knight", "\u265E");
        timing::sleepMilliseconds(20);
        module.call("registerPiece", "rook", "\u265C");
        timing::sleepMilliseconds(20);
        module.call("registerPiece", "bishop", "\u265D");
        timing::sleepMilliseconds(20);
        
        // Setup the starting board position.
        module.call("setBoardState", positionsToStateStrings(configValues.startingPosition));
        
        // Update the UI with the board position.
        module.call("updateUI");
        
        EventState eventState; // Stores various states between events.
        while (listener.isRunning())
        {
            // Update the UI to the most recent state.
            module.call("updateUI");

            // Read whatever events the listener has polled so far.
            storage = queue.getAllEvents(); // This calls updateUI() each time.
            for (const ui::Event& event : storage.getEvents())
            {
                std::cout << "Event received: " << event.getName() << std::endl;
                CheckUIEvent uiEvent = toCheckUIEvent(event.getName());
                switch(uiEvent.type)
                { // TODO - handle events here.
                    case CheckUIEventType::PickupPiece:
                    { // Signals that a piece was picked up, so calculate the legal moves.
                        eventState = processPickupPiece(module,
                                                        uiEvent,
                                                        boardState,
                                                        configValues.showLegalMoves);
                        break;
                    }

                    case CheckUIEventType::DropPiece:
                    {
                        processMoveEvent(module, uiEvent, boardState, eventState);
                        break;
                    }

                    case CheckUIEventType::ResetBoard:

                    case CheckUIEventType::SaveBoard:
                        std::cout << "Still coding this event type!" << std::endl;
                        break;

                    case CheckUIEventType::Stop:
                    { // Signals that the UI was exited.
                        listener.stop();
                        return constants::SUCCESS;
                    }
                    
                    case CheckUIEventType::Unknown:
                    default:
                    { // Unknown/unparsable event.
                        std::cerr << "The input event is unknown!" << std::endl;
                        return constants::FAILURE;
                    }
                }
            }
            listener.rethrowExceptionIfAny();
        }
        return constants::SUCCESS;
    }
} // namespace check
