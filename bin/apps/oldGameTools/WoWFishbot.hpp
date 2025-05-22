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
 
/**
 * This class contains the code related to the WoWFishbot. This code was decoupled from other
 * utility code within the MIA project and may need some fiddling, changes, or testing before
 * it works accurately again.
 */
class WoWFishbot
{
public:
    /**
     * The main constructor of the WoWFishbot class.
     */
    WoWFishbot();
    
    /// Default destructor.
    ~WoWFishbot();
    
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    
        /**
         * A fishbot for World of Warcraft.
         * @param fishButton[std::string] - The button which will trigger the fishing cast.
         * @param lureButton[std::string] - The button which will trigger a lure use.
         * @note This method only works on Windows. Primarily because WoW only has official Windows support. 
         */
        void WoWFishBot(std::string fishButton, std::string lureButton);
        
    #endif // Windows specifier

private:
    /// Intro text for the fishbot. This is mostly for flavor.
    void fishBotIntro();
    
} // class WoWFishbot
