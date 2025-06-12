/**
 * File: MIAOriginal.cpp
 * Author: Antonius Torode
 * Date: 05/24/2025
 * Description: Implementation of the MIAOriginal app.
 */

#include <iostream>
#include <utility>

// The associated header file.
#include "MIAOriginal.hpp"

// Used for error handling.
#include "MIAException.hpp"
#include "Error.hpp"
#include "StringUtils.hpp"
#include "MathUtils.hpp"
#include "FileUtils.hpp"
#include "Constants.hpp"
#include "Paths.hpp"

// Used for encryption features.
#include "D3CEncrypt.hpp"
#include "D3CEncryptPW.hpp"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    #include "windows/WindowsUtilities.hpp"
#endif


using std::string;
using std::remove;
using std::stoi;
using std::ifstream;
using std::vector;
using std::cout;
using std::endl;
using std::getline;
using std::cin;
using std::tolower;
using std::to_string;


MIAOriginal::MIAOriginal() :
    defaultCommandOpt("-c", "--command", "Run a default command when the app is ran.",
                                         CommandOption::commandOptionType::stringOption)
{ };


void MIAOriginal::initialize(int argc, char* argv[])
{
    try
    {    
        MIAApplication::initialize(argc, argv);

        // Set the values from the command line arguments.
        defaultCommandOpt.getOptionVal<string>(argc, argv, defaultCommand);
    }
    catch (const error::MIAException& ex)
    {
        std::cerr << "Error during MIAOriginal::initialize: " << ex.what() << endl;
    }
}


void MIAOriginal::printHelp() const
{
    MIAApplication::printHelp();
    
    // This is a dump of the help messages used by the various command options.
    cout << "MIAOriginal specific options:" << endl
              << defaultCommandOpt.getHelp() << endl
              << endl;
}


int MIAOriginal::run()
{
    printSplash();
    standby();
    return 0;
}



void MIAOriginal::printSplash()
{
    cout << "     ...................................................................." << endl;
    cout << "    ... //  ~~      |||      |||  ||||||||||      ||||     TM   ~~  \\\\ ..." << endl;
    cout << "   ... //  ~~       ||||    ||||      ||         ||  ||          ~~  \\\\ ..." << endl;
    cout << "  ... //  ~~        || ||  || ||      ||        ||||||||          ~~  \\\\ ..." << endl;
    cout << " ... //  ~~         ||  ||||  ||      ||       ||      ||          ~~  \\\\ ..." << endl;
    cout << "... //  ~~          ||   ||   ||  ||||||||||  ||        ||          ~~  \\\\ ..." << endl;
    cout << ".............................................................................." << endl;
    cout << "............................. TERMINAL INTERFACE ............................." << endl;
    cout << "............. Multiple Integrated Applications -- Version: " + constants::MIA_VERSION + " ............." << endl;
    cout << "....................     Programmer: Antonius Torode      ...................." << endl;
    cout << "....................      Compiled on: " << __DATE__ << "        ...................." << endl;
    cout << ".............................................................................." << endl;
    cout << "...WARNING: Some features remain untested after migrating MIA to app format..." << endl;
    cout << ".............................................................................." << endl;
    cout << "..." << endl;
    cout << "... Greetings! I am MIA.                                                   ..." << endl;
    
}


void MIAOriginal::standby()
{
    string input;

    if(!defaultCommand.empty())
    {
        cout << "... I've detected a default command entered: " << defaultCommand << endl;
        cout << endl;
        performMIACommand(defaultCommand);
        cout << endl;
    }

    helpMessage();
    while(true)
    {
        getline(cin,input);
        cout << endl;
        if(StringUtils::toLower(input) == "exit" || StringUtils::toLower(input) == "quit")
        {
            break;
        }
        else
        {
            performMIACommand(input);
        }
        helpMessage();
    }
}


void MIAOriginal::helpMessage()
{
    cout << "..." << endl;
    cout << "... You can type 'help' for a list of valid commands.                      ..." << endl;
    cout << "... Please enter a command.                                                ..." << endl;
    cout << "..." << endl;
    cout << endl;
}


bool MIAOriginal::useExcuse()
{
    int random = math::randomInt(1, 100, 0, true);
    string txt;

    if (random >= 20)
    {
        cout << endl;
        cout << "...Sorry, I can't do that (Invalid command entered). " << endl;

        string excuseFilePath = paths::getDefaultConfigDirToUse() + "/excuses.MIA";
        txt = files::getRandomLineOfFile(excuseFilePath);
        cout << "..." << txt << endl;
        return true;
    }
    return false;
}


void MIAOriginal::printMIAHelp()
{
    cout << "... A list of valid commands and a brief summary.                          ..." << endl;
    cout << "... Commands are NOT case sensitive.                                       ..." << endl;
    cout << "..." << endl;
    cout << "... help          | Displays a valid lists of commands." << endl;
    cout << "... add           | Adds two positive integers of any length. " << endl;
    cout << "... alarm -r      | Triggers a repeating alarm after the desired ms." << endl;
    cout << "... collatz       | Produces a collatz sequence based on a starting integer." << endl;
    cout << "... crypt -d0s1   | Encrypts a string using the d0s1 algorithm." << endl;
    cout << "... crypt -d0s2   | Encrypts a string using the d0s2 algorithm." << endl;
    cout << "... decrypt -d0s1 | De-crypts a string using the d0s1 algorithm." << endl;
    cout << "... decrypt -d0s2 | De-crypts a string using the d0s2 algorithm." << endl;
    cout << "... digitsum      | Returns the sum of the digits within an integer of any size." << endl;
    cout << "... error info    | Returns information regarding an error code." << endl;
    cout << "... error info -a | Returns information regarding all error codes." << endl;
    cout << "... factors       | Returns the number of factors within an integer." << endl;
    cout << "... lattice       | Returns lattice paths to the farthest corner of an NxM grid." << endl;
    cout << "... multiply      | Multiplies two integers of any length." << endl;
    cout << "... palindrome    | Determines if a positive integer is palindrome." << endl;
    cout << "... prime -help   | Displays help defaults for prime functions." << endl;
    cout << "... quadratic form| Calculates a solution to a*x^2+b*x+c=0." << endl;
    cout << "... randfromfile  | Prints a number of random lines from a text file." << endl;
    cout << "... subtract      | Finds the difference between two integers of any length." << endl;
    cout << "... triangle      | Determines if a number is a triangle number or not." << endl;
    cout << "... exit          | Quits MIA. " << endl;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    cout << "... button spam   | Spams a specified button (key press)." << endl;
    cout << "... button spam -t| Spams a specified button (key press) separated by tabs." << endl;
    cout << "... eyedropper    | Returns the RGB value of the pixel located at the cursor." << endl;
    cout << "... find mouse    | Finds/Returns the coordinates of the mouse after 5 seconds." << endl;
    cout << "... net -help     | Displays net functions available within MIA." << endl;
#endif
}


void MIAOriginal::helpPrime()
{
    cout << "... A list of various valid commands and a brief summary.                  ..." << endl;
    cout << "... Commands are NOT case sensitive.                                       ..." << endl;
    cout << "..." << endl;
    cout << "... prime         | Determines if a positive integer is prime or not." << endl;
    cout << "... prime -f      | Determines all of the prime factors of a positive integer." << endl;
    cout << "... prime -n      | Calculates the n'th prime number up to a max of 2147483647." << endl;
    cout << "... prime -n -p   | Creates a file of prime #'s up to a max of 2147483647." << endl;
    cout << "... prime -n -c   | Clears the file created by 'prime -n -p'." << endl;
}


void MIAOriginal::helpNet()
{
    cout << "... A list of various valid commands and a brief summary.                  ..." << endl;
    cout << "... Commands are NOT case sensitive.                                       ..." << endl;
    cout << "..." << endl;
    cout << "... net session   | Provides info about sessions established on a server." << endl;
    cout << "... net server -s | Lists all servers that are visible in a domain." << endl;
    cout << "... net server -w | Lists all workstations that are visible in a domain." << endl;
    //cout << "... net wkst      | Returns info about the configuration of a workstation." << endl;
    //cout << "... net remote    | Retrieves the optional features a remote system supports." << endl;
    //cout << "... net user      | Retrieves information about all user accounts on a server." << endl;
    //cout << "... net user -i   | Retrieves information about a user account on a server." << endl;
    //cout << "... net modals    | Retrieves global information for all users and global groups." << endl;
    //cout << "... net display   | Returns computer, or group account information." << endl;
}


void MIAOriginal::performMIACommand(string& input)
{
    string cmdInput = StringUtils::toLower(input);
    Commands::MIAInput in = Commands::commandToInputEnum(cmdInput);

    switch( in ){
        case Commands::MIAInput::HELP:
            printMIAHelp();
            break;
        case Commands::MIAInput::CRYPT_D0S1:
            d0s1::d0s1CryptRunner();
            break;
        case Commands::MIAInput::DECRYPT_D0S1:
            d0s1::d0s1DeCryptRunner();
            break;
        case Commands::MIAInput::CRYPT_D0S2:
            d0s2::d0s2CryptRunner();
            break;
        case Commands::MIAInput::DECRYPT_D0S2:
            d0s2::d0s2DeCryptRunner();
            break;
        case Commands::MIAInput::ENTANGLETEXT:
            Commands::entangleTextRunner();
            break;
        case Commands::MIAInput::COLLATZ:
            Commands::collatzRunner();
            break;
        case Commands::MIAInput::ADD:
            Commands::stringAdditionRunner();
            break;
        case Commands::MIAInput::MULTIPLY:
            Commands::stringMultiplyRunner();
            break;
        case Commands::MIAInput::SUBTRACT:
            Commands::stringSubtractionRunner();
            break;
        case Commands::MIAInput::PRIME:
            Commands::isPrimeRunner();
            break;
        case Commands::MIAInput::PALINDROME:
            Commands::isPalindromeRunner();
            break;
        case Commands::MIAInput::DIGITSUM:
            Commands::sumOfDigitsRunner();
            break;
        case Commands::MIAInput::PRIME_F:
            Commands::primeFactorsRunner();
            break;
        case Commands::MIAInput::FACTORS:
            Commands::numberOfFactorsRunner();
            break;
        case Commands::MIAInput::TRIANGLE:
            Commands::isTriangleNumberRunner();
            break;
        case Commands::MIAInput::LATTICE:
            Commands::latticePathsRunner();
            break;
        case Commands::MIAInput::PRIME_N:
            Commands::primeNRunner();
            break;
        case Commands::MIAInput::PRIME_N_P:
            Commands::primeNumberNpopulateRunner();
            break;
        case Commands::MIAInput::PRIME_N_C:
            Commands::primeNumberNeraseRunner();
            break;
        case Commands::MIAInput::BUTTONSPAM:
            Commands::buttonSpamRunner(false);
            break;
        case Commands::MIAInput::PRIME_HELP:
            helpPrime();
            break;
        case Commands::MIAInput::CRYPT_D0S3:
            cmd.d0s3CryptRunner();
            break;
        case Commands::MIAInput::DECRYPT_D0S3:
            cmd.d0s3DeCryptRunner();
            break;
        case Commands::MIAInput::BUTTONSPAM_T:
            Commands::buttonSpamRunner(true);
            break;
        case Commands::MIAInput::RANDFROMFILE:
            Commands::printRandomLinesFromFileRunner();
            break;
        case Commands::MIAInput::QUADRATICFORM:
            Commands::solveQuadraticFormulaRunner();
            break;
        case Commands::MIAInput::PRANJAL:
            Commands::pranjal();
            break;
        case Commands::MIAInput::FINDMOUSE:
            Commands::findMouse();
            break;
        case Commands::MIAInput::EYEDROPPER:
            Commands::eyedropper();
            break;
        case Commands::MIAInput::SPLASH:
            printSplash();
            break;
        case Commands::MIAInput::NETHELP:
            helpNet();
            break;
        case Commands::MIAInput::NETSESSION:
            cmd.runNetSessionEnum();
            break;
        case Commands::MIAInput::NETSERVER_W:
            cmd.runNetServerEnum('w');
            break;
        case Commands::MIAInput::NETSERVER_S:
            cmd.runNetServerEnum('s');
            break;
        case Commands::MIAInput::DATE:
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
            WindowsUtilities::terminalCommand("DATE");
#else
            throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
            break;
        case Commands::MIAInput::DICEROLL:
            Commands::rollDice(input);
            break;
        case Commands::MIAInput::ALARM_R:
            Commands::runRepeatingAlarm();
            break;
        case Commands::MIAInput::ERRORINFO:
            error::errorInfoRun(false);
            break;
        case Commands::MIAInput::ERRORINFO_A:
            error::errorInfoRun(true);
            break;


        case Commands::MIAInput::EXCUSE:
        default: //defaults to an unrecognized command.
            if(!useExcuse())
                cout << "... Invalid Command Entered.                                               ..." << endl;
            break;
    }
}
