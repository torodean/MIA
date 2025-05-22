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
#include "VirtualKeyStrokes.hpp"
// Include Timing for timing values.
#include "Timing.hpp"
// Old file used for configuration.
#include "Configurator.hpp"

using std::string;
using std::cout;
using std::endl;


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
// A fish bot made for WoW -- Not yet polished.
void WoWFishbot::WoWFishBot(string fishButton, string lureButton)
{
	Configurator cfg;
	cfg.initialize();
    fishBotIntro();

    //Begin useful variable initialization.
    HDC dc = GetDC(NULL);
    COLORREF color;
    int counter = 0;
    int red=1,green=1,blue=1;
    int increment = cfg.getWoWFishBotSpace("increment");
    if(Configurator::getVerboseMode())
        cout << "increment: " << increment << endl;
    int startX = cfg.getWoWFishBotSpace("startX") + increment/2, startY = cfg.getWoWFishBotSpace("startY");
    if(Configurator::getVerboseMode()){
        cout << "startX: " << startX << endl;
        cout << "startY: " << startY << endl;
    }
    int endX = cfg.getWoWFishBotSpace("endX"), endY = cfg.getWoWFishBotSpace("endY");
    if(Configurator::getVerboseMode()){
        cout << "endX: " << endX << endl;
        cout << "endY: " << endY << endl;
    }
    bool bobberFound = false;
    bool useLure = true;
    int catchDelay = cfg.getWoWFishBotSpace("delay");
    if(Configurator::getVerboseMode()) cout << "catchDelay: " << catchDelay << endl;

    //Determines whether a lure is being used based on input.
    if (lureButton == "NONE" || lureButton == "None" || lureButton == "none" || lureButton == "n" || lureButton == "N"){
        useLure = false;
    }

    //Creates a clock for timing how long the bot has ran for.
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end;
    long elapsed_time = 0;

    //Run the fishbot for some number of casts - determined by the config file variable WoWFishBotNumOfCasts.
    while(counter < cfg.getWoWFishBotSpace("casts")){

        //Applies lure.
        if (useLure && counter % 100 == 0){
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
        for (int j=startY;j<endY;j+=increment){
            for (int i=startX;i<endX;i+=increment){
                SetCursorPos(i,j);
                MIA_system::sleepMilliseconds(2);
                color = GetPixel(dc, i, j);
                //color = GetPixel(dc, cursor.x, cursor.y);

                getRGB(color, red, green, blue);

                //Troubleshooting printouts for color of pixels detected.
                if (Configurator::getVerboseMode()){
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
            if(bobberFound){
                break;
            }
        }
        if(!bobberFound){
            end = std::chrono::steady_clock::now();
            cout << "...I was unable to find the bobber!" << endl;
            cout << "...To make it look like we're not cheating of course." << endl;
        }

        //Waits a delay time and then clocks the bobber if it was found.
        if(bobberFound){
            MIA_system::sleepMilliseconds(catchDelay);
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
}
#endif


void WoWFishbot::fishBotIntro()
{
	Configurator cfg;
	cfg.initialize();
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
    cout << "...Number of casts set to: " << cfg.getWoWFishBotSpace("casts") << endl;
    MIA_system::sleepMilliseconds(drama);
    cout << "...Starting fishbot!" << endl;
    MIA_system::sleepMilliseconds(drama);
    cout << ".." << endl;
}
