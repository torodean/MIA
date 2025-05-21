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
#include "Configurator.hpp"

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
    
    void VirtualKeyStrokes::press(const char& character, int holdTime)
    {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
        if(character == '1'){
            one(holdTime);
        } else if(character == '2'){
            two(holdTime);
        } else if(character == '3'){
            three(holdTime);
        } else if(character == '4'){
            four(holdTime);
        } else if(character == '5'){
            five(holdTime);
        } else if(character == '6'){
            six(holdTime);
        } else if(character == '7'){
            seven(holdTime);
        } else if(character == '8'){
            eight(holdTime);
        } else if(character == '9'){
            nine(holdTime);
        } else if(character == '0'){
            zero(holdTime);
        } else if(character == 'a'){
            a(holdTime);
        } else if(character == 'b'){
            b(holdTime);
        } else if(character == 'c'){
            c(holdTime);
        } else if(character == 'd'){
            d(holdTime);
        } else if(character == 'e'){
            e(holdTime);
        } else if(character == 'f'){
            f(holdTime);
        } else if(character == 'g'){
            g(holdTime);
        } else if(character == 'h'){
            h(holdTime);
        } else if(character == 'i'){
            i(holdTime);
        } else if(character == 'j'){
            j(holdTime);
        } else if(character == 'k'){
            k(holdTime);
        } else if(character == 'l'){
            l(holdTime);
        } else if(character == 'm'){
            m(holdTime);
        } else if(character == 'n'){
            n(holdTime);
        } else if(character == 'o'){
            o(holdTime);
        } else if(character == 'p'){
            p(holdTime);
        } else if(character == 'q'){
            q(holdTime);
        } else if(character == 'r'){
            r(holdTime);
        } else if(character == 's'){
            s(holdTime);
        } else if(character == 't'){
            t(holdTime);
        } else if(character == 'u'){
            u(holdTime);
        } else if(character == 'v'){
            v(holdTime);
        } else if(character == 'w'){
            w(holdTime);
        } else if(character == 'x'){
            x(holdTime);
        } else if(character == 'y'){
            y(holdTime);
        } else if(character == 'z'){
            z(holdTime);
        } else if(character == ' '){
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
            Error::returnError(31424, std::to_string(character));
        }
    #elif __linux__
        bool skipHold = false;
        if (character == '1') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "1", 0);
        } else if (character == '2') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "2", 0);
        } else if (character == '3') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "3", 0);
        } else if (character == '4') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "4", 0);
        } else if (character == '5') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "5", 0);
        } else if (character == '6') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "6", 0);
        } else if (character == '7') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "7", 0);
        } else if (character == '8') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "8", 0);
        } else if (character == '9') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "9", 0);
        } else if (character == '0') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "0", 0);
        } else if (character == 'a') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "a", 0);
        } else if (character == 'b') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "b", 0);
        } else if (character == 'c') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "c", 0);
        } else if (character == 'd') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "d", 0);
        } else if (character == 'e') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "e", 0);
        } else if (character == 'f') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "f", 0);
        } else if (character == 'g') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "g", 0);
        } else if (character == 'h') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "h", 0);
        } else if (character == 'i') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "i", 0);
        } else if (character == 'j') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "j", 0);
        } else if (character == 'k') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "k", 0);
        } else if (character == 'l') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "l", 0);
        } else if (character == 'm') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "m", 0);
        } else if (character == 'n') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "n", 0);
        } else if (character == 'o') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "o", 0);
        } else if (character == 'p') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "p", 0);
        } else if (character == 'q') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "q", 0);
        } else if (character == 'r') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "r", 0);
        } else if (character == 's') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "s", 0);
        } else if (character == 't') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "t", 0);
        } else if (character == 'u') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "u", 0);
        } else if (character == 'v') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "v", 0);
        } else if (character == 'w') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "w", 0);
        } else if (character == 'x') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "x", 0);
        } else if (character == 'y') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "y", 0);
        } else if (character == 'z') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "z", 0);
        } else if (character == 'A') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "A", 0);
        } else if (character == 'B') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "B", 0);
        } else if (character == 'C') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "C", 0);
        } else if (character == 'D') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "D", 0);
        } else if (character == 'E') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "E", 0);
        } else if (character == 'F') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "F", 0);
        } else if (character == 'G') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "G", 0);
        } else if (character == 'H') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "H", 0);
        } else if (character == 'I') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "I", 0);
        } else if (character == 'J') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "J", 0);
        } else if (character == 'K') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "K", 0);
        } else if (character == 'L') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "L", 0);
        } else if (character == 'M') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "M", 0);
        } else if (character == 'N') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "N", 0);
        } else if (character == 'O') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "O", 0);
        } else if (character == 'P') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "P", 0);
        } else if (character == 'Q') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Q", 0);
        } else if (character == 'R') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "R", 0);
        } else if (character == 'S') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "S", 0);
        } else if (character == 'T') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "T", 0);
        } else if (character == 'U') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "U", 0);
        } else if (character == 'V') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "V", 0);
        } else if (character == 'W') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "W", 0);
        } else if (character == 'X') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "X", 0);
        } else if (character == 'Y') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Y", 0);
        } else if (character == 'Z') {
            xdo_send_keysequence_window(xdo, CURRENTWINDOW, "Z", 0);
        } else if (character == ' ') {
            space();
        } else if (character == '-') {
            minus();
        } else if (character == '=') {
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
            Error::returnError(31424, std::to_string(character));
        }
        if(!skipHold)
            MIA_system::sleepMilliseconds(holdTime);
    #endif

    
    void VirtualKeyStrokes::defaultSleep() const
    {
        MIA_system::sleepMilliseconds(globalSleep);
    }
    
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    void VirtualKeyStrokes::one(int holdTime, bool verboseMode){
        // Press the "1" key
        ip.ki.wVk = 0x31; // virtual-key code for the "1" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        MIA_system::sleepMilliseconds(holdTime);
        if(verboseMode)
            cout << "1" << endl;
    
        // Release the "1" key
        ip.ki.wVk = 0x31; // virtual-key code for the "1" key
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
        ip.ki.wVk = 0x33; // virtual-key code for the "3" key
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
        ip.ki.wVk = 0x36; // virtual-key code for the "6" key
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
    
    void VirtualKeyStrokes::leftclick(bool verboseMode){
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
    
    void VirtualKeyStrokes::rightclick(bool verboseMode){
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
    }
    
    //performs a sequence to perpetually dig as you would in minecraft.
    void VirtualKeyStrokes::minecraftDig(int time){
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
    void VirtualKeyStrokes::enter(){
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
    void VirtualKeyStrokes::backslash(){
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
    void VirtualKeyStrokes::slash(){
        // Press the "/" key
        ip.ki.wVk = 0x6F; // virtual-key code for the "/" key
        ip.ki.dwFlags = 0; // 0 for key press
        SendInput(1, &ip, sizeof(INPUT));
    
        // Release the "/" key
        ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
        SendInput(1, &ip, sizeof(INPUT));
    
        defaultSleep();
    }
    
    //Used for duplicating a letter in WoW. Useful for creating RP events.
    void VirtualKeyStrokes::duplicateLetter(int copies, string recipient)
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
    
    //Used for unloading letters from the mailbox in WoW. Useful for creating RP events.
    //Coordinates may need adapted based on screen resolutions, UI scales, etc..
    void VirtualKeyStrokes::unloadLetters(int copies)
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
    
    //Prints the pixel color at a scan of ranges in a 100 x 100 grid from the mouse location.
    void VirtualKeyStrokes::getPixelColor(){
        POINT cursor;
        GetCursorPos(&cursor);
    
        HDC dc = GetDC(NULL);
        COLORREF color;
        int red=0,green=0,blue=0;
    
        cout << "...Scanning." << endl;
    
        for (int i=cursor.x;i<cursor.x+100;i+=4){
            for (int j=cursor.y+2;j<cursor.y+100;j+=4){
                color = GetPixel(dc, i, j);
    
                getRGB(color, red, green, blue);
    
                cout << "(x,y): " << "(" << i << "," << j << ")" << endl;
                cout << "Red: " << red << "  --  " << "Green: " << green << "  --  " << "Blue: " << blue << endl;
            }
        }
        ReleaseDC(NULL,dc);
    
        cout << "...Finished." << endl;
    }
    
    //Prints the pixel color at the cursor location.
    void VirtualKeyStrokes::getPixelColorAtMouse(){
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
    
        ReleaseDC(NULL,dc);
    
        cout << "...Finished." << endl;
    }
    
    void VirtualKeyStrokes::moveMouseTo(int x, int y){
        SetCursorPos(x,y);
        MIA_system::sleepMilliseconds(40);
    }
    
    void VirtualKeyStrokes::fishBotIntro()
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
    
    void VirtualKeyStrokes::getRGB(COLORREF& color, int& r, int& g, int&b)
    {
        r = GetRValue(color);
        g = GetGValue(color);
        b = GetBValue(color);
    }
    
    //A fish bot made for WoW -- Not yet polished.
    void VirtualKeyStrokes::WoWFishBot(string fishButton, string lureButton)
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
    
        ReleaseDC(NULL,dc);
    }
    #endif
    
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
    
        for (int i=0;i<amount;i++){
            type(button);
            MIA_system::sleepMilliseconds(pause);
        }
    }
    
    void VirtualKeyStrokes::buttonSpamTab(const std::string& button, int amount, int pause)
    {
        MIA_system::sleepMilliseconds(5); //Waits 5 seconds before beginning.
    
        for (int i=0;i<amount;i++){
            type(button);
            defaultSleep();
            tab();
            MIA_system::sleepMilliseconds(pause);
        }
    }
    
    void VirtualKeyStrokes::type(std::string word)
    {
        int size = word.size();
        for(int i=0;i<size;i++){
            char letter(word[i]);
            press(letter);
            defaultSleep();
        }
    }
} // namespace MIA_system
