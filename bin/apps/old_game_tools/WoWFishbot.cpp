/**
 * File: WoWFishbot.cpp
 * Author: Antonius Torode
 * Created on: 05/22/2025
 * Description: This is the implementation of the fishbot functionality and features.This file contains 
 *     an old WoW fishbot from the original MIA code. It originally lived in the VirtualKeyCodes file(s) 
 *     but has since been decoupled. 
 */

#include <string>

// Include the associated header file.
#include "WoWFishbot.hpp"
// Include the virtual key strokes to use for simulation.
#include "VirtualKeyStrokes.hpp"
// Include Timing for timing values.
#include "Timing.hpp"
// Used for error handling.
#include "Error.hpp"
// Used for config path.
#include "Paths.hpp"
// Used for command line parsing.
#include "CommandOption.hpp"

using std::string;
using std::cout;
using std::endl;


WoWFishbot::WoWFishbot() : 
    config(defaultConfigFile),
    configFileOpt("-c", "--config", "Specify a config file to use (default = " +
                                    paths::getDefaultConfigDirToUse() + "/WoWConfig.MIA)",
                                    CommandOption::commandOptionType::stringOption)
                                    
{ };


void WoWFishbot::initialize(int argc, char* argv[])
{
    MIAApplication::initialize(argc, argv);
    
    std::string configFile = defaultConfigFile;
    configFileOpt.getOptionVal<std::string>(argc, argv, configFile);
    config.setConfigFileName(configFile);
    loadConfig();
}


int WoWFishbot::run()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__	
	//Default values.
	string fishButton = "3", lureButton = "8";
	
	cout << "...CAUTION! This fishbot was made for educational purposes." << endl;
	cout << "...WARNING! Use the fishbot at your own risk!" << endl;
	cout << "...DANGER! Using this fishbot may have negative consequences." << endl;
	cout << "...ALERT! This fishbot may get you banned." << endl;
	cout << "..." << endl;
	cout << "...In order for the fishbot to work, please enter in game settings and DISABLE" << endl << "... hardware cursor. See the manual for more details." << endl;
	cout << "...To use default values (3 for cast and 8 for lure) leave the following options" << endl << "... blank." << endl;
	cout << "...Press CTRL-C to stop the fishbot early once started." << endl;
	cout << "...Press ENTER to continue." << endl;
	std::cin.ignore();

	cout << "..." << endl;
	cout << "...Please enter which button you have set to cast: ";
	getline(std::cin, fishButton);
	cout << endl;
	cout << "...If you are not using a lure please enter 'NONE' " << endl;
	cout << "...Please enter which button you have set to apply a lure: ";
	getline(std::cin, lureButton);
	cout << endl;
	
	//Arbitrary default values based on my preferred setup.
	if(fishButton.empty() || fishButton == "\n" || fishButton == "\r")
	{
		fishButton = "3";
	}
	if(lureButton.empty() || lureButton == "\n" || lureButton == "\r")
	{
		lureButton = "8";
	}
	
	WoWFishBot(fishButton, lureButton);
#else
    error::returnError(31416);
    return 1;
#endif
    return 0;
}


// A fish bot made for WoW -- Not yet polished.
void WoWFishbot::WoWFishBot(string fishButton, string lureButton)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    fishBotIntro();

    //Begin useful variable initialization.
    HDC dc = GetDC(NULL);
    COLORREF color;
    int counter = 0;
    int red=1,green=1,blue=1;
    int increment = WoWFishBotIncrement;
    
    if(getVerboseMode())
        cout << "increment: " << increment << endl;
        
    int startX = WoWFishBotStartX + increment/2, startY = WoWFishBotStartY;
    
    if(getVerboseMode())
    {
        cout << "startX: " << startX << endl;
        cout << "startY: " << startY << endl;
    }
    
    int endX = WoWFishBotEndX, endY = WoWFishBotEndY;
    
    if(getVerboseMode())
    {
        cout << "endX: " << endX << endl;
        cout << "endY: " << endY << endl;
    }
    
    bool bobberFound = false;
    bool useLure = true;
    
    if(getVerboseMode()) 
        cout << "catchDelay: " << WoWFishBotDelay << endl;

    //Determines whether a lure is being used based on input.
    if (lureButton == "NONE" || lureButton == "None" || lureButton == "none" || lureButton == "n" || lureButton == "N")
    {
        useLure = false;
    }

    //Creates a clock for timing how long the bot has ran for.
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end;
    long elapsed_time = 0;

    //Run the fishbot for some number of casts - determined by the config file variable WoWFishBotNumOfCasts.
    while(counter < WoWFishBotNumOfCasts){

        //Applies lure.
        if (useLure && counter % 100 == 0)
        {
            cout << "...Applying lure." << endl;
            type(lureButton);
            MIA_system::sleepMilliseconds(3000);
        }

        //Casts.
        cout << "...Casting." << endl;
        type(fishButton);
        MIA_system::sleepMilliseconds(1500);
        cout << "...Scanning." << endl;

        //Finds bobber.
        for (int j=startY;j<endY;j+=increment)
        {
            for (int i=startX;i<endX;i+=increment)
            {
                SetCursorPos(i,j);
                MIA_system::sleepMilliseconds(2);
                color = GetPixel(dc, i, j);
                //color = GetPixel(dc, cursor.x, cursor.y);

                getRGB(color, red, green, blue);

                //Troubleshooting printouts for color of pixels detected.
                if (getVerboseMode()){
                    cout << "(x,y): " << "(" << i << "," << j << ")" << endl;
                    cout << "Red: " << red << "  --  " << "Green: " << green << "  --  " << "Blue: " << blue << endl;
                    cout << "RGB: (" << red << "," << green << "," << blue << ")" << endl;
                }

                //Check if the mouse is over the bobber.
                if(red > green + 35 || red > blue + 35){
                    cout << "...The bobber has been found!! ...I think." << endl;
                    bobberFound=true;
                    break;
                }
            }
            if(bobberFound)
            {
                break;
            }
        }
        if(!bobberFound)
        {
            end = std::chrono::steady_clock::now();
            cout << "...I was unable to find the bobber!" << endl;
            cout << "...To make it look like we're not cheating of course." << endl;
        }

        //Waits a delay time and then clocks the bobber if it was found.
        if(bobberFound){
            MIA_system::sleepMilliseconds(WoWFishBotDelay);
            leftclick();
        }
        MIA_system::sleepMilliseconds(1000);

        //Determines elapsed time and progress information.
        end = std::chrono::steady_clock::now();
        elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        counter++;
        bobberFound = false;
        cout << "...Elapsed time: " << elapsed_time << " milliseconds." << endl;
        cout << "...Number of casts: " << counter << endl;
        elapsed_time = 0;
    }

    ReleaseDC(NULL, dc);
#elif __LINUX__
    cout << "This feature is not programmed for linux!" << endl;
    return;
#endif
}


void WoWFishbot::fishBotIntro()
{
    int drama = 400;
    //Some gibberish for dramatic effect.
    //Also serves as a brief load time before bot starts.
    cout << "...Loading Fishbot Modules." << endl;
    MIA_system::sleepMilliseconds(drama);
    cout << "...Calculating response functions." << endl;
    MIA_system::sleepMilliseconds(drama);
    cout << "...Detecting saved crypto-keys." << endl;
    MIA_system::sleepMilliseconds(2*drama);
    cout << "...Saved keys found!" << endl;
    cout << "...Decrypting password hash values." << endl;
    MIA_system::sleepMilliseconds(drama);
    cout << "...MIA_Success!." << endl;
    cout << "...Sending security information to host." << endl;
    MIA_system::sleepMilliseconds(2*drama);
    cout << "...MIA_Success!." << endl;
    cout << "...Disabling daemon ninja process." << endl;
    MIA_system::sleepMilliseconds(drama);
    cout << ".." << endl;
    cout << "...Number of casts set to: " << WoWFishBotNumOfCasts << endl;
    MIA_system::sleepMilliseconds(drama);
    cout << "...Starting fishbot!" << endl;
    MIA_system::sleepMilliseconds(drama);
    cout << ".." << endl;
}

void WoWFishbot::loadConfig()
{
    WoWFishBotStartX = config.getInt("WoWFishBotStartX");
    WoWFishBotStartY = config.getInt("WoWFishBotStartY");
    WoWFishBotEndX = config.getInt("WoWFishBotEndX");
    WoWFishBotEndY = config.getInt("WoWFishBotEndY");
    WoWFishBotIncrement = config.getInt("WoWFishBotIncrement");
    WoWFishBotNumOfCasts = config.getInt("WoWFishBotNumOfCasts");
    WoWFishBotDelay = config.getInt("WoWFishBotDelay");
}

void WoWFishbot::printHelp() const
{
    MIAApplication::printHelp();
    
    cout << "Fishbot specific options:" << endl
         << configFileOpt.getHelp() 
         << endl;
}
