/**
 * File: RPGFiddler.hpp
 * Author: Antonius Torode
 * Created on: 05/24/2025
 * Description: A fiddler app to mess with and test rpg library elements.
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
#include "Player.hpp"


class RPGFiddler : public MIAApplication
{
public:

    /**
     * The main constructor of the RPGFiddler class. This will construct the command options.
     */
    RPGFiddler();

    /// Default destructor.
    ~RPGFiddler() = default;
    
    /**
     * This will initialize this class by parsing command line arguments for configuration options
     * and then loading the config file if provided.
     *
     * @param argc Number of command line arguments.
     * @param argv Array of command line argument strings.
     */
    void initialize(int argc, char* argv[]) override;
    
    /// Runs the RPGFiddler application.
    int run() override;

protected:

    /**
     * Prints help info for optional config file.
     */
    void printHelp() const override;

private:

    rpg::Player player;

}; // class RPGFiddler
