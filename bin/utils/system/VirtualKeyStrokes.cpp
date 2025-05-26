/**
 * File: VirtualKeyStrokes.cpp
 * Author: Antonius Torode
 * Created on: 03/01/2021
 * Description: Implements the VirtualKeyStrokes class for simulating virtual keyboard and 
 *     mouse input across platforms. Includes support for common key presses, 
 *     string typing, mouse actions, and platform-specific automation routines.
 */

// @TODO - this file needs some work.

// Include the associated header file.
#include "VirtualKeyStrokes.hpp"
// Include Timing for timing values.
#include "Timing.hpp"
// Include for error handling.
#include "Error.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#pragma comment (lib, "gdi32.lib")
#endif

using std::cout;
using std::endl;
using std::string;

namespace MIA_system
{
    VirtualKeyStrokes::VirtualKeyStrokes()
    {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    	// Set up a generic keyboard event.
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0; // hardware scan code for key
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
    #elif __linux__
        xdo = xdo_new(":1.0");
    #endif
    }
    
    VirtualKeyStrokes::~VirtualKeyStrokes()
    {
    #if __linux__
        delete xdo;
    #endif
    }
    
    void VirtualKeyStrokes::press(const char& character, int holdTime, bool verboseMode)
    {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
        if (std::isdigit(character)) 
        {
            pressNumber(character - '0', holdTime, verboseMode);
        } 
        else if(std::isalpha(static_cast<unsigned char>(character)))
        {        
            pressCharacter(character, holdTime, verboseMode);
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
            error::returnError(31424, std::to_string(character));
        }
    #elif __linux__
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
            error::returnError(31424, std::to_string(character));
        }
        if(!skipHold)
            MIA_system::sleepMilliseconds(holdTime);
    #endif
    }
    
    
    void VirtualKeyStrokes::defaultSleep() const
    {
        MIA_system::sleepMilliseconds(globalSleep);
    }
    
    
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    
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
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << num << endl;
    
        // Release the "num" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::one(int holdTime, bool verboseMode){
        // Press the "1" key
        ip.ki.wVk = 0x31; // virtual-key code for the "1" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "1" << endl;
    
        // Release the "1" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::two(int holdTime, bool verboseMode){
        // Press the "2" key
        ip.ki.wVk = 0x32; // virtual-key code for the "2" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "2" << endl;
    
        // Release the "2" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::three(int holdTime, bool verboseMode){
        // Press the "3" key
        ip.ki.wVk = 0x33; // virtual-key code for the "3" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "3" << endl;
    
        // Release the "3" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::four(int holdTime, bool verboseMode){
        // Press the "4" key
        ip.ki.wVk = 0x34; // virtual-key code for the "4" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "4" << endl;
    
        // Release the "4" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::five(int holdTime, bool verboseMode){
        // Press the "5" key
        ip.ki.wVk = 0x35; // virtual-key code for the "5" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "5" << endl;
    
        // Release the "5" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::six(int holdTime, bool verboseMode){
        // Press the "6" key
        ip.ki.wVk = 0x36; // virtual-key code for the "6" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "6" << endl;
    
        // Release the "6" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::seven(int holdTime, bool verboseMode){
        // Press the "7" key
        ip.ki.wVk = 0x37; // virtual-key code for the "7" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "7" << endl;
    
        // Release the "7" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::eight(int holdTime, bool verboseMode){
        // Press the "8" key
        ip.ki.wVk = 0x38; // virtual-key code for the "8" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "8" << endl;
    
        // Release the "8" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::nine(int holdTime, bool verboseMode){
        // Press the "9" key
        ip.ki.wVk = 0x39; // virtual-key code for the "9" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "9" << endl;
    
        // Release the "9" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    }
    
    void VirtualKeyStrokes::zero(int holdTime, bool verboseMode){
        // Press the "0" key
        ip.ki.wVk = 0x30; // virtual-key code for the "0" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "0" << endl;
    
        // Release the "0" key
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

        MIA_system::sleepMilliseconds(holdTime);
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
    
    void VirtualKeyStrokes::a(int holdTime, bool verboseMode){
        // Press the "a" key
        ip.ki.wVk = 0x41; // virtual-key code for the "a" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "a" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::b(int holdTime){
        // Press the "b" key
        ip.ki.wVk = 0x42; // virtual-key code for the "b" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "b" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::c(int holdTime){
        // Press the "c" key
        ip.ki.wVk = 0x43; // virtual-key code for the "c" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "c" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::d(int holdTime){
        // Press the "d" key
        ip.ki.wVk = 0x44; // virtual-key code for the "d" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "d" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::e(int holdTime){
        // Press the "e" key
        ip.ki.wVk = 0x45; // virtual-key code for the "e" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "e" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::f(int holdTime){
        // Press the "f" key
        ip.ki.wVk = 0x46; // virtual-key code for the "f" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "f" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::g(int holdTime){
        // Press the "g" key
        ip.ki.wVk = 0x47; // virtual-key code for the "g" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "g" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::h(int holdTime){
        // Press the "h" key
        ip.ki.wVk = 0x48; // virtual-key code for the "h" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "h" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::i(int holdTime){
        // Press the "i" key
        ip.ki.wVk = 0x49; // virtual-key code for the "i" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "i" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::j(int holdTime){
        // Press the "j" key
        ip.ki.wVk = 0x4A; // virtual-key code for the "j" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "j" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::k(int holdTime){
        // Press the "k" key
        ip.ki.wVk = 0x4B; // virtual-key code for the "k" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "k" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::l(int holdTime){
        // Press the "l" key
        ip.ki.wVk = 0x4C; // virtual-key code for the "l" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "l" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::m(int holdTime){
        // Press the "m" key
        ip.ki.wVk = 0x4D; // virtual-key code for the "m" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "m" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::n(int holdTime){
        // Press the "n" key
        ip.ki.wVk = 0x4E; // virtual-key code for the "n" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "n" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::o(int holdTime){
        // Press the "o" key
        ip.ki.wVk = 0x4F; // virtual-key code for the "o" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "o" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::p(int holdTime){
        // Press the "p" key
        ip.ki.wVk = 0x50; // virtual-key code for the "p" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "p" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::q(int holdTime){
        // Press the "q" key
        ip.ki.wVk = 0x51; // virtual-key code for the "q" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "q" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::r(int holdTime){
        // Press the "r" key
        ip.ki.wVk = 0x52; // virtual-key code for the "r" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "r" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::s(int holdTime){
        // Press the "s" key
        ip.ki.wVk = 0x53; // virtual-key code for the "s" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "s" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::t(int holdTime){
        // Press the "t" key
        ip.ki.wVk = 0x54; // virtual-key code for the "t" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "t" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::u(int holdTime){
        // Press the "u" key
        ip.ki.wVk = 0x55; // virtual-key code for the "u" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "u" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::v(int holdTime){
        // Press the "v" key
        ip.ki.wVk = 0x56; // virtual-key code for the "v" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "v" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::w(int holdTime){
        // Press the "w" key
        ip.ki.wVk = 0x57; // virtual-key code for the "w" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "w" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::x(int holdTime){
        // Press the "x" key
        ip.ki.wVk = 0x58; // virtual-key code for the "x" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "x" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::y(int holdTime){
        // Press the "y" key
        ip.ki.wVk = 0x59; // virtual-key code for the "y" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "y" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    void VirtualKeyStrokes::z(int holdTime){
        // Press the "z" key
        ip.ki.wVk = 0x5A; // virtual-key code for the "z" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
    
        // Release the "z" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    //In dev.
    void VirtualKeyStrokes::alt0248(){
        // Press the "alt" key
        ip.ki.wVk = VK_LMENU; // virtual-key code for the "alt" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        zero();
        two();
        four();
        eight();
    
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
    
        one();
        three();
        six();
    
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
    
        v();
    
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
    
    //performs a sequence to perpetually dig as you would in minecraft.
    void VirtualKeyStrokes::minecraftDig(int time)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); //Waits 5 seconds before beginning.
    
        // Set up a generic keyboard event.
        ip.type = INPUT_KEYBOARD;
        ip.ki.wScan = 0; // hardware scan code for key
        ip.ki.time = 0;
        ip.ki.dwExtraInfo = 0;
    
        ip.ki.wVk = 0x57; // virtual-key code for the "w" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        INPUT ip2;
        ip2.type = INPUT_MOUSE;
        ip2.mi.dwFlags = MOUSEEVENTF_LEFTDOWN; // left down
        SendInput(1,&ip2,sizeof(INPUT));
    
        std::this_thread::sleep_for(std::chrono::milliseconds(time*1000));
    
        // left up
        ZeroMemory(&ip2,sizeof(INPUT));
        ip2.type = INPUT_MOUSE;
        ip2.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1,&ip2,sizeof(INPUT));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
        // Release the "w" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
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
    
        MIA_system::sleepMilliseconds(wait);
        GetCursorPos(&cursor);
    
        cout << ".." << endl;
        int x = cursor.x, y = cursor.y;
        cout << "The mouse curse is at: " << x << ", " << y << endl;
        cout << ".." << endl;
    }
    
    // Prints the pixel color at a scan of ranges in a 100 x 100 grid from the mouse location.
    void VirtualKeyStrokes::getPixelColor()
    {
        POINT cursor;
        GetCursorPos(&cursor);
    
        HDC dc = GetDC(NULL);
        COLORREF color;
        int red=0,green=0,blue=0;
    
        cout << "...Scanning." << endl;
    
        for (int i=cursor.x;i<cursor.x+100;i+=4)
        {
            for (int j=cursor.y+2;j<cursor.y+100;j+=4)
            {
                color = GetPixel(dc, i, j);
    
                getRGB(color, red, green, blue);
    
                cout << "(x,y): " << "(" << i << "," << j << ")" << endl;
                cout << "Red: " << red << "  --  " << "Green: " << green << "  --  " << "Blue: " << blue << endl;
            }
        }
        ReleaseDC(NULL, dc);
    
        cout << "...Finished." << endl;
    }
    
    // Prints the pixel color at the cursor location.
    void VirtualKeyStrokes::getPixelColorAtMouse()
    {
        POINT cursor;
        GetCursorPos(&cursor);
    
        HDC dc = GetDC(NULL);
    
        cout << "...Scanning." << endl;
    
        COLORREF color = GetPixel(dc, cursor.x, cursor.y);
    
        int red = 0, green = 0, blue = 0;
        getRGB(color, red, green, blue);
    
        cout << "(x,y): " << "(" << cursor.x << "," << cursor.y << ")" << endl;
        cout << "Red: " << red << "  --  " << "Green: " << green << "  --  " << "Blue: " << blue << endl;
        cout << "RGB: (" << red << "," << green << "," << blue << ")" << endl;
    
        ReleaseDC(NULL, dc);
    
        cout << "...Finished." << endl;
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
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
        INPUT Input;
        // left down
        Input.type = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        SendInput(1,&Input,sizeof(INPUT));
    
        if(verboseMode)
            cout << "LEFT CLICK" << endl;
    
        // left up
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type  = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
        SendInput(1,&Input,sizeof(INPUT));
        std::this_thread::sleep_for(std::chrono::milliseconds(2*globalSleep));
#endif
    }
    
    void VirtualKeyStrokes::rightclick(bool verboseMode)
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
        INPUT Input;
        // right down
        Input.type = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
        SendInput(1,&Input,sizeof(INPUT));
    
        if(verboseMode)
            cout << "RIGHT CLICK" << endl;
    
        // right up
        ZeroMemory(&Input,sizeof(INPUT));
        Input.type = INPUT_MOUSE;
        Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
        SendInput(1,&Input,sizeof(INPUT));
        std::this_thread::sleep_for(std::chrono::milliseconds(2*globalSleep));
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
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
        SetCursorPos(x,y);
        MIA_system::sleepMilliseconds(40);
#endif
    }
    
    void VirtualKeyStrokes::minus(bool verboseMode)
    {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    
        // Press the "-" key
        ip.ki.wVk = VK_OEM_MINUS; // virtual-key code for the "-" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        if(verboseMode)
            cout << "-" << endl;
    
        // Release the "-" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    #elif __linux__
        xdo_send_keysequence_window(xdo, CURRENTWINDOW, "minus", 0);
    #endif
    }
    
    void VirtualKeyStrokes::equal(bool verboseMode)
    {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    
        // Press the "=" key
        ip.ki.wVk = VK_OEM_PLUS; // virtual-key code for the "=" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        if(verboseMode)
            cout << "=" << endl;
    
        // Release the "=" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    #elif __linux__
        xdo_send_keysequence_window(xdo, CURRENTWINDOW, "equal", 0);
    #endif
    }
    
    void VirtualKeyStrokes::space(bool verboseMode)
    {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    
        // Press the "space" key
        ip.ki.wVk = VK_SPACE; // virtual-key code for the "space" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        if(verboseMode)
            cout << "space" << endl;
    
        // Release the "space" key
        ip.ki.wVk = VK_SPACE; // virtual-key code for the "space" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    #elif __linux__
        xdo_send_keysequence_window(xdo, CURRENTWINDOW, "space", 0);
    #endif
    }
    
    void VirtualKeyStrokes::tab(bool verboseMode)
    {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    
        // Press the "tab" key
        ip.ki.wVk = VK_TAB; // virtual-key code for the "tab" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        if(verboseMode)
            cout << "TAB" << endl;
    
        // Release the "tab" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
        defaultSleep();
    #elif __linux__
        xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Tab", 0);
    #endif
    }
    
    void VirtualKeyStrokes::buttonSpam(const std::string& button, int amount, int pause)
    {
        MIA_system::sleepMilliseconds(5); //Waits 5 seconds before beginning.
    
        for (int i=0;i<amount;i++)
        {
            type(button);
            MIA_system::sleepMilliseconds(pause);
        }
    }
    
    void VirtualKeyStrokes::buttonSpamTab(const std::string& button, int amount, int pause)
    {
        MIA_system::sleepMilliseconds(5); //Waits 5 seconds before beginning.
    
        for (int i=0;i<amount;i++)
        {
            type(button);
            defaultSleep();
            tab();
            MIA_system::sleepMilliseconds(pause);
        }
    }
    
    void VirtualKeyStrokes::type(std::string word)
    {
        int size = word.size();
        for(int i=0;i<size;i++)
        {
            char letter(word[i]);
            press(letter);
            defaultSleep();
        }
    }
    
    
    VirtualKeyStrokes::ClickType VirtualKeyStrokes::stringToClickType(std::string input)
    {
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
} // namespace MIA_system
