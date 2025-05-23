/**
 * File: Commands.cpp
 * Author: Antonius Torode
 * Creation Date: 03/01/2021
 * Description:
 */

#include <iostream>
#include "Commands.hpp"
#include "StringUtils.hpp"
#include "TerminalTools.hpp"
#include "D3CMath.hpp"
#include "Error.hpp"
#include "FileUtils.hpp"
#include "MathUtils.hpp"
#include "MIASequencer.hpp"
#include "MIAWorkout.hpp"
#include "audio/SystemSounds.hpp"
#include "SystemUtils.hpp"

#ifdef USE_Database_LIB
#include "../libs/database/MIADatabase.hpp"
#endif


// If windows, we want the LOLA application tools to be loaded and the windows features.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#include "windows/LOLANetUse.hpp"
#include <Windows.h>
#else
#include <unistd.h>
#endif


using std::cout;
using std::endl;
using std::string;


Commands::MIAInput Commands::commandToInputEnum(string& input)
{
	// First, set the output to an excuse in case the valid one is not found.
	// Doubles ans a MIAInput initialization.
    Commands::MIAInput output = MIAInput::EXCUSE;

    if(input == "help")
        output = MIAInput::HELP;
    else if (input == "crypt -d0s1")
        output = MIAInput::CRYPT_D0S1;
    else if (input == "decrypt -d0s1")
        output = MIAInput::DECRYPT_D0S1;
    else if (input == "crypt -d0s2")
        output = MIAInput::CRYPT_D0S2;
    else if (input == "decrypt -d0s2")
        output = MIAInput::DECRYPT_D0S2;
    else if (input == "collatz")
        output = MIAInput::COLLATZ;
    else if (input == "entangle text")
        output = MIAInput::ENTANGLETEXT;
    else if (input == "add")
        output = MIAInput::ADD;
    else if (input == "alarm -r")
        output = MIAInput::ALARM_R;
    else if (input == "multiply")
        output = MIAInput::MULTIPLY;
    else if (input == "subtract")
        output = MIAInput::SUBTRACT;
    else if (input == "prime")
        output = MIAInput::PRIME;
    else if (input == "palindrome")
        output = MIAInput::PALINDROME;
    else if (input == "digitsum")
        output = MIAInput::DIGITSUM;
    else if (input == "prime -f")
        output = MIAInput::PRIME_F;
    else if (input == "factors")
        output = MIAInput::FACTORS;
    else if (input == "triangle")
        output = MIAInput::TRIANGLE;
    else if (input == "lattice")
        output = MIAInput::LATTICE;
    else if (input == "prime -n")
        output = MIAInput::PRIME_N;
	//@TODO Code in automation for parameter order swapping.
    else if (input == "prime -n -p" || input == "prime -p -n") 
        output = MIAInput::PRIME_N_P;
    else if (input == "prime -n -c" || input == "prime -c -n")
        output = MIAInput::PRIME_N_C;
    else if (input == "mc dig")
        output = MIAInput::MCDIG;
    else if (input == "button spam")
        output = MIAInput::BUTTONSPAM;
    else if (input == "button spam -t")
        output = MIAInput::BUTTONSPAM_T;
    else if (input == "mc explore")
        output = MIAInput::MCEXPLORE;
    else if (input == "prime -help" || input == "prime --help")
        output = MIAInput::PRIME_HELP;
    else if (input == "crypt -d0s3")
        output = MIAInput::CRYPT_D0S3;
    else if (input == "decrypt -d0s3")
        output = MIAInput::DECRYPT_D0S3;
    else if (input == "randfromfile")
        output = MIAInput::RANDFROMFILE;
    else if (input == "wow dup letter")
        output = MIAInput::WOWDUPLETTER;
    else if (input == "quadratic form")
        output = MIAInput::QUADRATICFORM;
    else if (input == "pranjal")
        output = MIAInput::PRANJAL;
    else if (input == "find mouse")
        output = MIAInput::FINDMOUSE;
    else if (input == "eyedropper")
        output = MIAInput::EYEDROPPER;
    else if (input == "config")
        output = MIAInput::CONFIG;
    else if (input == "fishbot")
        output = MIAInput::FISHBOT;
    else if (input == "workout")
        output = MIAInput::WORKOUT;
    else if (input == "splash")
        output = MIAInput::SPLASH;
    else if (input == "workout -w")
        output = MIAInput::WORKOUT_W;
    else if (input == "net -help")
		output = MIAInput::NETHELP;
    else if (input == "net session")
        output = MIAInput::NETSESSION;
    else if (input == "net server -w")
        output = MIAInput::NETSERVER_W;
    else if (input == "net server -s")
        output = MIAInput::NETSERVER_S;
    else if (input == "wow unload")
        output = MIAInput::WOWUNLOAD;
    else if (input == "date")
        output = MIAInput::DATE;
    else if (input == "sequencer")
        output = MIAInput::SEQUENCER;
    else if (input == "sequencer -l")
        output = MIAInput::SEQUENCER_L;
    else if (StringUtils::inputRoll(input))
        output = MIAInput::DICEROLL;
    else if (input == "error info")
        output = MIAInput::ERRORINFO;
    else if (input == "error info -a")
        output = MIAInput::ERRORINFO_A;
    else if (input == "test")
        output = MIAInput::TEST;
    return output;
}


void Commands::collatzRunner()
{
    D3CMath math;
    string input, output;

    cout << "...Enter a starting number (positive integer): " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin,input);
    MIATerminalTools::blankLine();

    long longInput = atol(input.c_str());

    output = math.productCollatzSequence(longInput);

    cout << "...collatz: " << output << endl;
    MIATerminalTools::blankLine();
    cout << "...Collatz sequence complete." << endl;
    MIATerminalTools::blankLine();
}


void Commands::stringAdditionRunner()
{
    D3CMath math;
    string intOne, intTwo, output;

    cout << "...Addition of two integers of any size." << endl << "...Enter first integer to add: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, intOne);
    MIATerminalTools::blankLine();

    cout << "...Enter second integer to add: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, intTwo);
    MIATerminalTools::blankLine();

    output = math.addTwoStrings(intOne, intTwo);

    cout << "...Integer sum: " << output << endl;
    MIATerminalTools::blankLine();
}


void Commands::stringMultiplyRunner()
{
    D3CMath math;
    string intOne, intTwo, output;

    cout << "...Multiplication of two integers of any size." << endl << "...Enter first integer to multiply: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, intOne);
    MIATerminalTools::blankLine();

    cout << "...Enter second integer to multiply: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, intTwo);
    MIATerminalTools::blankLine();

    output = math.multiplyTwoStrings(intOne, intTwo);

    cout << "...Integer product: " << output << endl;
    MIATerminalTools::blankLine();
}


void Commands::stringSubtractionRunner()
{
    D3CMath math;
    string intOne, intTwo, output;

    cout << "...Subtraction of two integers of any size." << endl << "...Enter first integer: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, intOne);
    MIATerminalTools::blankLine();

    cout << "...Enter second integer to subtract: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, intTwo);
    MIATerminalTools::blankLine();

    output = math.subtractTwoStrings(intOne, intTwo);

    cout << "...Integer difference: " << output << endl;
    MIATerminalTools::blankLine();
}


void Commands::isPrimeRunner()
{
    D3CMath math;
    string input;

    cout << "...Enter a positive Integer: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, input);
    MIATerminalTools::blankLine();

    long longInput = atol(input.c_str());

    if(math.isPrime(longInput))
    {
        cout << "...The number you entered is prime. " << endl;
    }else{
        cout << "...The number you entered is NOT prime. " << endl;
    }
    MIATerminalTools::blankLine();
}


void Commands::isPalindromeRunner()
{
    D3CMath math;
    string input;

    cout << "...Enter a positive Integer: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, input);
    MIATerminalTools::blankLine();

    long longInput = atol(input.c_str());

    if(math.isPalindrome(longInput))
    {
        cout << "...The number you entered is a palindrome. " << endl;
    }else{
        cout << "...The number you entered is NOT a palindrome. " << endl;
    }
    MIATerminalTools::blankLine();
}


void Commands::sumOfDigitsRunner()
{
    D3CMath math;
    string input;

    cout << "...Enter a positive Integer of any length: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, input);
    MIATerminalTools::blankLine();

    long output = math.sumOfDigits(input);

    cout << "...Sum of Digits: " << output << endl;
    MIATerminalTools::blankLine();
}


void Commands::primeFactorsRunner()
{
    D3CMath math;
    string input;

    cout << "...Enter a positive Integer: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, input);
    MIATerminalTools::blankLine();

    long longInput = atol(input.c_str());

    cout << "...The prime factors are: ";
    for (long i = 1; i <= (longInput + 1) / 2; i++) {
        if (math.isPrime(i) && longInput % i == 0)
        {
            cout << i << " ";
        }
    }
    MIATerminalTools::blankLine();
}


void Commands::numberOfFactorsRunner()
{
    D3CMath math;
    string input;

    cout << "...Enter a positive Integer: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, input);
    MIATerminalTools::blankLine();

    long longInput = atol(input.c_str());

    int factors = math.numberOfFactors(longInput);

    cout << "...The number of factors are: " << factors << endl;
    MIATerminalTools::blankLine();
}


void Commands::isTriangleNumberRunner()
{
    D3CMath math;
    string input;

    cout << "...Enter a positive Integer: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, input);
    MIATerminalTools::blankLine();

    long longInput = atol(input.c_str());

    if(math.isTriangleNumber(longInput))
    {
        cout << "...The number you entered is a triangle number." << endl;
    }else{
        cout << "...The number you entered is NOT a triangle number." << endl;
    }
    MIATerminalTools::blankLine();
}


void Commands::latticePathsRunner()
{
    D3CMath math;
    string n, m;

    MIATerminalTools::blankLine();
    cout << "...IMPORTANT: This feature depends on the c++ long. " << endl;
    cout << "...Therefore if the lattice paths exceed 2147483647,  " << endl;
    cout << "...the answers will no longer be reliable. " << endl;
    MIATerminalTools::blankLine();

    cout << "...Enter grid size in horizontal direction: : " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, n);
    MIATerminalTools::blankLine();
    cout << "...Enter grid size in vertical direction: : " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, m);
    MIATerminalTools::blankLine();

    long intOne = atol(n.c_str());
    long intTwo = atol(m.c_str());
    long output = math.latticePathsOfSquare(intOne, intTwo);

    cout << "...The total lattice paths are: " << output << endl;
    MIATerminalTools::blankLine();
}


void Commands::primeNRunner()
{
    D3CMath math;
    string input;

    cout << "...Enter a positive integer: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, input);
    MIATerminalTools::blankLine();

    long longInput = atol(input.c_str());
    long output = math.primeNumberN(longInput);

    cout << "...Prime number " << input << ": " << output << endl;
    MIATerminalTools::blankLine();
}


void Commands::primeNumberNpopulateRunner()
{
    D3CMath math;

    MIATerminalTools::blankLine();
    cout << "...PrimeNumberList.txt now being written to: " << math.returnPrimeNlocation() << endl;
    cout << "...This file will increase the efficiency of the 'prime -n' command." << endl;
    cout << "...This may take some time..." << endl;
    MIATerminalTools::blankLine();

    math.primeNumberNpopulate();
}


void Commands::primeNumberNeraseRunner()
{
    D3CMath math;
    math.primeNumberNerase();
}


void Commands::buttonSpamRunner(bool enableTab)
{
    VirtualKeyStrokes key;
	
	string button;

	cout << "...Enter a button sequence you would like spammed: ";
	getline(std::cin, button);
	MIATerminalTools::blankLine();
	
	string amountStr;

	cout << "...How many times would you like it spammed? ";
	getline(std::cin, amountStr);
	MIATerminalTools::blankLine();
	
	int pause;
	
	cout << "...How long between each key press (in ms)? ";
	std::cin >> pause;
	MIATerminalTools::blankLine();
	
	int amount = atoi(amountStr.c_str());
	
	cout << "...Preparing to spam " << button << " " << amountStr << " times." << endl;
	cout << "...Beginning in 5 seconds." << endl;
	MIATerminalTools::blankLine();

	if (enableTab)
		key.buttonSpamTab(button, amount, pause);
	else
		key.buttonSpam(button, amount, pause);
}


void Commands::minecraftDigRunner()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes key;
	
	string input;

	cout << "...How long would you like to dig for (in seconds)? ";
	getline(std::cin, input);
	MIATerminalTools::blankLine();
	
	int time = atoi(input.c_str());
	
	cout << "...Beginning in 5 seconds." << endl;
	key.minecraftDig(time);
#else
    Error::returnError(31416);
#endif
}


void Commands::exploreMinecraft()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes key;
	
	int stepSize = 100, delayStartTime = 5000;
	double percentComplete = 0.0,  counter = 0.0;
	int breakTime;
	
	int startx, startz, stopx, stopz;
	
	cout << "...Please enter the lower numbers as the starting values." << endl;
	MIATerminalTools::blankLine();
	
	cout << "...Enter a starting X coordinate (integer): ";
	std::cin >> startx;
	MIATerminalTools::blankLine();
	
	cout << "...Enter an ending X coordinate (integer): ";
	std::cin >> stopx;
	MIATerminalTools::blankLine();
	
	cout << "...Enter a starting Z coordinate (integer): ";
	std::cin >> startz;
	MIATerminalTools::blankLine();
	
	cout << "...Enter an ending Z coordinate (integer): ";
	std::cin >> stopz;
	MIATerminalTools::blankLine();
	
	cout << "...Enter the intermidiate time between steps in milliseconds (integer): ";
	std::cin >> breakTime;
	MIATerminalTools::blankLine();
	
	double totalSteps = ((stopx-startx)*1.0/(stepSize*1.0)+1)*((stopz-startz)*1.0/(stepSize*1.0)+1);
	cout << "...totalSteps: " << totalSteps << endl;
	
	cout << "...Beginning in " << delayStartTime << " milliseconds." << endl;
	MIATerminalTools::blankLine();
	
	Sleep(delayStartTime);
	
	cout << "...Sleeping between each loop for " << breakTime << " milliseconds." << endl;

	
	for(int x=startx; x <= stopx; x++)
	{
		for(int z=startz; z <= stopz; z++)
		{
			
			key.slash();
			key.type("tp d0sag3 ");
			
			key.type(std::to_string(x));
			key.space();
			
			key.type("100");
			key.space();
			
			key.type(std::to_string(z));

			key.enter();
			
			counter += 1.0;
			percentComplete = counter*100.0/totalSteps;
			cout << "...Percentage Complete: " << percentComplete << " %" << endl;
			Sleep(breakTime);
			
			z+=stepSize-1;
		}		
		x+=stepSize-1;
	}
#else
    Error::returnError(31416);
#endif
}


void Commands::duplicateLetterRunner()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes keys;
	
	cout << "...INSTRUCTIONS: Please have the contents of the letter copied to your clipboard." << endl;
	cout << "...INSTRUCTIONS: After entering final parameters, move mouse curser to the WoW 'send' button." << endl;

	MIATerminalTools::blankLine();
	
	int copies;
	cout << "...Please enter the number of letters you want duplicated: ";
	std::cin >> copies;
	std::cin.ignore();
	MIATerminalTools::blankLine();
	
	string recipient;
	cout << "...Please enter the recipient you want the letters sent to: ";
	getline(std::cin, recipient);
	MIATerminalTools::blankLine();
	
	cout << "...Beginning in 5000 milliseconds (5s)." << endl;
	MIATerminalTools::blankLine();
	keys.duplicateLetter(copies, recipient);
	
	cout << "...Finished duplicating letters." << endl;
#else
    Error::returnError(31416);
#endif
}


void Commands::unloadLetterRunner()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes keys;
	
	cout << "...INSTRUCTIONS: Please have your WoW inbox open." << endl;
	cout << "...INSTRUCTIONS: Please set MIAConfig variables appropriately (see manual)." << endl;

	MIATerminalTools::blankLine();
	
	int copies;
	cout << "...Please enter the number of letters you want unloaded: ";
	std::cin >> copies;
	std::cin.ignore();
	MIATerminalTools::blankLine();
	
	cout << "...Beginning in 5000 milliseconds (5s)." << endl;
	MIATerminalTools::blankLine();
	keys.unloadLetters(copies);
	
	cout << "...Finished unloading letters." << endl;
#else
    Error::returnError(31416);
#endif
}


void Commands::d0s3CryptRunner()
{
    /*
    MIAEncrypt crypt(3);
    string inputFile;
    */
    Error::returnError(31415); // Still in dev.

    /*
    cout << "...Please enter a file (using default file path) to be encrypted: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, inputFile);

    crypt.encryptFile(inputFile, "OutputFile");
     */
}


void Commands::d0s3DeCryptRunner()
{
    Error::returnError(31415);
}


void Commands::printRandomLinesFromFileRunner()
{
    int lines;
    cout << "...How many random lines would you like to print from a file? ";
    std::cin >> lines;
    std::cin.ignore();
    MIATerminalTools::blankLine();

    string answer;
    cout << "...Would you like to use default input file path? (y/n): ";
    getline(std::cin, answer);
    MIATerminalTools::blankLine();

    if(StringUtils::formOfYes(answer))
    {
        FileUtils::printRandomLinesFromFile(true, lines);
    } else {
        FileUtils::printRandomLinesFromFile(false, lines);
    }
}


void Commands::solveQuadraticFormulaRunner()
{
    D3CMath math;

    double a,b,c;
    cout << "...This is a solver for equations of the form a*x^2+b*x+c=0." << endl;
    cout << "...Please Enter a: ";
    std::cin >> a;
    cout << "...Please Enter b: ";
    std::cin >> b;
    cout << "...Please Enter c: ";
    std::cin >> c;
    std::cin.ignore();
    MIATerminalTools::blankLine();

    string ans = D3CMath::solveQuadraticFormula(a,b,c);
    MIATerminalTools::blankLine();
    cout << "...The solution is: " << ans << endl;
}


void Commands::pranjal()
{
    string output = StringUtils::shuffleString("pranjal");
    cout << "...Did you mean " << output << "?" << endl;
}

void Commands::findMouse()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes keys;
	cout << "...Locating mouse in 2 seconds." << endl;
	keys.findMouseCoords(2);
#else
    Error::returnError(31416);
#endif
}


void Commands::workoutRunner(bool weekly)
{
    string input;
    cout << "...Loading MIA workout generator. " << endl;
    MIATerminalTools::blankDots();
    cout << "...Assuming default values, difficulties range from 0-100." << endl;
    cout << "...1-10 (VERY EASY), 11-24 (EASY), 25-39 (NORMAL)" << endl;
    cout << "...39-54 (HARD), 54-74 (VERY HARD), 75+ (INSANE)" << endl;
    cout << "...Please enter a difficulty: ";
    getline(std::cin,input);

    transform(input.begin(), input.end(), input.begin(), ::tolower);
    double difficulty;

    //The pranjal input for a difficulty is a joke reference to a friend.
    if(input == "pranjal")
    {
        difficulty = 0.0;
    } else {
        difficulty = stod(input);
    }
    MIAWorkout workout;
    workout.generateWorkout(difficulty, weekly);
}


void Commands::eyedropper()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes keys;
	keys.getPixelColorAtMouse();
#else
    Error::returnError(31416);
#endif
}


void Commands::runFishbot()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes keys;
	
	//Default values.
	string fishButton = "3", lureButton = "8";
	
	cout << "...CAUTION! This fishbot was made for educational purposes." << endl;
	cout << "...WARNING! Use the fishbot at your own risk!" << endl;
	cout << "...DANGER! Using this fishbot may have negative consequences." << endl;
	cout << "...ALERT! This fishbot may get you banned." << endl;
	MIATerminalTools::blankDots();
	cout << "...In order for the fishbot to work, please enter in game settings and DISABLE" << endl << "... hardware cursor. See the manual for more details." << endl;
	cout << "...To use default values (3 for cast and 8 for lure) leave the following options" << endl << "... blank." << endl;
	cout << "...Press CTRL-C to stop the fishbot early once started." << endl;
	cout << "...Press ENTER to continue." << endl;
	std::cin.ignore();

	MIATerminalTools::blankDots();
	cout << "...Please enter which button you have set to cast: ";
	getline(std::cin, fishButton);
	MIATerminalTools::blankLine();
	cout << "...If you are not using a lure please enter 'NONE' " << endl;
	cout << "...Please enter which button you have set to apply a lure: ";
	getline(std::cin, lureButton);
	MIATerminalTools::blankLine();
	
	//Arbitrary default values based on my preferred setup.
	if(fishButton.empty() || fishButton == "\n" || fishButton == "\r")
	{
		fishButton = "3";
	}
	if(lureButton.empty() || lureButton == "\n" || lureButton == "\r")
	{
		lureButton = "8";
	}
	
	keys.WoWFishBot(fishButton, lureButton);
#else
    Error::returnError(31416);
#endif
}


void Commands::runNetSessionEnum()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    Error::returnError(31415);
	LOLANetUse lola;
	string server;
	int argc = 4;  //set to 4 indicating that all fields [0] -> [3] of argv are filled.
	
	cout << "...Please enter a server address for me to gather information about." << endl;
	MIATerminalTools::blankLine();
	getline(std::cin, server);
	MIATerminalTools::blankLine();
	
	const size_t len = server.length() + 1;
	if(Configurator::getVerboseMode())
		cout << "len: " << len << endl;
    wchar_t w_server[len];

    swprintf(w_server, len, L"%s", server.c_str());
	
	cout << "...Loading NetSessionEnumRunner." << endl;
	
	wchar_t *argv[] = {nullptr, w_server, nullptr, nullptr};
	lola.NetSessionEnumRunner(argc, argv);
	
	MIATerminalTools::blankDots();
	cout << "...NetSessionEnumRunner Finished." << endl;
#else
    Error::returnError(31416);
#endif
}


void Commands::runNetServerEnum(char mode)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    Error::returnError(31415);
	LOLANetUse lola;
	string domain;
	int argc = 2;  //set to 2 indicating that all fields [0] -> [1] of argv are filled.
	
	cout << "...Please enter a domain for me to gather information about." << endl;
	MIATerminalTools::blankLine();
	getline(std::cin, domain);
	MIATerminalTools::blankLine();
	
	const size_t len = domain.length() + 1;
    wchar_t w_domain[len];

    swprintf(w_domain, len, L"...%s", domain.c_str());

	wchar_t *argv[] = {nullptr, w_domain};
	cout << "...Loading NetServerEnum." << endl;
	
	if(mode == 'w')
	{
		lola.NetServerEnumRunner_WKST(argc, argv);
	} 
	else if(mode == 's')
	{
		lola.NetServerEnumRunner_SERV(argc, argv);
	}
	
	MIATerminalTools::blankDots();
	cout << "...NetServerEnum Finished!." << endl;
#else
    Error::returnError(31416);
#endif
}


void Commands::runNetWkstaGetInfo()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    Error::returnError(31415);
	LOLANetUse lola;
	string wkst;
	int argc = 2;  //set to 4 indicating that all fields [0] -> [1] of argv are filled.
	
	cout << "...Please enter a workstation address for me to gather information about." << endl;
	MIATerminalTools::blankLine();
	getline(std::cin, wkst);
	MIATerminalTools::blankLine();
	
	const size_t len = wkst.length() + 1;
    wchar_t w_wkst[len];

    swprintf(w_wkst, len, L"...%s", wkst.c_str());
	
	cout << "...Loading NetWkstaGetInfo." << endl;
	
	wchar_t *argv[] = {nullptr, w_wkst};
	lola.NetWkstaGetInfoRunner(argc, argv);
	
	MIATerminalTools::blankDots();
	cout << "...NetSessionEnum Finished." << endl;
#else
    Error::returnError(31416);
#endif
}


void Commands::runNetRemoteComputerSupports()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    Error::returnError(31415);
	LOLANetUse lola;
	string wkst;
	int argc = 2;  //set to 4 indicating that all fields [0] -> [1] of argv are filled.
	
	cout << "...Please enter a workstation address for me to gather information about." << endl;
	MIATerminalTools::blankLine();
	getline(std::cin, wkst);
	MIATerminalTools::blankLine();
	
	const size_t len = wkst.length() + 1;
    wchar_t w_wkst[len];

    swprintf(w_wkst, len, L"...%s", wkst.c_str());
	
	cout << "...Loading NetRemoteComputerSupports." << endl;
	
	wchar_t *argv[] = {NULL, w_wkst};
	lola.NetRemoteComputerSupportsRunner(argc, argv);
	
	MIATerminalTools::blankDots();
	cout << "...NetRemoteComputerSupports Finished." << endl;
#else
    Error::returnError(31416);
#endif
}


void Commands::runNetUserEnum()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    Error::returnError(31415);
	LOLANetUse lola;
	string wkst;
	int argc = 2;  //set to 4 indicating that all fields [0] -> [1] of argv are filled.
	
	cout << "...Please enter a server for me to gather information about." << endl;
	MIATerminalTools::blankLine();
	getline(std::cin, wkst);
	MIATerminalTools::blankLine();
	
	const size_t len = wkst.length() + 1;
    wchar_t w_wkst[len];

    swprintf(w_wkst, len, L"...%s", wkst.c_str());
	
	cout << "...Loading NetUserEnum." << endl;
	
	wchar_t *argv[] = {nullptr, w_wkst};
	lola.NetUserEnumRunner(argc, argv);
	
	MIATerminalTools::blankDots();
	cout << "...NetUserEnum Finished." << endl;
#else
    Error::returnError(31416);
#endif
}


int Commands::rollDice(const string& input)
{
    int diceRoll = MathUtils::roll(input);
    return diceRoll;
}

void Commands::runSequencer()
{
    cout << "...Beginning MIA Sequencer." << endl;
    Sequencer seq;
    string input;
    cout << "...Enter a sequence to initiate: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, input);
    vector<string> names = seq.getSequenceNames();
    if (find(names.begin(), names.end(), input) != names.end())
    {
        seq.activateSequence(input);
    } else {
        Error::returnError(31423, input);
    }
}


void Commands::loopSequencer()
{
    cout << "...Beginning MIA Sequencer loop." << endl;
    cout << "...This will loop the sequence indefinitely." << endl;
    Sequencer seq;
    string input;
    cout << "...Enter a sequence to initiate: " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, input);
    vector<string> names = seq.getSequenceNames();
    if (find(names.begin(), names.end(), input) != names.end())
    {
        while(true)
        {
            seq.activateSequence(input);
        }
    } else {
        Error::returnError(31423, input);
    }
}


[[noreturn]] void Commands::runRepeatingAlarm()
{
    std::string input;
    cout << "...WARNING: This feature requires sudo privileges." << endl;
    cout << "...Enter time between alarm triggers (in seconds): " << endl;
    MIATerminalTools::blankLine();
    getline(std::cin, input);
    MIATerminalTools::blankLine();

    int interval = atoi(input.c_str());
    int minutes = interval/60;
    int totalTimeMS = interval*1000;


    cout << "...Beginning repeating alarm loop." << endl;

    while (true)
    {
        std::cout << "...Beeping!" << std::endl;
        for(int i=0;i<10;i++)
        {
            SystemSounds::beep();
            SystemUtils::sleepMilliseconds(75);
        }
        if(interval % 60 == 0)
        {
            for (int i = 0; i < minutes; i++)
            {
                std::cout << "Countdown: T - " << minutes - i << " minutes!" << std::endl;
                SystemUtils::sleepMilliseconds(totalTimeMS / minutes);
            }
        }
        else
        {
            SystemUtils::sleepMilliseconds(totalTimeMS);
        }
    }
}

void Commands::runDatabaseInterface()
{
#ifdef USE_Database_LIB
    MIADatabase database;
    database.initialize();
    if(database.connect())
    {

    }
    else
    {
        Error::returnError(Error::ErrorCode::Database_Connection_Failure);
    }
#elif __linux__
    Error::returnError(Error::ErrorCode::Database_Features_Not_Built);
#endif
}




//Function made for testing.
//This should be placed at the end of the file for easy accessibility. 
void Commands::runTest()
{
#ifdef USE_Database_LIB
    std::cout << "Testing config specific Database settings." << std::endl;
    MIADatabase db2;
    db2.initialize();
    db2.connect();
    db2.deleteTable("timeTable");
    std::vector<std::string> elements = {"id INT", "value VARCHAR(25)", "time TIME"};
    std::vector<std::string> element = {"id", "value", "time"};
    std::vector<std::string> value0 = {"0", "'Value0'", "'11:12:13'"};
    std::vector<std::string> value1 = {"1", "'Value1'", "'12:13:14'"};
    db2.addTable("timeTable", elements);
    db2.addElementToTable("timeTable", element, value0);
    db2.addElementToTable("timeTable", element, value1);
    //db2.testDatabase();
    db2.viewTable("timeTable");
    std::cout << "Finished Database testing." << std::endl;
#endif
    //Error::returnError(31418); //Returns nothing set for testing.

    ///* Uncomment this for testing things for Windows only.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    //Sequencer s;
	//s.activateSequence("test");
#elif __linux__
//    Music m;
//    m.playSong("test.mp3");
//    Error::returnError(31416);
#endif
}

void Commands::entangleTextRunner()
{
    string input;
    cout << "...Loading Text Entangler. " << endl;
    MIATerminalTools::blankDots();
    cout << "...Enter text to entangle: " << endl;
    getline(std::cin,input);
    std::vector<std::string> output = StringUtils::entangleText(input);
    cout << " First half: " << output[0] << endl;
    cout << "second half: " << output[1] << endl;
}
