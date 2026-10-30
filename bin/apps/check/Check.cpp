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
// Used for handling events.
#include "EventHandler.hpp"
// Used for the win/draw result of a game.
#include "GameResult.hpp"

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
                              configValues.startingPosition,
                              configValues.startingPlayerState);

        // Set the python module settings.
        module.call("setVerbose", configValues.verboseMode);
    }


    void Check::loadConfig()
    {
        try
        { // Load configuration here.
            configValues = constructConfig(config);

            /*
             * The verboseMode can be overridden from the config file. The desired behavior
             * here is to enable verbose mode if it is on anywhere (command line or config).
             */
            overrideVerboseMode(configValues.verboseMode || getVerboseMode());
        }
        catch (error::MIAException& ex)
        {
            std::cerr << "Error loading in configuration file value(s): " << ex.what();
            // TODO - rethrow here.
        }

        if (getVerboseMode())
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

        // Create the UI.
        module.call("createUI", configValues.boardSizeX, configValues.boardSizeY);

        // Register pieces.
        registerPieceArtInUI(module, configValues.pieceArts);

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
                {
                    case CheckUIEventType::PickupPiece:
                    { // Signals that a piece was picked up, so calculate the legal moves.
                        if (eventState.gameOver)
                        { // The board stays locked until the game is reset.
                            break;
                        }

                        eventState = processPickupPiece(module,
                                                        uiEvent,
                                                        boardState,
                                                        configValues.showLegalMoves);
                        break;
                    }

                    case CheckUIEventType::DropPiece:
                    { // Signals a piece was dropped in the ui.
                        if (eventState.gameOver)
                        { // The board stays locked until the game is reset.
                            break;
                        }

                        GameResult result = processMoveEvent(module, uiEvent, boardState, eventState);
                        handleGameResult(module, result, eventState);
                        break;
                    }

                    case CheckUIEventType::PawnPromotion:
                    { // Signals a pawn promotion ui event was received.
                        if (eventState.gameOver)
                        { // The board stays locked until the game is reset.
                            break;
                        }

                        GameResult result = processPawnPromotion(module, uiEvent, boardState);
                        handleGameResult(module, result, eventState);
                        break;
                    }

                    case CheckUIEventType::ResetBoard:
                    { // Signals the game should be reset so that it can be played again.
                        std::cout << "Resetting the game!" << std::endl;
                        processResetBoard(module, boardState, eventState);
                        break;
                    }

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
