/**
 * File: MIAOriginal.hpp
 * Author: Antonius Torode
 * Created on: 05/24/2025
 * Description: An app which implements many of the features from the
 *     original MIA application (before moving to an app structure).
 */
#pragma once

#include <string>

// Used for common app setup.
#include "MIAApplication.hpp"
// Used for command options.
#include "CommandOption.hpp"

#include "D3CEncrypt.hpp"
#include "D3CEncryptPW.hpp"
#include "Commands.hpp"

/**
 * A template app.
 */
class MIAOriginal : public MIAApplication
{
public:

    /**
     * The main constructor of the MIAOriginal class. This will construct the command options.
     */
    MIAOriginal();

    /// Default destructor.
    ~MIAOriginal() = default;
    
    /**
     * This will initialize this class by parsing command line arguments.
     * @param argc Number of command line arguments.
     * @param argv Array of command line argument strings.
     */
    void initialize(int argc, char* argv[]) override;
    
    /// Runs the MIAOriginal application.
    int run() override;

protected:

    /**
     * Prints help info for optional config file.
     */
    void printHelp() const override;

private:
    
    // Command options for this app.
    CommandOption defaultCommandOpt;

    /// String to store a default command to run.
    std::string defaultCommand{};
    
    /// Create an instance of various utility classes.
    Commands cmd;
    D3CEncrypt d0s1;
    D3CEncryptPW d0s2;
    
    /**
     * Prints the splash screen for MIA. Prints the original MIA splash screen.
     */
    static void printSplash();

    /**
     * Displays an introductory greeting to the user.
     */
    static void intro();

    /**
     * The standby screen while waiting for a user input.
     * The original MIA standby/wait-for-command feature.
     */
    void standby();

    /**
     * Informs the user of the help feature and asks for a command.
     */
    static void helpMessage();

    /**
     * Displays a list of valid commands and what they do to the user.
     */
    static void printMIAHelp();

    /**
     * Provides information regarding the prime number functions
     */
    static void helpPrime();

    /**
     * Displays information regarding the Net functions.
     */
    static void helpNet();
    
    /**
     * Takes the input command by the user and runs the corresponding feature.
     * @param input is the command to perform
     */
    void performMIACommand(std::string& input);
    
    /**
     * Outputs an excuse for not being able to perform a command. Has a 20% chance to do so.
     * @return returns true if an excuse was output.
     */
    bool useExcuse();

}; // class MIAOriginal
