/**
 * @file: KeyListenerTask.cpp
 * @author: Antonius Torode
 * @date: 06/25/2025
 * Description:
 *     TODO
 */

#include "KeyListenerTask.hpp"
/// Used for preprocessor definitions.
#include "Constants.hpp"
#include "Timing.hpp"

#if defined(IS_WINDOWS)
    #include <windows.h>
#elif defined(__linux__)
    #include <X11/keysym.h>
#endif


KeyListenerTask::KeyListenerTask()
#if defined(__linux__)
    : display(XOpenDisplay(nullptr))
    , root(display ? DefaultRootWindow(display) : 0)
#endif
{ }


KeyListenerTask::KeyListenerTask(unsigned int keyCode) : keyCode(keyCode)
#if defined(__linux__)
    , display(XOpenDisplay(nullptr))
    , root(display ? DefaultRootWindow(display) : 0)
#endif
{
    initialize();
}


KeyListenerTask::~KeyListenerTask()
{
#if defined(__linux__)
    if (grabbed) 
        XUngrabKey(display, keyCode, modifiers, root);
        
    if (display) 
        XCloseDisplay(display);
#endif
}


void KeyListenerTask::initialize()
{
if (!isActive())
{
    // TODO - throw MIAException here.
    return;
}

// TODO - remove this after testing.
//keyCode = XKeysymToKeycode(display, XK_1);

#if defined(__linux__)
    if (display && root) 
    {
        int grabResult = XGrabKey(display, keyCode, modifiers, root, True, GrabModeAsync, GrabModeAsync);
        XFlush(display);
        XSelectInput(display, root, KeyPressMask);
        grabbed = true;
        if (grabResult == 0) std::cout << "XGrabKey failed\n";
        else std::cout << "Key grab successful\n";
    }
#endif
}

int count = 0;
unsigned long counter = 0;

void KeyListenerTask::run()
{
    if (count %1000 == 0)
    {
        std::cout << "run(): " << counter << std::endl;
        counter++;   
    }
    count++;
    
    if (!isActive())
    {
        // TODO - throw MIAException here.
        return;
    }

#if defined(IS_WINDOWS)
    SHORT keyState = GetAsyncKeyState(keyCode);
    
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
            std::cout << "Listening for keycode: " << keyCode << std::endl;
            if (keyEvent->keycode == keyCode && (keyEvent->state & modifiers) == modifiers) 
                toggleCondition();
        }
    }
    timing::sleepMilliseconds(5);
#endif
}

