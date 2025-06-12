/**
 * File: MIATemplate.hpp
 * Author: Antonius Torode
 * Created on: 05/24/2025
 * Description: A template program for MIA development. This app doubles as a tool to test
 *     new development and interface features as well.
 */
#pragma once

#include <string>
#include <vector>

// Used for the configuration.
#include "MIAConfig.hpp"
// Used for common app setup.
#include "MIAApplication.hpp"
// Used for command options.
#include "CommandOption.hpp"


/**
 * A template app.
 */
class MIATemplate : public MIAApplication
{
public:

    /**
     * The main constructor of the MIATemplate class. This will construct the command options.
     */
    MIATemplate();

    /// Default destructor.
    ~MIATemplate() = default;
    
    /**
     * This will initialize this class by parsing command line arguments for configuration options
     * and then loading the config file if provided.
     *
     * @param argc Number of command line arguments.
     * @param argv Array of command line argument strings.
     */
    void initialize(int argc, char* argv[]) override;
    
    /// Runs the MIATemplate application.
    int run() override;

protected:

    /**
     * Prints help info for optional config file.
     */
    void printHelp() const override;

private:

    /**
     * A Struct for storing some random test configuration values. These are the
     * values which are defined in the template config file.
     */
    struct TemplateConfig
    {
        bool boolValue{false};
        int intValue{0};
        double doubleValue{0.0};
        std::string stringValue{""};
        std::vector<std::string> listValue{};
    
        /// Used to dump the configuration files read in.
        void printConfigValues() const;
    };
    TemplateConfig configFileVals; 
    
    /// The configuration loader for this app.
    config::MIAConfig config;

    // Command options for this app.
    CommandOption configFileOpt;
    CommandOption testOpt;
    
    /// Stores whether testOpt was specified or not.
    bool testMode{false};
    
    /// The name of the defauilt config file that this class uses.
    std::string defaultConfigFile{"MIATemplate.MIA"};
    
    /// Load the configuration variables.
    void loadConfig();
}; // class MIATemplate
