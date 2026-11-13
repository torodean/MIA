/**
 * @file Check.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief The Custom House-made Engine for Chess Knowledge (CHECK) application. This is
 *        a custom chess simulator with various (continually expanding) features. It is
 *        made entirely for fun and learning.
 */
#pragma once

#include <string>

// Used for the configuration.
#include "MIAConfig.hpp"
#include "CheckConfig.hpp"
// Used for common app setup.
#include "MIAApplication.hpp"
// Used for command options.
#include "CommandOption.hpp"

// Needed for ui interfacing and event handling.
#include "PythonModule.hpp"
#include "EventQueue.hpp"
#include "PythonUIListener.hpp"
#include "EventHandler.hpp"
#include "BoardState.hpp"

namespace check
{
    /**
     * The main application class for the Custom House-made Engine for Chess Knowledge (CHECK) app.
     * This class is intended to be used with the MIA_MAIN macro and satisfies MIA application
     * requirements. 
     */
    class Check : public MIAApplication
    {
    public:

        /**
         * The main constructor of the Check class. This will construct the command options.
         */
        Check();

        /// Default destructor.
        ~Check() = default;

        /**
         * This will initialize this class by parsing command line arguments for configuration options
         * and then loading the config file provided.
         *
         * @param argc Number of command line arguments.
         * @param argv Array of command line argument strings.
         */
        void initialize(int argc, char* argv[]) override;

        /// Runs the Check application.
        int run() override;

    protected:

        /**
         * Prints help info for optional config file.
         */
        void printHelp() const override;

    private:

        /// The name of the default config file that this class uses.
        std::string defaultConfigFile{"Check.MIA"};

        /// The configuration loader for this app.
        config::MIAConfig config;
        /// The configuration values for this application.
        CheckConfig configValues;

        // Command options for this app.
        CommandOption configFileOpt;

        /// Load the configuration variables.
        void loadConfig();

        /**
         * Sets up the board and UI for the given configuration: validates the
         * starting position, (re-)initializes the board state, and (re)creates
         * the UI with the piece art and starting position. Used to build the
         * board and UI for a configuration, including when loading a saved board
         * state that was created from a different configuration.
         *
         * @param config The configuration to set the board and UI up for.
         * @throws MIAException if the starting position is invalid.
         */
        void setupBoard(const CheckConfig& config);

        /**
         * Handles a LoadBoard event. Restores the configuration the save was made
         * from, rebuilds the board and UI for it, replays the saved moves, then
         * syncs the UI, turn indicator, and game-over state to the loaded position.
         *
         * @param uiEvent The LoadBoard event; its argument holds the save file path.
         * @param eventState The per-game event state to refresh after loading.
         */
        void handleLoadEvent(const CheckUIEvent& uiEvent, EventState& eventState);

        /**
         * Define the components which are needed for UI event handling
         * and ui interfacing.
         *
         * The defaultModuleName is the default python ui module to use.
         * The PythonModule handles loading in the python UI.
         * The EventQueue stores events the UI produces.
         * The EventStorage stores the events retrieved from the queue.
         * The PythonUIListener handles monitoring and filling the UI queue.
         */
        std::string defaultModuleName{"checkUI"};
        PythonModule module;
        ui::EventQueue queue;
        ui::EventStorage storage;
        python_ui::PythonUIListener listener;

        /// Stores the current board state.
        BoardState boardState;
    }; // class Check
} // namespace check
