/**
 * File: WoWFishbot.hpp
 * Author: Antonius Torode
 * Created on: 05/22/2025
 * Description: This file contains an old WoW fishbot from the original MIA code. It originally
 *     lived in the VirtualKeyCodes file(s) but has since been decoupled. 
 */
#pragma once

#include <string>

// Used for virtual key presses. 
#include "VirtualKeyStrokes.hpp"
// Used for the configuration.
#include "MIAConfig.hpp"
// Used for common app setup.
#include "MIAApplication.hpp"
// Used for command options.
#include "CommandOption.hpp"
 
/**
 * This class contains the code related to the WoWFishbot. This code was decoupled from other
 * utility code within the MIA project and may need some fiddling, changes, or testing before
 * it works accurately again.
 */
class WoWFishbot : public MIAApplication
{
public:
    /**
     * The main constructor of the WoWFishbot class. This will construct the command options.
     */
    WoWFishbot();
    
    /// Default destructor.
    ~WoWFishbot() = default;
    
    /**
     * This will initialize this class by parsing command line arguments for configuration options
     * and then loading the config file if provided.
     *
     * @param argc Number of command line arguments.
     * @param argv Array of command line argument strings.
     */
    void initialize(int argc, char* argv[]) override;
    
    /**
     * Runs the fishbot.
     * @throw [MIAException] - Throws if not implemented for the OS.
     * @return [int] - Returns 0 on success.
     */
    int run() override;

protected:

    /**
     * Prints help info for optional config file.
     */
    virtual void printHelp() const;

private:
    /**
     * A fishbot for World of Warcraft.
     * @param fishButton[std::string] - The button which will trigger the fishing cast.
     * @param lureButton[std::string] - The button which will trigger a lure use.
     * @note This method only works on Windows. Primarily because WoW only has official Windows support. 
     */
    void WoWFishBot(std::string fishButton, std::string lureButton);
    
    /// Intro text for the fishbot. This is mostly for flavor.
    void fishBotIntro();
    
    /// The configuration loader for this app.
    MIA_System::MIAConfig config;
    
    // Command options for this app.
    CommandOption configFileOpt;
    
    /// The name of the defauilt config file that this class uses.
    std::string defaultConfigFile{"WoWConfig.MIA"};
    
    /// Load the configuration variables.
    void loadConfig();
    
    /**
     * WoW WoWFishbot variables. These have default values for a specific setup.
     */
    int WoWFishBotStartX{725};
    int WoWFishBotStartY{360};
    int WoWFishBotEndX{1230};
    int WoWFishBotEndY{495};
    int WoWFishBotIncrement{40};
    int WoWFishBotNumOfCasts{10000};
    int WoWFishBotDelay{1000};
}; // class WoWFishbot
