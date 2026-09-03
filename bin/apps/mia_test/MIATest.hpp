/**
 * @file MIATest.hpp
 * @author Antonius Torode
 * @date 05/24/2025
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
 * A test app. 
 * This app is just a place to test various things that are being developed.
 */
class MIATest : public MIAApplication
{
public:

    /**
     * The main constructor of the MIATest class. This will construct the command options.
     */
    MIATest();

    /// Default destructor.
    ~MIATest() = default;
    
    /**
     * This will initialize this class by parsing command line arguments for configuration options
     * and then loading the config file if provided.
     *
     * @param argc Number of command line arguments.
     * @param argv Array of command line argument strings.
     */
    void initialize(int argc, char* argv[]) override;
    
    /// Runs the MIATest application.
    int run() override;    

}; // class MIATest
