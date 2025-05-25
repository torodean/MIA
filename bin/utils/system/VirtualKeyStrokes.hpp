/**
 * File: VirtualKeyStrokes.hpp
 * Author: Antonius Torode
 * Created on: 03/01/2021
 * Description: Declares the VirtualKeyStrokes class, a cross-platform utility for simulating
 *     keyboard and mouse input. Supports character and string input emulation, 
 *     basic automation tasks, and platform-specific input functions for Windows and Linux.
 */
// @TODO - this file needs some work.
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

namespace MIA_system
{
    /**
     * This class is for sending virtual key strokes and simulating user input.
     * This class is cross platform.
     */
    class VirtualKeyStrokes
    {
    public:
    
        /**
         * An enum representing the various types of clicks available via a mouse.
         */
        enum ClickType
        {
            LEFT_CLICK,   ///< Represents a left click.
            RIGHT_CLICK,  ///< Represents a right click.
            MIDDLE_CLICK  ///< Represents a middle click.
        };
    
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
         * @param verboseMode[bool] If true, prints the pressed number to standard output.
         */
        void press(const char& character, int holdTime = 0, bool verboseMode = false);
    
        /**
         * This will simulate the key presses for a string.
         * @param character[const std::string&] - the string to simulate key presses for.
         */
        void type(std::string word);
    
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__

        /**
         * @brief Simulates pressing a number key (0–9) using virtual key codes.
         *
         * This function sends a key press and release event for the specified number key.
         * It validates input to ensure the number is within [0, 9], simulates a press using
         * Windows SendInput API, waits for the specified hold duration, and then releases
         * the key. Optionally prints the number to stdout if verboseMode is enabled.
         *
         * @param num[int] - The number key to press (must be in range 0–9).
         * @param holdTime[int] - Duration in milliseconds to hold the key before releasing.
         * @param verboseMode[bool] - If true, prints the pressed number to standard output.
         */
        void pressNumber(int num, int holdTime = 0, bool verboseMode = false);
        
        /**
         * @brief Simulates pressing an alphabetic key (a–z or A–Z) using virtual key codes.
         *
         * This function sends a key press and release event for the specified character.
         * It supports both uppercase and lowercase letters by adjusting the virtual key code
         * and shift state as needed. Uses the Windows SendInput API to simulate input.
         *
         * @param ch[char] - The character to press (must be a letter a–z or A–Z).
         * @param holdTime[int] - Duration in milliseconds to hold the key before releasing.
         * @param verboseMode[bool] - If true, prints the pressed character to standard output.
         */
        void VirtualKeyStrokes::pressChar(char ch, int holdTime = 0, bool verboseMode = false);
        
        // The old methods which are now deprecated.
        [[deprecated("Use pressNumber(1) instead.")]]
        void one(int holdTime = 0, bool verboseMode = false);
        [[deprecated("Use pressNumber(2) instead.")]]
        void two(int holdTime = 0, bool verboseMode = false);
        [[deprecated("Use pressNumber(3) instead.")]]
        void three(int holdTime = 0, bool verboseMode = false);
        [[deprecated("Use pressNumber(4) instead.")]]
        void four(int holdTime = 0, bool verboseMode = false);
        [[deprecated("Use pressNumber(5) instead.")]]
        void five(int holdTime = 0, bool verboseMode = false);
        [[deprecated("Use pressNumber(6) instead.")]]
        void six(int holdTime = 0, bool verboseMode = false);
        [[deprecated("Use pressNumber(7) instead.")]]
        void seven(int holdTime = 0, bool verboseMode = false);
        [[deprecated("Use pressNumber(8) instead.")]]
        void eight(int holdTime = 0, bool verboseMode = false);
        [[deprecated("Use pressNumber(9) instead.")]]
        void nine(int holdTime = 0, bool verboseMode = false);
        [[deprecated("Use pressNumber(0) instead.")]]
        void zero(int holdTime = 0, bool verboseMode = false);
        
        // @TODO - these methods can be simplified into one (or at least a few).
        [[deprecated("Use pressChar(a) instead.")]]
        void a(int holdTime = 0);
        [[deprecated("Use pressChar(b) instead.")]]
        void b(int holdTime = 0);
        [[deprecated("Use pressChar(c) instead.")]]
        void c(int holdTime = 0);
        [[deprecated("Use pressChar(d) instead.")]]
        void d(int holdTime = 0);
        [[deprecated("Use pressChar(e) instead.")]]
        void e(int holdTime = 0);
        [[deprecated("Use pressChar(f) instead.")]]
        void f(int holdTime = 0);
        [[deprecated("Use pressChar(g) instead.")]]
        void g(int holdTime = 0);
        [[deprecated("Use pressChar(h) instead.")]]
        void h(int holdTime = 0);
        [[deprecated("Use pressChar(i) instead.")]]
        void i(int holdTime = 0);
        [[deprecated("Use pressChar(j) instead.")]]
        void j(int holdTime = 0);
        [[deprecated("Use pressChar(k) instead.")]]
        void k(int holdTime = 0);
        [[deprecated("Use pressChar(l) instead.")]]
        void l(int holdTime = 0);
        [[deprecated("Use pressChar(m) instead.")]]
        void m(int holdTime = 0);
        [[deprecated("Use pressChar(n) instead.")]]
        void n(int holdTime = 0);
        [[deprecated("Use pressChar(o) instead.")]]
        void o(int holdTime = 0);
        [[deprecated("Use pressChar(p) instead.")]]
        void p(int holdTime = 0);
        [[deprecated("Use pressChar(q) instead.")]]
        void q(int holdTime = 0);
        [[deprecated("Use pressChar(r) instead.")]]
        void r(int holdTime = 0);
        [[deprecated("Use pressChar(s) instead.")]]
        void s(int holdTime = 0);
        [[deprecated("Use pressChar(t) instead.")]]
        void t(int holdTime = 0);
        [[deprecated("Use pressChar(u) instead.")]]
        void u(int holdTime = 0);
        [[deprecated("Use pressChar(v) instead.")]]
        void v(int holdTime = 0);
        [[deprecated("Use pressChar(w) instead.")]]
        void w(int holdTime = 0);
        [[deprecated("Use pressChar(x) instead.")]]
        void x(int holdTime = 0);
        [[deprecated("Use pressChar(y) instead.")]]
        void y(int holdTime = 0);
        [[deprecated("Use pressChar(z) instead.")]]
        void z(int holdTime = 0);
        
        void enter();
        void numlock();
        void alt0248();
        void alt136();
        void paste();
        void backslash();
        void slash();
        void leftclick(bool verboseMode = false);
        void rightclick(bool verboseMode = false);
    

        /**
         * Functions relating to Minecraft. // @TODO - move to an app
         * This can stay here for now since it has no internal dependancies. 
         */
        void minecraftDig(int time);
    
        // Other useful functions.
        void findMouseCoords(int waitTime);
        
        /**
         * @brief Scans and prints RGB color values of pixels in a 100x100 grid offset from the mouse cursor.
         *
         * This function captures the screen and iterates over a grid region starting from the mouse cursor’s
         * position, sampling every 4 pixels horizontally and vertically. For each sampled point, it retrieves
         * the RGB values and prints the coordinates and corresponding color values.
         * 
         * Useful for scanning a region rather than a single pixel.
         */
        void getPixelColor();
        
        /**
         * @brief Retrieves and prints the RGB color values of the pixel under the mouse cursor.
         *
         * This function uses the Windows API to get the current position of the mouse cursor,
         * reads the color of the pixel at that position from the screen, and then prints the
         * RGB components along with the cursor's coordinates to the standard output.
         * It uses `GetCursorPos` to retrieve the cursor location, `GetPixel` to obtain the pixel
         * color, and `getRGB` to extract the red, green, and blue components.
         */
        void getPixelColorAtMouse();
        void moveMouseTo(int x, int y);
    
    #endif
    
        void minus(bool verboseMode = false);       ///< Simulates press of the minus key.
        void equal(bool verboseMode = false);       ///< Simulates press of the equal key.
        void space(bool verboseMode = false);       ///< Simulates press of the space key.
        void tab(bool verboseMode = false);         ///< Simulates press of the tab key.
    
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
} // namespace MIA_system
