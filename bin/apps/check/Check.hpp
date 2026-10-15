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
// Used for common app setup.
#include "MIAApplication.hpp"
// Used for command options.
#include "CommandOption.hpp"


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

    // Command options for this app.
    CommandOption configFileOpt;

    /// Load the configuration variables.
    void loadConfig();
}; // class Check
