/**
 * @file SingleKeyListener.cpp
 * @author Antonius Torode
 * @date 06/25/2025
 * @brief Implements the SingleKeyListener methods.
 */

// Associated header file.
#include "SingleKeyListener.hpp"

#include <cstring>
#include <iostream>

// Used for sleeping.
#include "Timing.hpp"
//Used for exception and error handling.
#include "MIAException.hpp"
#include "Error.hpp"

#if defined(IS_WINDOWS)
    #include <windows.h>
#elif defined(__linux__)
    #include <X11/keysym.h>
#endif


SingleKeyListener::SingleKeyListener()
#if defined(__linux__)
    : display(XOpenDisplay(nullptr)),
      root(display ? DefaultRootWindow(display) : 0)
#endif
{ 
    setTaskName("SingleKeyListener");
}


SingleKeyListener::SingleKeyListener(char keyCode)
#if defined(IS_WINDOWS)
	: keyCode(std::toupper(keyCode))
#elif defined(__linux__)
	: keyCode(keyCode),
	  display(XOpenDisplay(nullptr)),
	  root(display ? DefaultRootWindow(display) : 0)
#endif
{
    setTaskName("SingleKeyListener");
    initialize();
}


SingleKeyListener::~SingleKeyListener()
{
#if defined(__linux__)
    if (grabbed) 
        XUngrabKey(display, linuxKeyCode, modifiers, root);
        
    if (display) 
        XCloseDisplay(display);
#endif
}


void SingleKeyListener::initialize()
{
	if (!isActive())
	{
		// setKeyCode() must be called before initialize.
		MIA_THROW(error::ErrorCode::Out_Of_Order_Method_Calls, 
		          "SingleKeyListener: setKeyCode() must be called before initialize()!");
		return;
	}

#if defined(__linux__)
    if (display && root) 
    {
        linuxKeyCode = charToKeyCode(keyCode); // Set the linux key code value.
        XGrabKey(display, linuxKeyCode, modifiers, root, True, GrabModeAsync, GrabModeAsync);
        XFlush(display);
        XSelectInput(display, root, KeyPressMask);
        grabbed = true;
    }
#endif
}


void SingleKeyListener::setKeyCode(char code)
{ 
#if defined(IS_WINDOWS)
	keyCode = std::toupper(static_cast<unsigned char>(code));
#elif defined(__linux__)
	keyCode = code;
#else	
	keyCode = code;
#endif
}


#if defined(__linux__)
unsigned int SingleKeyListener::charToKeyCode(char c) 
{
    char str[2] = {c, '\0'};
    KeySym keysym = XStringToKeysym(str);
    
    if (keysym == NoSymbol) 
        return 0;
        
    return XKeysymToKeycode(display, keysym);
}
#endif

void SingleKeyListener::run()
{
    if (!isActive())
    {
		// setKeyCode() must be called before run.
		MIA_THROW(error::ErrorCode::Out_Of_Order_Method_Calls, 
		          "SingleKeyListener: setKeyCode() must be called before run()!");
        return;
    }

#if defined(IS_WINDOWS)
    SHORT keyState = GetAsyncKeyState(static_cast<unsigned int>(keyCode));
    
    // GetAsyncKeyState() returns a SHORT (16-bit int) with two relevant bits:
    // Bit 15 (0x8000) — high-order bit — is 1 if the key is currently down.
    // Bit 0 (0x0001) — low-order bit — is 1 if the key was pressed since the last call.
    bool pressed = (keyState & 0x8000) != 0;

    // Detect rising edge: key pressed now, was not pressed previously
    if (pressed && !lastPressed) 
    {
        toggleCondition();
        //std::cout << "Key toggled state: " << isConditionMet() << std::endl;
    }
    lastPressed = pressed;
    
#elif defined(__linux__)
    if (!display) return;

    while (XPending(display)) 
    {
        XEvent event;
        XNextEvent(display, &event);
        if (event.type == KeyPress)
        {
            auto* keyEvent = reinterpret_cast<XKeyEvent*>(&event);
            KeySym sym = XLookupKeysym(keyEvent, 0);
            char* keyString = XKeysymToString(sym);            

            if (context->verboseMode)
            {
                std::cout << "Listening for keycode: " << linuxKeyCode << std::endl;

                std::cout << "Detected key press: keycode=" << keyEvent->keycode
                          << ", keysym=" << sym
                          << ", string=" << (keyString ? keyString : "unknown")
                          << std::endl;
            }
			
            if (keyEvent->keycode == linuxKeyCode) 
                toggleCondition();
        }
    }
    timing::sleepMilliseconds(5);
#endif
}

