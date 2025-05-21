/**
 * File: VirtualKeyStrokes.hpp
 * Author: Antonius Torode
 * Created on: 03/01/2021
 * Description: Declares the VirtualKeyStrokes class, a cross-platform utility for simulating
 *     keyboard and mouse input. Supports character and string input emulation, 
 *     basic automation tasks, and platform-specific input functions for Windows and Linux.
 */
#pragma once

#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdio>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#include <windows.h>
#elif __linux__
extern "C" 
{
#include <xdo.h>
}
#endif

namespace system
{
    /**
     * This class is for sending virtual key strokes and simulating user input.
     * This class is cross platform.
     */
    class VirtualKeyStrokes
    {
    public:    
        /**
         * Main default constructor for the VirtualKeyStrokes class.
         */
        VirtualKeyStrokes();
    
        /**
         * Main destructor for the VirtualKeyStrokes object.
         */
        ~VirtualKeyStrokes();
    
        /**
         * This will simulate a key press.
         * @param character[const char&] - the key to press.
         * @param holdTime [int] - A time to gold after the keypress in ms (default = 0)
         */
        void press(const char& character, int holdTime = 0);
    
        /**
         * This will simulate the key presses for a string.
         * @param character[const std::string&] - the string to simulate key presses for.
         */
        void type(std::string word);
    
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__

        // @TODO - these methods can be simplified into one (or at least two).
        void one(int holdTime = 0);
        void two(int holdTime = 0);
        void three(int holdTime = 0);
        void four(int holdTime = 0);
        void five(int holdTime = 0);
        void six(int holdTime = 0);
        void seven(int holdTime = 0);
        void eight(int holdTime = 0);
        void nine(int holdTime = 0);
        void zero(int holdTime = 0);
        void enter();
        void a(int holdTime = 0);
        void b(int holdTime = 0);
        void c(int holdTime = 0);
        void d(int holdTime = 0);
        void e(int holdTime = 0);
        void f(int holdTime = 0);
        void g(int holdTime = 0);
        void h(int holdTime = 0);
        void i(int holdTime = 0);
        void j(int holdTime = 0);
        void k(int holdTime = 0);
        void l(int holdTime = 0);
        void m(int holdTime = 0);
        void n(int holdTime = 0);
        void o(int holdTime = 0);
        void p(int holdTime = 0);
        void q(int holdTime = 0);
        void r(int holdTime = 0);
        void s(int holdTime = 0);
        void t(int holdTime = 0);
        void u(int holdTime = 0);
        void v(int holdTime = 0);
        void w(int holdTime = 0);
        void x(int holdTime = 0);
        void y(int holdTime = 0);
        void z(int holdTime = 0);
        void numlock();
        void alt0248();
        void alt136();
        void shift(char character);
        void paste();
        void backslash();
        void slash();
        void leftclick();
        void rightclick();
    
        // Functions relating to Minecraft. // @TODO - move to an app
        void minecraftDig(int time);
    
        // Functions relating to World of Warcraft. // @TODO - move to an app
        void duplicateLetter(int copies, std::string recipient);
        void unloadLetters(int copies);
        void WoWFishBot(std::string fishButton, std::string lureButton);
    
        // Other useful functions.
        void findMouseCoords(int waitTime);
        void getPixelColor();
        void getPixelColorAtMouse();
        void moveMouseTo(int x, int y);
    
    #endif
    
        void minus();       ///< Simulates press of the minus key.
        void equal();       ///< Simulates press of the equal key.
        void space();       ///< Simulates press of the space key.
        void tab();         ///< Simulates press of the tab key.
    
        /// Sleeps for the default globalSleep time used by the VirtuakKeyStrokes class..
        void defaultSleep() const;
    	void sleep(int time);
    
        /**
         * This will spam a key press a certain number of times with a pause between each press.
         * @param button[char button] - the character to press.
         * @param amount[int] - The number of times to press the key.
         * @param pause[int] - The time to pause between key presses in ms.
         */
        void buttonSpam(const std::string& button, int amount, int pause);
    
        /**
         * This will spam a key press a certain number of times with a pause between each press and a tab after.
         * @param button[char button] - the character to press.
         * @param amount[int] - The number of times to press the key.
         * @param pause[int] - The time to pause between key presses in ms.
         */
        void buttonSpamTab(const std::string& button, int amount, int pause);
    
    private:
    
        /// Global time to sleep between each key press (in ms).
        int globalSleep = 10;
    
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
        INPUT ip;
    
        void fishBotIntro(); // @TODO - move to an app
        void getRGB(COLORREF& color, int& r, int& g, int& b);
    #elif __linux__
        /**
         * This pointer is used for the xdotool which can simulate key strokes on linux through X11.
         * Useful docs of the library and related files can be found here"
         * https://libxdo-d.dpldocs.info/xdo.xdo_send_keysequence_window.html
         * A list of keysyms that can be used are here:
         * https://wiki.linuxquestions.org/wiki/List_of_keysyms
         */
        xdo_t *xdo;
    #endif
    }; // class VirtualKeyStrokes
} // namespace system
