/**
 * File: Commands.cpp
 * Author: Antonius Torode
 * Creation Date: 03/01/2021
 * Description:
 */

#include <iostream>
#include "Commands.hpp"
#include "StringUtils.hpp"
#include "D3CMath.hpp"
#include "Error.hpp"
#include "MIAException.hpp"
#include "FileUtils.hpp"
#include "MathUtils.hpp"
#include "SystemSounds.hpp"
#include "Timing.hpp"
#include "VirtualKeyStrokes.hpp"

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
using math::D3CMath;
using virtual_keys::VirtualKeyStrokes;


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
    else if (input == "button spam")
        output = MIAInput::BUTTONSPAM;
    else if (input == "button spam -t")
        output = MIAInput::BUTTONSPAM_T;
    else if (input == "prime -help" || input == "prime --help")
        output = MIAInput::PRIME_HELP;
    else if (input == "crypt -d0s3")
        output = MIAInput::CRYPT_D0S3;
    else if (input == "decrypt -d0s3")
        output = MIAInput::DECRYPT_D0S3;
    else if (input == "randfromfile")
        output = MIAInput::RANDFROMFILE;
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
    else if (input == "splash")
        output = MIAInput::SPLASH;
    else if (input == "net -help")
		output = MIAInput::NETHELP;
    else if (input == "net session")
        output = MIAInput::NETSESSION;
    else if (input == "net server -w")
        output = MIAInput::NETSERVER_W;
    else if (input == "net server -s")
        output = MIAInput::NETSERVER_S;
    else if (input == "date")
        output = MIAInput::DATE;
    else if (StringUtils::inputRoll(input))
        output = MIAInput::DICEROLL;
    else if (input == "error info")
        output = MIAInput::ERRORINFO;
    else if (input == "error info -a")
        output = MIAInput::ERRORINFO_A;
    return output;
}


void Commands::collatzRunner()
{
    string input, output;

    cout << "...Enter a starting number (positive integer): " << endl;
    cout << endl;
    getline(std::cin,input);
    cout << endl;

    long longInput = atol(input.c_str());

    output = math::productCollatzSequence(longInput);

    cout << "...collatz: " << output << endl;
    cout << endl;
    cout << "...Collatz sequence complete." << endl;
    cout << endl;
}


void Commands::stringAdditionRunner()
{
    string intOne, intTwo, output;

    cout << "...Addition of two integers of any size." << endl << "...Enter first integer to add: " << endl;
    cout << endl;
    getline(std::cin, intOne);
    cout << endl;

    cout << "...Enter second integer to add: " << endl;
    cout << endl;
    getline(std::cin, intTwo);
    cout << endl;

    output = math::addTwoStrings(intOne, intTwo);

    cout << "...Integer sum: " << output << endl;
    cout << endl;
}


void Commands::stringMultiplyRunner()
{
    string intOne, intTwo, output;

    cout << "...Multiplication of two integers of any size." << endl << "...Enter first integer to multiply: " << endl;
    cout << endl;
    getline(std::cin, intOne);
    cout << endl;

    cout << "...Enter second integer to multiply: " << endl;
    cout << endl;
    getline(std::cin, intTwo);
    cout << endl;

    output = math::multiplyTwoStrings(intOne, intTwo);

    cout << "...Integer product: " << output << endl;
    cout << endl;
}


void Commands::stringSubtractionRunner()
{
    string intOne, intTwo, output;

    cout << "...Subtraction of two integers of any size." << endl << "...Enter first integer: " << endl;
    cout << endl;
    getline(std::cin, intOne);
    cout << endl;

    cout << "...Enter second integer to subtract: " << endl;
    cout << endl;
    getline(std::cin, intTwo);
    cout << endl;

    output = math::subtractTwoStrings(intOne, intTwo);

    cout << "...Integer difference: " << output << endl;
    cout << endl;
}


void Commands::isPrimeRunner()
{
    D3CMath math;
    string input;

    cout << "...Enter a positive Integer: " << endl;
    cout << endl;
    getline(std::cin, input);
    cout << endl;

    long longInput = atol(input.c_str());

    if(math.isPrime(longInput))
    {
        cout << "...The number you entered is prime. " << endl;
    }else{
        cout << "...The number you entered is NOT prime. " << endl;
    }
    cout << endl;
}


void Commands::isPalindromeRunner()
{
    string input;

    cout << "...Enter a positive Integer: " << endl;
    cout << endl;
    getline(std::cin, input);
    cout << endl;

    long longInput = atol(input.c_str());

    if(math::isPalindrome(longInput))
    {
        cout << "...The number you entered is a palindrome. " << endl;
    }else{
        cout << "...The number you entered is NOT a palindrome. " << endl;
    }
    cout << endl;
}


void Commands::sumOfDigitsRunner()
{
    string input;

    cout << "...Enter a positive Integer of any length: " << endl;
    cout << endl;
    getline(std::cin, input);
    cout << endl;

    long output = math::sumOfDigits(input);

    cout << "...Sum of Digits: " << output << endl;
    cout << endl;
}


void Commands::primeFactorsRunner()
{
    D3CMath math;
    string input;

    cout << "...Enter a positive Integer: " << endl;
    cout << endl;
    getline(std::cin, input);
    cout << endl;

    long longInput = atol(input.c_str());

    cout << "...The prime factors are: ";
    for (long i = 1; i <= (longInput + 1) / 2; i++) {
        if (math.isPrime(i) && longInput % i == 0)
        {
            cout << i << " ";
        }
    }
    cout << endl;
}


void Commands::numberOfFactorsRunner()
{
    string input;

    cout << "...Enter a positive Integer: " << endl;
    cout << endl;
    getline(std::cin, input);
    cout << endl;

    long longInput = atol(input.c_str());

    int factors = math::numberOfFactors(longInput);

    cout << "...The number of factors are: " << factors << endl;
    cout << endl;
}


void Commands::isTriangleNumberRunner()
{
    string input;

    cout << "...Enter a positive Integer: " << endl;
    cout << endl;
    getline(std::cin, input);
    cout << endl;

    long longInput = atol(input.c_str());

    if(math::isTriangleNumber(longInput))
    {
        cout << "...The number you entered is a triangle number." << endl;
    }else{
        cout << "...The number you entered is NOT a triangle number." << endl;
    }
    cout << endl;
}


void Commands::latticePathsRunner()
{
    string n, m;

    cout << endl;
    cout << "...IMPORTANT: This feature depends on the c++ long. " << endl;
    cout << "...Therefore if the lattice paths exceed 2147483647,  " << endl;
    cout << "...the answers will no longer be reliable. " << endl;
    cout << endl;

    cout << "...Enter grid size in horizontal direction: : " << endl;
    cout << endl;
    getline(std::cin, n);
    cout << endl;
    cout << "...Enter grid size in vertical direction: : " << endl;
    cout << endl;
    getline(std::cin, m);
    cout << endl;

    long intOne = atol(n.c_str());
    long intTwo = atol(m.c_str());
    long output = math::latticePathsOfSquare(intOne, intTwo);

    cout << "...The total lattice paths are: " << output << endl;
    cout << endl;
}


void Commands::primeNRunner()
{
    D3CMath math;
    string input;

    cout << "...Enter a positive integer: " << endl;
    cout << endl;
    getline(std::cin, input);
    cout << endl;

    long longInput = atol(input.c_str());
    long output = math.primeNumberN(longInput);

    cout << "...Prime number " << input << ": " << output << endl;
    cout << endl;
}


void Commands::primeNumberNpopulateRunner()
{
    D3CMath math;

    cout << endl;
    cout << "...PrimeNumberList.txt now being written to: " << math.returnPrimeNlocation() << endl;
    cout << "...This file will increase the efficiency of the 'prime -n' command." << endl;
    cout << "...This may take some time..." << endl;
    cout << endl;

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
	cout << endl;
	
	string amountStr;

	cout << "...How many times would you like it spammed? ";
	getline(std::cin, amountStr);
	cout << endl;
	
	int pause;
	
	cout << "...How long between each key press (in ms)? ";
	std::cin >> pause;
	cout << endl;
	
	int amount = atoi(amountStr.c_str());
	
	cout << "...Preparing to spam " << button << " " << amountStr << " times." << endl;
	cout << "...Beginning in 5 seconds." << endl;
	cout << endl;

	if (enableTab)
		key.buttonSpamTab(button, amount, pause);
	else
		key.buttonSpam(button, amount, pause);
}

/* TODO - Keep these around until the MinecraftUtils app is finished (if I make one).
void Commands::minecraftDigRunner()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes key;
	
	string input;

	cout << "...How long would you like to dig for (in seconds)? ";
	getline(std::cin, input);
	cout << endl;
	
	int time = atoi(input.c_str());
	
	cout << "...Beginning in 5 seconds." << endl;
	key.minecraftDig(time);
#else
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
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
	cout << endl;
	
	cout << "...Enter a starting X coordinate (integer): ";
	std::cin >> startx;
	cout << endl;
	
	cout << "...Enter an ending X coordinate (integer): ";
	std::cin >> stopx;
	cout << endl;
	
	cout << "...Enter a starting Z coordinate (integer): ";
	std::cin >> startz;
	cout << endl;
	
	cout << "...Enter an ending Z coordinate (integer): ";
	std::cin >> stopz;
	cout << endl;
	
	cout << "...Enter the intermidiate time between steps in milliseconds (integer): ";
	std::cin >> breakTime;
	cout << endl;
	
	double totalSteps = ((stopx-startx)*1.0/(stepSize*1.0)+1)*((stopz-startz)*1.0/(stepSize*1.0)+1);
	cout << "...totalSteps: " << totalSteps << endl;
	
	cout << "...Beginning in " << delayStartTime << " milliseconds." << endl;
	cout << endl;
	
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
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
}
*/

/* TODO - Keep these around until the WoWUtils app is finished.
void Commands::duplicateLetterRunner()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes keys;
	
	cout << "...INSTRUCTIONS: Please have the contents of the letter copied to your clipboard." << endl;
	cout << "...INSTRUCTIONS: After entering final parameters, move mouse curser to the WoW 'send' button." << endl;

	cout << endl;
	
	int copies;
	cout << "...Please enter the number of letters you want duplicated: ";
	std::cin >> copies;
	std::cin.ignore();
	cout << endl;
	
	string recipient;
	cout << "...Please enter the recipient you want the letters sent to: ";
	getline(std::cin, recipient);
	cout << endl;
	
	cout << "...Beginning in 5000 milliseconds (5s)." << endl;
	cout << endl;
	keys.duplicateLetter(copies, recipient);
	
	cout << "...Finished duplicating letters." << endl;
#else
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
}


void Commands::unloadLetterRunner()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes keys;
	
	cout << "...INSTRUCTIONS: Please have your WoW inbox open." << endl;
	cout << "...INSTRUCTIONS: Please set MIAConfig variables appropriately (see manual)." << endl;

	cout << endl;
	
	int copies;
	cout << "...Please enter the number of letters you want unloaded: ";
	std::cin >> copies;
	std::cin.ignore();
	cout << endl;
	
	cout << "...Beginning in 5000 milliseconds (5s)." << endl;
	cout << endl;
	keys.unloadLetters(copies);
	
	cout << "...Finished unloading letters." << endl;
#else
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
}
*/

void Commands::d0s3CryptRunner()
{
    /*
    MIAEncrypt crypt(3);
    string inputFile;
    */    
    throw error::MIAException(error::ErrorCode::Feature_In_Dev);

    /*
    cout << "...Please enter a file (using default file path) to be encrypted: " << endl;
    cout << endl;
    getline(std::cin, inputFile);

    crypt.encryptFile(inputFile, "OutputFile");
     */
}


void Commands::d0s3DeCryptRunner()
{
    throw error::MIAException(error::ErrorCode::Feature_In_Dev);
}


void Commands::printRandomLinesFromFileRunner()
{
    int lines;
    cout << "...How many random lines would you like to print from a file? ";
    std::cin >> lines;
    std::cin.ignore();
    cout << endl;

    string filePath;
    cout << "...Please enter the complete file path of the file: ";
    getline(std::cin, filePath);
    cout << endl;

    files::printRandomLinesFromFile(filePath, lines);
}


void Commands::solveQuadraticFormulaRunner()
{
    double a,b,c;
    cout << "...This is a solver for equations of the form a*x^2+b*x+c=0." << endl;
    cout << "...Please Enter a: ";
    std::cin >> a;
    cout << "...Please Enter b: ";
    std::cin >> b;
    cout << "...Please Enter c: ";
    std::cin >> c;
    std::cin.ignore();
    cout << endl;

    string ans = math::solveQuadraticFormula(a,b,c);
    cout << endl;
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
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
}


void Commands::eyedropper()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    VirtualKeyStrokes keys;
	keys.getPixelColorAtMouse();
#else
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
}


void Commands::runNetSessionEnum()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    throw error::MIAException(error::ErrorCode::Feature_In_Dev);
	LOLANetUse lola;
	string server;
	int argc = 4;  //set to 4 indicating that all fields [0] -> [3] of argv are filled.
	
	cout << "...Please enter a server address for me to gather information about." << endl;
	cout << endl;
	getline(std::cin, server);
	cout << endl;
	
	const size_t len = server.length() + 1;
	if(Configurator::getVerboseMode())
		cout << "len: " << len << endl;
    wchar_t w_server[len];

    swprintf(w_server, len, L"%s", server.c_str());
	
	cout << "...Loading NetSessionEnumRunner." << endl;
	
	wchar_t *argv[] = {nullptr, w_server, nullptr, nullptr};
	lola.NetSessionEnumRunner(argc, argv);
	
	cout << "..." << endl;
	cout << "...NetSessionEnumRunner Finished." << endl;
#else
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
}


void Commands::runNetServerEnum(char mode)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    throw error::MIAException(error::ErrorCode::Feature_In_Dev);
	LOLANetUse lola;
	string domain;
	int argc = 2;  //set to 2 indicating that all fields [0] -> [1] of argv are filled.
	
	cout << "...Please enter a domain for me to gather information about." << endl;
	cout << endl;
	getline(std::cin, domain);
	cout << endl;
	
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
	
	cout << "..." << endl;
	cout << "...NetServerEnum Finished!." << endl;
#else
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
}


void Commands::runNetWkstaGetInfo()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    throw error::MIAException(error::ErrorCode::Feature_In_Dev);
	LOLANetUse lola;
	string wkst;
	int argc = 2;  //set to 4 indicating that all fields [0] -> [1] of argv are filled.
	
	cout << "...Please enter a workstation address for me to gather information about." << endl;
	cout << endl;
	getline(std::cin, wkst);
	cout << endl;
	
	const size_t len = wkst.length() + 1;
    wchar_t w_wkst[len];

    swprintf(w_wkst, len, L"...%s", wkst.c_str());
	
	cout << "...Loading NetWkstaGetInfo." << endl;
	
	wchar_t *argv[] = {nullptr, w_wkst};
	lola.NetWkstaGetInfoRunner(argc, argv);
	
	cout << "..." << endl;
	cout << "...NetSessionEnum Finished." << endl;
#else
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
}


void Commands::runNetRemoteComputerSupports()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    throw error::MIAException(error::ErrorCode::Feature_In_Dev);
	LOLANetUse lola;
	string wkst;
	int argc = 2;  //set to 4 indicating that all fields [0] -> [1] of argv are filled.
	
	cout << "...Please enter a workstation address for me to gather information about." << endl;
	cout << endl;
	getline(std::cin, wkst);
	cout << endl;
	
	const size_t len = wkst.length() + 1;
    wchar_t w_wkst[len];

    swprintf(w_wkst, len, L"...%s", wkst.c_str());
	
	cout << "...Loading NetRemoteComputerSupports." << endl;
	
	wchar_t *argv[] = {NULL, w_wkst};
	lola.NetRemoteComputerSupportsRunner(argc, argv);
	
	cout << "..." << endl;
	cout << "...NetRemoteComputerSupports Finished." << endl;
#else
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
}


void Commands::runNetUserEnum()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    throw error::MIAException(error::ErrorCode::Feature_In_Dev);
	LOLANetUse lola;
	string wkst;
	int argc = 2;  //set to 4 indicating that all fields [0] -> [1] of argv are filled.
	
	cout << "...Please enter a server for me to gather information about." << endl;
	cout << endl;
	getline(std::cin, wkst);
	cout << endl;
	
	const size_t len = wkst.length() + 1;
    wchar_t w_wkst[len];

    swprintf(w_wkst, len, L"...%s", wkst.c_str());
	
	cout << "...Loading NetUserEnum." << endl;
	
	wchar_t *argv[] = {nullptr, w_wkst};
	lola.NetUserEnumRunner(argc, argv);
	
	cout << "..." << endl;
	cout << "...NetUserEnum Finished." << endl;
#else
    throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
#endif
}


int Commands::rollDice(const string& input)
{
    int diceRoll = math::roll(input);
    return diceRoll;
}


[[noreturn]] void Commands::runRepeatingAlarm()
{
    std::string input;
    cout << "...WARNING: This feature requires sudo privileges." << endl;
    cout << "...Enter time between alarm triggers (in seconds): " << endl;
    cout << endl;
    getline(std::cin, input);
    cout << endl;

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
            timing::sleepMilliseconds(75);
        }
        if(interval % 60 == 0)
        {
            for (int i = 0; i < minutes; i++)
            {
                std::cout << "Countdown: T - " << minutes - i << " minutes!" << std::endl;
                timing::sleepMilliseconds(totalTimeMS / minutes);
            }
        }
        else
        {
            timing::sleepMilliseconds(totalTimeMS);
        }
    }
}


void Commands::entangleTextRunner()
{
    string input;
    cout << "...Loading Text Entangler. " << endl;
    cout << "..." << endl;
    cout << "...Enter text to entangle: " << endl;
    getline(std::cin,input);
    std::vector<std::string> output = StringUtils::entangleText(input);
    cout << " First half: " << output[0] << endl;
    cout << "second half: " << output[1] << endl;
}
