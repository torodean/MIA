/**
 * @file Commands.hpp
 * @author Antonius Torode
 * @date 03/01/2021
 * Description: This file contains the Commands class, which defines the 
 *     available terminal commands and their runner commands. This file
 *     was copied from the original MIA code with minimal changes. A few
 *     commands were removed (those moved to standalone apps) and simple
 *     refactoring was done to allow the code to function with the new setup.
 *
 *     Do NOT use this code as an example of what to-do, as it is quite old.
 */
#pragma once

#include <string>

class Commands
{
public:
	/// Main constructor for the Commands class.
    Commands() = default;

	// Main destructor for the Commands class.
    ~Commands() = default;

	// Valid MIA commands.
    enum MIAInput
    {
        ADD,
        ALARM_R,
        BUTTONSPAM,
        BUTTONSPAM_T,
        COLLATZ,
        CONFIG,
        CRYPT_D0S1,
        CRYPT_D0S2,
        CRYPT_D0S3,
        DATE,
        DECRYPT_D0S1,
        DECRYPT_D0S2,
        DECRYPT_D0S3,
        DICEROLL,
        DIGITSUM,
        ENTANGLETEXT,
        ERRORINFO,
        ERRORINFO_A,
        EYEDROPPER,
        EXCUSE,
        FACTORS,
        FINDMOUSE,
        HELP,
        LATTICE,
        MULTIPLY,
        NETHELP,
        NETSESSION,
        NETSERVER_W,
        NETSERVER_S,
        PALINDROME,
        PRIME,
        PRIME_F,
        PRIME_N,
        PRIME_N_P,
        PRIME_N_C,
        PRIME_HELP,
        PRANJAL,
        QUADRATICFORM,
        RANDFROMFILE,
        SPLASH,
        SUBTRACT,
        TRIANGLE,
        TEST,
    };

    /**
     * A function used for testing
     * This should be placed at the end of the cpp file for easy accessibility.
     */
    static void runTest();

    /**
     * Takes the user input as a string and converts it to a corresponding
     * value of Program::MIAInput enum to be used in the switch case.
     * Replaced Program::commandToInputVar(string input) in versin 0.145.
     * @param input - the string input of a command.
     * @return - the enum object correlating to the command text.
     */
    static MIAInput commandToInputEnum(std::string &input);

    /**
     * Main UI runner for the D3CMath productCollatzSequence() function.
     */
    static void collatzRunner();

    /**
     * Main UI runner for the D3CMath addTwoStrings() function.
     */
    static void stringAdditionRunner();

    /**
     * Main UI runner for the D3CMath multiplyTwoStrings() function.
     */
    static void stringMultiplyRunner();

    /**
     * Main UI runner for the D3CMath subtractTwoStrings() function.
     */
    static void stringSubtractionRunner();

    /**
     * Main UI runner for the D3CMath isPrime() function.
     */
    static void isPrimeRunner();

    /**
     * Main UI runner for the D3CMath isPalindrome() function.
     */
    static void isPalindromeRunner();

    /**
     * Main UI runner for the D3CMath sumOfDigits() function.
     */
    static void sumOfDigitsRunner();

    /**
     * Main UI runner for the D3CMath primeFactors() function.
     */
    static void primeFactorsRunner();

    /**
     * Main UI runner for the D3CMath numberOfFactors() function.
     */
    static void numberOfFactorsRunner();

    /**
     * Main UI runner for the D3CMath isTriangleNumber() function.
     */
    static void isTriangleNumberRunner();

    /**
     * Main UI runner for the D3CMath latticePaths() function.
     */
    static void latticePathsRunner();

    /**
     * Main UI runner for the D3CMath primeN() function.
     */
    static void primeNRunner();

    /**
     * Main UI runner for the D3CMath primeNPopulate() function.
     */
    static void primeNumberNpopulateRunner();

    /**
     * Main UI runner for the D3CMath primeNerase() function.
     */
    static void primeNumberNeraseRunner();

    /**
     * Spams a button a specific number of times.
     * @param enableTab[bool] - option to tab between each button press.
     */
    static void buttonSpamRunner(bool enableTab);

    /**
     * Runs d0s3 encryption
     */
    void d0s3CryptRunner();

    /**
     * Runs d0s3 decryption.
     */
    void d0s3DeCryptRunner();

    /**
     * Prints a number of random lines from a text file. The user specifies the file and number of lines.
     */
    static void printRandomLinesFromFileRunner();

    /**
     * Runner for the solveQuadraticFormula function.
     */
    static void solveQuadraticFormulaRunner();

    /**
     * A joke function I created to rearrange the letters in a friends name. For unspecified reasons. Never deprecate.
     */
    static void pranjal();

    /**
     * Finds and returns the coordinates of the mouse pointer after 5 seconds.
     */
    static void findMouse();

    /**
     * Runs the eyedropper command which determines the color of the pixel at the mouse location.
     */
    static void eyedropper();

    /**
     * Runs a fishbot for World of Warcraft.
     */
    static void runFishbot();

    /**
     * Runs the NetSessionEnumRunner from the LOLANetUse.cpp file.
     */
    void runNetSessionEnum();

    /**
     * Runs the NetServerEnumRunner_WKST from the LOLANetUse.cpp file.
     * @param mode[char] - Valid modes are 'w' for workstation or 's' for server.
     */
    void runNetServerEnum(char mode);

    /**
     * Runs the NetWkstaGetInfoRunner from the LOLANetUse.cpp file.
     */
    void runNetWkstaGetInfo();

    /**
     * Runs the NetRemoteComputerSupportsRunner from the LOLANetUse.cpp file.
     */
    void runNetRemoteComputerSupports();

    /**
     * Runs the NetUserEnumRunner from the LOLANetUse.cpp file
     */
    void runNetUserEnum();

    /**
     * Rolls a dice using commands programmed in misc class.
     * @param input[const std::string&] - String reference to the input dice roll.
     * @return [int] - Returns the total dice roll value.
     */
    static int rollDice(const std::string& input);

	/**
	 * A method to repeat an alarm sound on a regular cadence.
	 */
    [[noreturn]] static void runRepeatingAlarm();

    static void entangleTextRunner();

};
