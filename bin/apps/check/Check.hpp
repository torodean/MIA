/**
 * @file Check.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief The Custom House-made Engine for Chess Knowledge (CHECK) application. This is
 *        a custom chess simulator with various (continually expanding) features. It is
 *        made entirely for fun and learning.
 */
#pragma once

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

        /// The name of the defauilt config file that this class uses.
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
         * Define the components which are needed for UI event handling
         * and ui interfacing.
         *
         * The defaultModuleName is the default python ui module to use.
         * The PythonModule handles loading in the python UI.
         * The EventQueue stores events the UI produces.
         * The EventStorage stores the events retreived from the queue.
         * The PythonUIListener handles monitoring and filling the UI queue.
         */
        std::string defaultModuleName{"check"};
        PythonModule module;
        ui::EventQueue queue;
        ui::EventStorage storage;
        python_ui::PythonUIListener listener;
    }; // class Check
} // namespace check
