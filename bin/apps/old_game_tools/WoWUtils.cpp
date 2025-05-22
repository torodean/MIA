/**
 * File: WoWUtils.cpp
 * Author: Antonius Torode
 * Created on: 05/22/2025
 * Description: This file is the implementation of various old WoW features from the original 
 *     MIA code. It originally lived in the VirtualKeyCodes file(s) but has since been decoupled. 
 */


// Include the associated header file.
#include "VirtualKeyStrokes.hpp"
// Include Timing for timing values.
#include "Timing.hpp"
// Old file used for configuration.
#include "Configurator.hpp"


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__

void WoWUtils::unloadLetters(int copies)
{	
	Configurator cfg;
	cfg.initialize();
    int x0 = cfg.getWoWMailboxFirstLetterLocation('x');
    int y0 = cfg.getWoWMailboxFirstLetterLocation('y');
    int x1 = cfg.getWoWMailboxLootLetterLocation('x');
    int y1 = cfg.getWoWMailboxLootLetterLocation('y');
    int x2 = cfg.getWoWMailboxDeleteLetterLocation('x');
    int y2 = cfg.getWoWMailboxDeleteLetterLocation('y');

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    for(int i=0;i<copies;i++){
        SetCursorPos(x0, y0);
        MIA_system::sleepMilliseconds(300);
        leftclick();
        MIA_system::sleepMilliseconds(300);
        SetCursorPos(x1, y1);
        MIA_system::sleepMilliseconds(300);
        leftclick();
        MIA_system::sleepMilliseconds(300);
        SetCursorPos(x2, y2);
        MIA_system::sleepMilliseconds(300);
        leftclick();
        MIA_system::sleepMilliseconds(300);
    }
}


void WoWUtils::duplicateLetter(int copies, string recipient)
{
	Configurator cfg;
	cfg.initialize();
    int x = cfg.getWoWMailboxSendLetterLocation('x');
    int y = cfg.getWoWMailboxSendLetterLocation('y');

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    for(int i=0;i<copies;i++){
        type(recipient);
        MIA_system::sleepMilliseconds(200);
        tab();
        MIA_system::sleepMilliseconds(200);
        type("subject");
        MIA_system::sleepMilliseconds(200);
        tab();
        MIA_system::sleepMilliseconds(200);
        paste();
        MIA_system::sleepMilliseconds(400);
        SetCursorPos(x, y);
        MIA_system::sleepMilliseconds(300);
        leftclick();
        MIA_system::sleepMilliseconds(2500);
    }
}

#endif // Windows specifier
