/**
 * File: VirtualKeyStrokes.cpp
 * Author: Antonius Torode
 * Created on: 03/01/2021
 * Description: Implements the VirtualKeyStrokes class for simulating virtual keyboard and 
 *     mouse input across platforms. Includes support for common key presses, 
 *     string typing, mouse actions, and platform-specific automation routines.
 */

// @TODO - this file needs some work.
#include <algorithm>

// Include the associated header file.
#include "VirtualKeyStrokes.hpp"
// Include Timing for timing values.
#include "Timing.hpp"
// Include for error handling.
#include "Error.hpp"
#include "MIAException.hpp"


#if defined(IS_WINDOWS)
    #pragma comment (lib, "gdi32.lib")
#elif defined(__linux__)
    #include <X11/extensions/XTest.h>
#endif

using std::string;

namespace virtual_keys
{
    VirtualKeyStrokes::VirtualKeyStrokes()
    {
    #if defined(IS_WINDOWS)
    	// Set up a generic keyboard event.
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0; // hardware scan code for key
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
    #elif defined(__linux__)
        xdo = xdo_new(":1.0");
        display = XOpenDisplay(nullptr);
    #endif
    }
    
    VirtualKeyStrokes::~VirtualKeyStrokes()
    {
    #if __linux__
        delete xdo;
        XCloseDisplay(display);
    #endif
    }
    
    std::string VirtualKeyStrokes::clickTypeToString(VirtualKeyStrokes::ClickType click) 
    {
        switch (click) 
        {
            case VirtualKeyStrokes::ClickType::LEFT_CLICK:
                return "LEFT_CLICK"; 
            case VirtualKeyStrokes::ClickType::RIGHT_CLICK:
                return "RIGHT_CLICK"; 
            case VirtualKeyStrokes::ClickType::MIDDLE_CLICK:
                return "MIDDLE_CLICK"; 
            default:
                return "UNKNOWN"; 
        }
    }
    
    void VirtualKeyStrokes::press(const char& character, int holdTime, bool verboseMode)
    {
    #if defined(IS_WINDOWS)
        if (std::isdigit(character)) 
        {
            pressNumber(character - '0', holdTime, verboseMode);
        } 
        else if(std::isalpha(static_cast<unsigned char>(character)))
        {        
            pressChar(character, holdTime, verboseMode);
        } 
        else if(character == ' '){
            space();
        } else if(character == '-'){
            minus();
        }  else if(character == '='){
            equal();
        } else if(character == 'L'){
            leftclick();
        } else if(character == 'R'){
            rightclick();
        } else if (character == '\\'){
            backslash();
        } else if (character == '/'){
            slash();
        } else if (character == 'E'){
            enter();
        } else if (character == 'T'){
            tab();
        } else if (character == 'N'){
            numlock();
        } else if (character == ' '){
            space();
        } else{
            std::string err = std::to_string(character);
            throw error::MIAException(error::ErrorCode::Invalid_Character_Input, err);
        }
    #elif defined(__linux__)
        bool skipHold = false;
        if (std::isdigit(static_cast<unsigned char>(character)) || 
            std::isalpha(static_cast<unsigned char>(character)) ) 
        {
            char keyStr[2] = { character, '\0' };
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, keyStr, 0);
        }
        else if (character == ' ') 
        {
            space();
        } 
        else if (character == '-') 
        {
            minus();
        } 
        else if (character == '=') 
        {
            equal();
    //    } else if (character == 'L') {
    //        leftclick();
    //    } else if (character == 'R') {
    //        rightclick();
    //    } else if (character == '\\') {
    //        backslash();
    //    } else if (character == '/') {
    //        slash();
        } else {
            skipHold = true;            
            std::string err = std::to_string(character);
            throw error::MIAException(error::ErrorCode::Invalid_Character_Input, err);
        }
        if(!skipHold)
            timing::sleepMilliseconds(holdTime);
    #endif
    }
    
    
    void VirtualKeyStrokes::defaultSleep() const
    {
        timing::sleepMilliseconds(globalSleep);
    }
    
    
    #if defined(IS_WINDOWS)
    
    void VirtualKeyStrokes::pressNumber(int num, int holdTime, bool verboseMode)  
    {
        // Press the "num" key (num must be an int number from 0 to 9.
        if ( num < 0 || num > 9 )
        {
            return;
        }
        
        int keyCode = 0x30 + num;
        
        ip.ki.wVk = keyCode; // virtual-key code for the "num" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        timing::sleepMilliseconds(holdTime);
        if(verboseMode)
            std::cout << num << std::endl;
    
        // Release the "num" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::pressChar(char ch, int holdTime, bool verboseMode)
    {
        if (!std::isalpha(static_cast<unsigned char>(ch)))
            return;

        bool useShift = std::isupper(static_cast<unsigned char>(ch));
        
        // Convert to uppercase and get ASCII code (VK_A–VK_Z correspond to 'A'–'Z' ASCII values 0x41–0x5A)
        int vkCode = std::toupper(static_cast<unsigned char>(ch)); // VK_A–VK_Z are 0x41–0x5A

        if (useShift)
        {
            ip.ki.wVk = VK_SHIFT;
            ip.ki.dwFlags = 0; // Press Shift
            SendInput(1, &ip, sizeof(INPUT));
        }

        ip.ki.wVk = vkCode;
        ip.ki.dwFlags = 0; // Press key
        SendInput(1, &ip, sizeof(INPUT));

        timing::sleepMilliseconds(holdTime);
        if (verboseMode)
            std::cout << ch << std::endl;

        ip.ki.dwFlags = KEYEVENTF_KEYUP; // Release key
        SendInput(1, &ip, sizeof(INPUT));

        if (useShift)
        {
            ip.ki.wVk = VK_SHIFT;
            ip.ki.dwFlags = KEYEVENTF_KEYUP; // Release Shift
            SendInput(1, &ip, sizeof(INPUT));
        }

        defaultSleep();
    }
    
    
    //In dev.
    void VirtualKeyStrokes::alt0248(){
        // Press the "alt" key
        ip.ki.wVk = VK_LMENU; // virtual-key code for the "alt" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        pressNumber(0);
        pressNumber(2);
        pressNumber(4);
        pressNumber(8);
    
        // Release the "alt" key
        ip.ki.wVk = VK_MENU; // virtual-key code for the "alt" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    //In dev.
    void VirtualKeyStrokes::alt136(){
        // Press the "alt" key
        ip.ki.wVk = VK_MENU; // virtual-key code for the "alt" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        pressNumber(1);
        pressNumber(3);
        pressNumber(6);
    
        // Release the "alt" key
        ip.ki.wVk = VK_MENU;
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::paste(){
        // Press the "ctrl" key
        ip.ki.wVk = VK_CONTROL; // virtual-key code for the "ctrl" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        pressChar('v');
    
        // Release the "ctrl" key
        ip.ki.wVk = VK_CONTROL; // virtual-key code for the "ctrl" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::numlock(){
        // Press the "y" key
        ip.ki.wVk = 0x90; // virtual-key code for the "y" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        // Release the "y" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    // Press the "ENTER" key
    void VirtualKeyStrokes::enter()
    {
        // Press the "ENTER" key
        ip.ki.wVk = 0x0D; // virtual-key code for the "ENTER" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        // Release the "ENTER" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    // Press the "\" key
    void VirtualKeyStrokes::backslash()
    {
        // Press the "\" key
        ip.ki.wVk = 0xE2; // virtual-key code for the "\" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        // Release the "/" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    // Press the "/" key
    void VirtualKeyStrokes::slash()
    {
        // Press the "/" key
        ip.ki.wVk = 0x6F; // virtual-key code for the "/" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        // Release the "/" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }    

    
    //Prints the current location of the mouse curser after some wait time.
    void VirtualKeyStrokes::findMouseCoords(int wait){
    
        POINT cursor;
    
        timing::sleepMilliseconds(wait);
        GetCursorPos(&cursor);
    
        std::cout << ".." << std::endl;
        int x = cursor.x, y = cursor.y;
        std::cout << "The mouse curse is at: " << x << ", " << y << std::endl;
        std::cout << ".." << std::endl;
    }
    
    // Prints the pixel color at a scan of ranges in a 100 x 100 grid from the mouse location.
    void VirtualKeyStrokes::getPixelColor()
    {
        POINT cursor;
        GetCursorPos(&cursor);
    
        HDC dc = GetDC(NULL);
        COLORREF color;
        int red=0,green=0,blue=0;
    
        std::cout << "...Scanning." << std::endl;
    
        for (int i=cursor.x;i<cursor.x+100;i+=4)
        {
            for (int j=cursor.y+2;j<cursor.y+100;j+=4)
            {
                color = GetPixel(dc, i, j);
    
                getRGB(color, red, green, blue);
    
                std::cout << "(x,y): " << "(" << i << "," << j << ")" << std::endl;
                std::cout << "Red: " << red << "  --  " << "Green: " << green << "  --  " << "Blue: " << blue << std::endl;
            }
        }
        ReleaseDC(NULL, dc);
    
        std::cout << "...Finished." << std::endl;
    }
    
    // Prints the pixel color at the cursor location.
    void VirtualKeyStrokes::getPixelColorAtMouse()
    {
        POINT cursor;
        GetCursorPos(&cursor);
    
        HDC dc = GetDC(NULL);
    
        std::cout << "...Scanning." << std::endl;
    
        COLORREF color = GetPixel(dc, cursor.x, cursor.y);
    
        int red = 0, green = 0, blue = 0;
        getRGB(color, red, green, blue);
    
        std::cout << "(x,y): " << "(" << cursor.x << "," << cursor.y << ")" << std::endl;
        std::cout << "Red: " << red << "  --  " << "Green: " << green << "  --  " << "Blue: " << blue << std::endl;
        std::cout << "RGB: (" << red << "," << green << "," << blue << ")" << std::endl;
    
        ReleaseDC(NULL, dc);
    
        std::cout << "...Finished." << std::endl;
    }
    
    void VirtualKeyStrokes::getRGB(COLORREF& color, int& r, int& g, int&b)
    {
        r = GetRValue(color);
        g = GetGValue(color);
        b = GetBValue(color);
    } 
    #endif   
    
    void VirtualKeyStrokes::leftclick(bool verboseMode)
    {
	#if defined(IS_WINDOWS)
        INPUT Input;
        // left down
        Input.type = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1,&Input,sizeof(INPUT));
    
        if(verboseMode)
            std::cout << "LEFT CLICK" << std::endl;
    
        // left up
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type  = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1,&Input,sizeof(INPUT));        
        timing::sleepMilliseconds(2*globalSleep);
    #elif defined(__linux__)
        // Press left button (Button1)
        XTestFakeButtonEvent(display, 1, True, CurrentTime);

        if (verboseMode)
            std::cout << "LEFT CLICK" << std::endl;

        // Release left button
        XTestFakeButtonEvent(display, 1, False, CurrentTime);
        XFlush(display);
        timing::sleepMilliseconds(2*globalSleep);
	#endif
    }
    
    void VirtualKeyStrokes::rightclick(bool verboseMode)
    {
	#if defined(IS_WINDOWS)
        INPUT Input;
        // right down
        Input.type = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        SendInput(1,&Input,sizeof(INPUT));
    
        if(verboseMode)
            std::cout << "RIGHT CLICK" << std::endl;
    
        // right up
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        SendInput(1,&Input,sizeof(INPUT));
        timing::sleepMilliseconds(2*globalSleep);
    #elif defined(__linux__)
        // Press right button (Button1)
        XTestFakeButtonEvent(display, 3, True, CurrentTime);

        if (verboseMode)
            std::cout << "RIGHT CLICK" << std::endl;

        // Release right button
        XTestFakeButtonEvent(display, 3, False, CurrentTime);
        XFlush(display);
        timing::sleepMilliseconds(2*globalSleep);
	#endif
    }
    
    void VirtualKeyStrokes::mouseClick(VirtualKeyStrokes::ClickType clickType, bool verboseMode)
    {
        switch(clickType)
        {
            case VirtualKeyStrokes::ClickType::LEFT_CLICK:
                leftclick(verboseMode);
                break;            
            case VirtualKeyStrokes::ClickType::RIGHT_CLICK:
                rightclick(verboseMode);
                break;
            default:
                break;
        }
    }
    
    void VirtualKeyStrokes::moveMouseTo(int x, int y)
    {
	#if defined(IS_WINDOWS)
        SetCursorPos(x,y);
        timing::sleepMilliseconds(40);
    #elif defined(__linux__)
        throw error::MIAException(error::ErrorCode::Windows_Only_Feature);
	#endif
    }
    
    void VirtualKeyStrokes::minus(bool verboseMode)
    {
    #if defined(IS_WINDOWS)
    
        // Press the "-" key
        ip.ki.wVk = VK_OEM_MINUS; // virtual-key code for the "-" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        if(verboseMode)
            std::cout << "-" << std::endl;
    
        // Release the "-" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    #elif defined(__linux__)
        xdo_send_keysequence_window(xdo, CURRENTWINDOW, "minus", 0);
    #endif
    }
    
    void VirtualKeyStrokes::equal(bool verboseMode)
    {
    #if defined(IS_WINDOWS)
    
        // Press the "=" key
        ip.ki.wVk = VK_OEM_PLUS; // virtual-key code for the "=" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        if(verboseMode)
            std::cout << "=" << std::endl;
    
        // Release the "=" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    #elif defined(__linux__)
        xdo_send_keysequence_window(xdo, CURRENTWINDOW, "equal", 0);
    #endif
    }
    
    void VirtualKeyStrokes::space(bool verboseMode)
    {
    #if defined(IS_WINDOWS)
    
        // Press the "space" key
        ip.ki.wVk = VK_SPACE; // virtual-key code for the "space" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        if(verboseMode)
            std::cout << "space" << std::endl;
    
        // Release the "space" key
        ip.ki.wVk = VK_SPACE; // virtual-key code for the "space" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    #elif defined(__linux__)
        xdo_send_keysequence_window(xdo, CURRENTWINDOW, "space", 0);
    #endif
    }
    
    void VirtualKeyStrokes::tab(bool verboseMode)
    {
    #if defined(IS_WINDOWS)
    
        // Press the "tab" key
        ip.ki.wVk = VK_TAB; // virtual-key code for the "tab" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        if(verboseMode)
            std::cout << "TAB" << std::endl;
    
        // Release the "tab" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    #elif defined(__linux__)
        xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Tab", 0);
    #endif
    }
    
    void VirtualKeyStrokes::buttonSpam(const std::string& button, int amount, int pause)
    {
        timing::sleepMilliseconds(5); //Waits 5 seconds before beginning.
    
        for (int i=0;i<amount;i++)
        {
            type(button);
            timing::sleepMilliseconds(pause);
        }
    }
    
    void VirtualKeyStrokes::buttonSpamTab(const std::string& button, int amount, int pause)
    {
        timing::sleepMilliseconds(5); //Waits 5 seconds before beginning.
    
        for (int i=0;i<amount;i++)
        {
            type(button);
            defaultSleep();
            tab();
            timing::sleepMilliseconds(pause);
        }
    }
    
    void VirtualKeyStrokes::type(const std::string& word)
    {
        int size = word.size();
        for(int i=0;i<size;i++)
        {
            char letter(word[i]);
            press(letter);
            defaultSleep();
        }
    }
    
    
    VirtualKeyStrokes::ClickType VirtualKeyStrokes::stringToClickType(const std::string& in)
    {
        std::string input = in;
        // Convert the string to lowercase.
        std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char c) { return std::tolower(c); });

        if (input == "right_click" || input == "rightclick")
            return ClickType::RIGHT_CLICK;
        else if (input == "left_click" || input == "leftclick")
            return ClickType::LEFT_CLICK;
        else if (input == "middle_click" || input == "middleclick")
            return ClickType::MIDDLE_CLICK;
        else
            return ClickType::UNKNOWN;
    }
} // namespace virtual_keys
