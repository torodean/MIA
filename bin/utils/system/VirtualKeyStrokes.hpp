/**
 * @file VirtualKeyStrokes.hpp
 * @author Antonius Torode
 * @date 03/01/2021
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

/// Used for preprocessor definitions.
#include "Constants.hpp"

#if defined(IS_WINDOWS)
    #include <windows.h>
#elif defined(__linux__)
    #include <X11/Xlib.h>
    extern "C" 
    {
    #include <xdo.h>
    }
#endif

namespace virtual_keys
{
    /**
     * This class is for sending virtual key strokes and simulating user input.
     * This class is cross platform.
     */
    class VirtualKeyStrokes
    {
    public:
    
        /**
         * @enum ClickType
         * @brief Represents the various mouse click types.
         * Used to identify the type of mouse click interaction.
         */
        enum class ClickType
        {
            UNKNOWN,      ///< An unknown click type.
            LEFT_CLICK,   ///< Represents a left click.
            RIGHT_CLICK,  ///< Represents a right click.
            MIDDLE_CLICK  ///< Represents a middle click.
        };
        
        /**
         * @brief Converts a ClickType enum value to its corresponding human-readable string.
         * @param clickType The ClickType to convert.
         * @return A string representation of the given ClickType.
         */
        static std::string clickTypeToString(ClickType clickType);

        /**
         * @brief Converts a string to the corresponding ClickType enum value.
         * @param input The string representing the click type.
         * @return The matching ClickType, or ClickType::UNKNOWN if no match is found.
         */
        static ClickType stringToClickType(const std::string& input);
        
        /**
         * @enum SpecialButton
         * @brief Represents the various 'special' button types (non-alpha-numeric). Typically,
         * these are buttons which can't be represented by a single character. 
         * Used to identify the type of button interaction for special cases.
         */
        enum class SpecialButton
        {
            UNKNOWN,      ///< An unknown button type.
            ENTER,
            TAB,
            SPACE,
            NUM_LOCK,
			SCROLL_DOWN,
			SCROLL_UP
        };
        
        /**
         * @brief Converts a SpecialButton enum value to its corresponding human-readable string.
         * @param specialButton The SpecialButton to convert.
         * @return A string representation of the given SpecialButton.
         */
        static std::string specialButtonToString(SpecialButton specialButton);

        /**
         * @brief Converts a string to the corresponding SpecialButton enum value.
         * @param input The string representing the click type.
         * @return The matching SpecialButton, or SpecialButton::UNKNOWN if no match is found.
         */
        static SpecialButton stringToSpecialButton(const std::string& input);        
    
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
         * @param holdTime [int] - A time to hold after the keypress in ms (default = 0).
         * @param verboseMode[bool] If true, prints the pressed number to standard output.
         * @throw [MIAException] - Throws if an unsupported character is entered.
         */
        void press(const char& character, int holdTime = 0, bool verboseMode = false);
    
        /**
         * This will simulate the key presses for a string.
         * @param character[const std::string&] - the string to simulate key presses for.
         * @param holdTime [int] - A time to hold each keypress in ms (default = 0).
         * @param verboseMode[bool] If true, prints the pressed number to standard output.
         */
        void type(const std::string& word, int holdTime = 0, bool verboseMode = false);
    
        /**
         * @brief Simulates a mouse click of the specified type.
         * @param clickType The type of mouse click to perform (e.g., LEFT_CLICK, RIGHT_CLICK, etc.).
         * @param holdTime [int] - A time to hold each click in ms (default = 0).
         * @param verboseMode If true, enables verbose output describing the action performed. Default is false.
         */
        void mouseClick(ClickType clickType, int holdTime = 0, bool verboseMode = false);        
        
        /**
         * @brief Simulates a mouse click of the specified type.
         * @param clickType The type of mouse click to perform (e.g., LEFT_CLICK, RIGHT_CLICK, etc.).
         * @param holdTime [int] - A time to hold each keypress in ms (default = 0).
         * @param verboseMode If true, enables verbose output describing the action performed. Default is false.
         */
        void pressSpecialButton(SpecialButton specialButton, int holdTime = 0, bool verboseMode = false);
    
        void leftclick(int holdTime = 0, bool verboseMode = false);        
        void rightclick(int holdTime = 0, bool verboseMode = false);
		
		/**
		 * Simulates a single mouse scroll wheel step upward
		 * (away from the user).
		 *
		 * @param verboseMode If true, prints the action to stdout.
		 */
		void scrollWheelUp(bool verboseMode);

		/**
		 * Simulates a single mouse scroll wheel step downward
		 * (toward the user).
		 *
		 * @param verboseMode If true, prints the action to stdout.
		 */
		void scrollWheelDown(bool verboseMode);
    
        void moveMouseTo(int x, int y);
    
        void minus(int holdTime = 0, bool verboseMode = false);       ///< Simulates press of the minus key.
        void equal(int holdTime = 0, bool verboseMode = false);       ///< Simulates press of the equal key.
        void space(int holdTime = 0, bool verboseMode = false);       ///< Simulates press of the space key.
        void tab(bool verboseMode = false);                           ///< Simulates press of the tab key.
        void enter(bool verboseMode = false);                         ///< Simulates press of the enter key.
        void numlock(bool verboseMode = false);                       ///< Simulates press of the numlock key.
        void backslash(int holdTime = 0, bool verboseMode = false);   ///< Simulates press of the backslash key.
        void slash(int holdTime = 0, bool verboseMode = false);       ///< Simulates press of the slash key.
    
        /// Sleeps for the default globalSleep time used by the VirtuakKeyStrokes class..
        void defaultSleep() const;
    	void sleep(int time);

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
        void pressChar(char ch, int holdTime = 0, bool verboseMode = false);
        
        void alt0248();
        void alt136();
        void paste();
    
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
        
    #if defined(IS_WINDOWS)
        void getRGB(COLORREF& color, int& r, int& g, int& b);
    #endif
    
    private:
    
        /// Global time to sleep between each key press (in ms).
        int globalSleep = 10;
    
    #if defined(IS_WINDOWS)
    
        INPUT ip;
        
    #elif defined(__linux__)
    
        /**
         * This pointer is used for the xdotool which can simulate key strokes on linux through X11.
         * Useful docs of the library and related files can be found here"
         * https://libxdo-d.dpldocs.info/xdo.xdo_send_keysequence_window.html
         * A list of keysyms that can be used are here:
         * https://wiki.linuxquestions.org/wiki/List_of_keysyms
         */
        xdo_t *xdo;
        
        /**
         * @brief Pointer to the X11 display connection used for simulating input events.
         *
         * Initialized with XOpenDisplay(nullptr) to connect to the default display.
         * Used internally for issuing fake mouse events via the XTest extension.
         */
        Display *display;
        
    #endif
    }; // class VirtualKeyStrokes
} // namespace virtual_keys
