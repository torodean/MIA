/**
 * File: MIATerminal.hpp
 * Author: Antonius Torode
 * Date: 03/01/2021
 * Description:
 */

#include "SubProgram.hpp"
#include "Configurator.hpp"
#include "Commands.hpp"
#include "encryption/D3CEncryptPW.hpp"
#include "encryption/D3CEncrypt.hpp"

using std::string;

/**
 * Main class for running the Terminal sub-program.
 */
class MIATerminal : public SubProgram
{
public:

    /// Disallow the default constructor.
    MIATerminal() = delete;

    /**
     * Main constructor for the MIATerminal sub-program.
     */
    explicit MIATerminal(string terminalName);

    /**
     * Main destructor for the MIATerminal sub-program.
     */
    ~MIATerminal() = default;

    /**
     * Main user interface for MIA.
     * @param defaultCommand[std::string&] A default command to run when
     *      running terminal interface (default = "").
     */
    void run(const string& defaultCommand = "");


    /**
     * Outputs an excuse for not being able to perform a command. Has a 20% chance to do so.
     * @return returns true if an excuse was output.
     */
    bool useExcuse();

protected:

private:

    /**
     * Takes the input command by the user and runs the corresponding feature.
     * @param input is the command to perform
     */
    void performMIACommand(std::string& input);

    Configurator config;
    Commands cmd;
    D3CEncrypt d0s1;
    D3CEncryptPW d0s2;

    /**
     * Prints the splash screen for MIA.
     */
    static void printSplash();

    /**
     * Displays an introductory greeting to the user.
     */
    static void intro();

    /**
     * The standby screen while waiting for a user input.
     * @param defaultCommand is a default command to run
     */
    void standby(string defaultCommand);

    /**
     * Informs the user of the help feature and asks for a command.
     */
    static void helpMessage();

    /**
     * Displays a list of valid commands and what they do to the user.
     */
    static void printHelp();

    /**
     * Provides information regarding the prime number functions
     */
    static void helpPrime();

    /**
     * Displays information regarding the Net functions.
     */
    static void helpNet();


    /**
     * A function used for testing
     * This should be placed at the end of the file for easy accessibility.
     */
    static void runTest();
};
