/**
 * @file KeybindListener.cpp
 * @author Antonius Torode
 * @date 09/15/2026
 * @brief Implements a background listener which watches keys and key combinations and pushes events into a queue.
 */

// The associated header file.
#include "KeybindListener.hpp"

// Used for the pause between polls.
#include "Timing.hpp"
// Used for lowercasing modifier tokens.
#include "StringUtils.hpp"
// Used for the MIAException thrown when config pairs cannot be read.
#include "MIAException.hpp"

#if defined(IS_WINDOWS)
    #include <windows.h>
	#include <CharUtils.hpp>
#elif defined(IS_LINUX)
    #include <X11/keysym.h>
    // Used for XkbSetDetectableAutoRepeat, which filters keyboard auto-repeat.
    #include <X11/XKBlib.h>
#endif

#include <algorithm>
#include <iostream>
#include <mutex>
#include <vector>


namespace ui
{
    namespace
    {
        /**
         * Splits a binding token string on '+'.
         *
         * @param combo The token string, e.g. "alt+t" or "a+b".
         * @return The tokens, in order, or an empty vector if the string is
         *         not a well-formed token list.
         */
        std::vector<std::string> splitTokens(const std::string& combo,
                                             bool verboseMode = false)
        {
            std::vector<std::string> tokens = 
                string_utils::delimiterString(combo, "+", verboseMode);

            for (const std::string& token : tokens)
            { // Not well-formed strings will contain an empty element.
                if (token.empty())
                    return {};
            }

            return tokens;
        }
		
#if defined(IS_LINUX)
        /*
         * The lock masks (NumLock, CapsLock, ScrollLock) are included in every
         * grab's modifier set. X matches grabs against the exact modifier
         * state, so without these variants a binding stops matching while a
         * lock key is on.
         */
        constexpr unsigned int LOCK_MASKS = LockMask | Mod2Mask | Mod5Mask;

        /**
         * Converts a Modifier bit mask into the X11 modifier mask to grab with.
         * The two encodings do not share bit values, so the flags are mapped
         * one by one.
         *
         * @param modifier The required modifiers as Modifier bits.
         * @return The X11 mask covering all of the modifiers.
         */
        unsigned int x11MaskFromModifiers(Modifier modifier)
        {
            unsigned int mask = 0;
            unsigned int bits = static_cast<unsigned int>(modifier);

            if (bits & static_cast<unsigned int>(Modifier::ALT))
                mask |= Mod1Mask;
            if (bits & static_cast<unsigned int>(Modifier::CTRL))
                mask |= ControlMask;
            if (bits & static_cast<unsigned int>(Modifier::SHIFT))
                mask |= ShiftMask;

            return mask;
        }

        /// Set when an X11 error occurs during a grab-and-sync sequence.
        bool grabFailed = false;

        /**
         * A temporary error handler which records grab failures instead of
         * terminating the process. Installed around the grab calls; XGrabKey
         * reports errors like BadAccess asynchronously, so they only surface
         * through the handler after an XSync.
         */
        int grabErrorHandler(Display*, XErrorEvent*)
        {
            grabFailed = true;
            return 0;
        }
#endif
    }

    KeybindListener::KeybindListener(const config::MIAConfig& config,
                                     ui::EventQueue& queue,
                                     const std::string& keybindPrefix,
                                     int pollIntervalMilliseconds,
                                     bool captureGloballyEnabled) : 
        eventQueue(queue),
        pollIntervalMs(pollIntervalMilliseconds),
        captureGlobally(captureGloballyEnabled)
    {
#if defined(IS_LINUX)
        display = XOpenDisplay(nullptr);
        root = display ? DefaultRootWindow(display) : 0;
#endif
        std::vector<constants::KeyValuePair> pairs = config.getAllConfigPairs();

        for (const constants::KeyValuePair& pair : pairs)
        {
            if (pair.first.rfind(keybindPrefix, 0) != 0)
                continue;

            // The binding's tokens are whatever follows the prefix, joined by '+'.
            std::vector<std::string> tokens = 
                splitTokens(pair.first.substr(keybindPrefix.size()));
            if (tokens.empty())
                continue;

            Keybind keybind;
            bool valid = true;

            for (const std::string& token : tokens)
            {
                Modifier modifier = modifierFromToken(token);
                if (modifier != Modifier::NONE)
                {
                    keybind.modifier = static_cast<Modifier>(
                        static_cast<unsigned int>(keybind.modifier) | 
                        static_cast<unsigned int>(modifier));
                }
                else if (token.size() == 1)
                {
                    keybind.keys.insert(token[0]);
                }
                else
                {
                    valid = false;
                }
            }

            if (!valid || keybind.keys.empty())
                continue;

            keybinds.emplace(std::move(keybind), Event(pair.second));
        }

        for (const auto& [keybind, event] : keybinds)
            watchedKeys.insert(keybind.keys.begin(), keybind.keys.end());
    }


    KeybindListener::KeybindListener(const KeybindEventMap& eventMappings,
                                     ui::EventQueue& queue,
                                     int pollIntervalMilliseconds,
                                     bool captureGloballyEnabled) :
        keybinds(eventMappings),
        eventQueue(queue),
        pollIntervalMs(pollIntervalMilliseconds),
        captureGlobally(captureGloballyEnabled)
    {
#if defined(IS_LINUX)
        display = XOpenDisplay(nullptr);
        root = display ? DefaultRootWindow(display) : 0;
#endif
        for (const auto& [keybind, event] : keybinds)
            watchedKeys.insert(keybind.keys.begin(), keybind.keys.end());
    }


    KeybindListener::~KeybindListener()
    {
#if defined(IS_LINUX)
        if (grabbed)
        {
            for (const auto& [keybind, event] : keybinds)
            {
                unsigned int mask = x11MaskFromModifiers(keybind.modifier);

                for (char key : keybind.keys)
                {
                    unsigned int keycode = charToKeyCode(key);
                    if (keycode != 0)
                        XUngrabKey(display, keycode, mask, root);
                }
            }
        }

        if (display)
            XCloseDisplay(display);
#endif
    }



    bool KeybindListener::isActive() const
    {
        return !keybinds.empty();
    }


    std::set<char> KeybindListener::getHeldKeys() const
    {
#if defined(IS_WINDOWS)
        std::set<char> heldKeys;

        for (char key : watchedKeys)
        {
            // Bit 15 of GetAsyncKeyState's result is set while the key is down.
            if ((GetAsyncKeyState(static_cast<unsigned int>(char_utils::toUpper(key))) & 0x8000) != 0)
                heldKeys.insert(key);
        }

        return heldKeys;
#elif defined(IS_LINUX)
        std::set<char> heldKeys;

        std::lock_guard<std::mutex> lock(heldKeysMutex);
        for (unsigned int keycode : heldKeycodes)
        {
            auto it = charByKeycode.find(keycode);
            if (it != charByKeycode.end())
                heldKeys.insert(it->second);
        }

        return heldKeys;
#endif
    }


    unsigned int KeybindListener::getHeldModifiers() const
    {
#if defined(IS_WINDOWS)
        unsigned int held = 0;

        if ((GetAsyncKeyState(VK_MENU) & 0x8000) != 0)
            held |= static_cast<unsigned int>(Modifier::ALT);
        if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0)
            held |= static_cast<unsigned int>(Modifier::CTRL);
        if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
            held |= static_cast<unsigned int>(Modifier::SHIFT);

        return held;
#elif defined(IS_LINUX)
        std::lock_guard<std::mutex> lock(heldKeysMutex);
        return heldModifiers;
#endif
    }


    bool KeybindListener::isKeyHeld(char key) const
    {
        std::set<char> heldKeys = getHeldKeys();
        return heldKeys.find(key) != heldKeys.end();
    }


    bool KeybindListener::isBindingHeld(const std::set<char>& keys, Modifier modifier) const
    {
        unsigned int heldModifiers = getHeldModifiers();
        unsigned int wanted = static_cast<unsigned int>(modifier);

        return (heldModifiers & wanted) == wanted
            && std::includes(getHeldKeys().begin(), getHeldKeys().end(), keys.begin(), keys.end());
    }


    void KeybindListener::initialize()
    {
        if (!isActive())
        {
            // @TODO - throw MIAException here.
            return;
        }

#if defined(IS_LINUX)
        if (!display || !root)
        {
            std::cerr << "ERROR: KeybindListener could not open a connection to the X server;"
                      << " no keys will be detected." << std::endl;
            return;
        }

        /*
         * Ask X to report only real presses and releases. Without this, the
         * keyboard's auto-repeat sends synthetic release/press pairs while a
         * key is held, which would re-fire bindings and desynchronize the
         * held-key state from the physical keyboard.
         */
        Bool detectable = False;
        XkbSetDetectableAutoRepeat(display, True, &detectable);
        if (!detectable)
        {
            std::cerr << "WARNING: The X server does not support detectable"
                      << " auto-repeat; held keys may repeat their events."
                      << std::endl;
        }

        // Every binding's keys must convert to keycodes for the state polling,
        // whether or not the keys are grabbed.
        std::size_t usableBindings = 0;

        for (const auto& [keybind, event] : keybinds)
        {
            std::map<unsigned int, char> keycodeChars;
            bool allConverted = true;

            for (char key : keybind.keys)
            {
                unsigned int keycode = charToKeyCode(key);
                if (keycode == 0)
                {
                    allConverted = false;
                    break;
                }
                keycodeChars.emplace(keycode, key);
            }

            if (!allConverted)
            {
                std::cerr << "WARNING: A key in binding " << event.getName()
                          << " has no X11 keycode; the binding is skipped."
                          << std::endl;
                continue;
            }

            // The X11 modifier mask this binding's grabs require.
            unsigned int mask = x11MaskFromModifiers(keybind.modifier);

            if (captureGlobally)
            {
                for (const auto& [keycode, key] : keycodeChars)
                {
                /*
                 * X matches grabs against the exact modifier state, so each
                 * keycode is grabbed for every combination of the binding's
                 * mask and the lock masks. The error handler records
                 * asynchronous grab failures, which the XSync flushes.
                 */
                for (unsigned int lockBits = 0; lockBits <= LOCK_MASKS; ++lockBits)
                {
                    if (lockBits & ~LOCK_MASKS)
                        continue;

                    grabFailed = false;
                    XErrorHandler previous = XSetErrorHandler(grabErrorHandler);
                    XGrabKey(display, keycode, mask | lockBits, root,
                             True, GrabModeAsync, GrabModeAsync);
                    XSync(display, False);
                    XSetErrorHandler(previous);

                    if (grabFailed)
                    {
                        std::cerr << "WARNING: X11 grab failed for keycode "
                                  << keycode << " (mask " << (mask | lockBits) << ");"
                                  << " the binding may be owned by another client."
                                  << std::endl;
                        break;
                    }
                }

                    charByKeycode.emplace(keycode, key);
                }
            }
            else
            {
                for (const auto& [keycode, key] : keycodeChars)
                    charByKeycode.emplace(keycode, key);
            }

            ++usableBindings;

            if (context != nullptr && context->verboseMode)
            {
                std::cout << "STATUS: "
                          << (captureGlobally ? "grabbed" : "watching")
                          << " binding '" << event.getName() << "' (keys:";
                for (const auto& [keycode, key] : keycodeChars)
                    std::cout << " " << key << "=" << keycode;
                std::cout << ", modifier mask " << mask << ")" << std::endl;
            }
        }

        if (usableBindings == 0)
        {
            std::cerr << "ERROR: KeybindListener found no usable keybindings;"
                      << " no keys will be detected." << std::endl;
            return;
        }

        if (context != nullptr && context->verboseMode)
        {
            std::cout << "KeybindListener prepared " << usableBindings
                      << " binding(s)"
                      << (captureGlobally ? " with global capture." : ".")
                      << std::endl;
        }

        // Map each modifier to its physical keycodes so reconcileHeldKeys()
        // can build the held-modifier mask from the keyboard state.
        for (auto modifierKeycodePair : {
                std::make_pair(static_cast<unsigned int>(Modifier::ALT), XK_Alt_L),
                std::make_pair(static_cast<unsigned int>(Modifier::ALT), XK_Alt_R),
                std::make_pair(static_cast<unsigned int>(Modifier::CTRL), XK_Control_L),
                std::make_pair(static_cast<unsigned int>(Modifier::CTRL), XK_Control_R),
                std::make_pair(static_cast<unsigned int>(Modifier::SHIFT), XK_Shift_L),
                std::make_pair(static_cast<unsigned int>(Modifier::SHIFT), XK_Shift_R)})
        {
            unsigned int keycode = XKeysymToKeycode(display, modifierKeycodePair.second);
            if (keycode != 0)
                modifierKeycodes[modifierKeycodePair.first].insert(keycode);
        }

        if (captureGlobally)
        {
            // The grabs deliver key events; select them so they drain cleanly.
            XSelectInput(display, root, KeyPressMask | KeyReleaseMask);
            XFlush(display);
            grabbed = true;
        }
#endif
    }


    void KeybindListener::stop()
    {
        /*
         * Close the queue before joining so any thread blocked in
         * waitForEvents() wakes and observes the shutdown instead of waiting
         * for an event which will never arrive.
         */
        eventQueue.close();

        BackgroundTask::stop();
    }


    unsigned int KeybindListener::charToKeyCode(char c)
    {
#if defined(IS_LINUX)
        char str[2] = {c, '\0'};
        KeySym keysym = XStringToKeysym(str);

        if (keysym == NoSymbol)
            return 0;

        return XKeysymToKeycode(display, keysym);
#else
        (void)c;
        return 0;
#endif
    }


    Modifier KeybindListener::modifierFromToken(const std::string& token)
    {
        // The comparison is case-insensitive so configs can write Alt, ALT, or alt.
        std::string lowered = string_utils::toLower(token);

        if (lowered == "alt")
            return Modifier::ALT;
        if (lowered == "ctrl" || lowered == "control")
            return Modifier::CTRL;
        if (lowered == "shift")
            return Modifier::SHIFT;

        return Modifier::NONE;
    }


#if defined(IS_LINUX)
    void KeybindListener::reconcileHeldKeys()
    {
        /*
         * The keyboard's physical state: one bit per key, 8 bits per byte.
         * Querying this each poll keeps the held state in sync with the real
         * keyboard even when a release event is lost, which would otherwise
         * leave a key stuck in the held state.
         */
        char keymap[32];
        XQueryKeymap(display, keymap);

        unsigned int physicalModifiers = 0;
        for (const auto& [modifier, keycodes] : modifierKeycodes)
        {
            for (unsigned int keycode : keycodes)
            {
                if (keymap[keycode / 8] & (1 << (keycode % 8)))
                {
                    physicalModifiers |= modifier;
                    break;
                }
            }
        }

        std::lock_guard<std::mutex> lock(heldKeysMutex);

        heldKeycodes.clear();
        for (const auto& [keycode, key] : charByKeycode)
        {
            bool isDown = keymap[keycode / 8] & (1 << (keycode % 8));
            if (isDown)
                heldKeycodes.insert(keycode);
        }

        heldModifiers = physicalModifiers;
    }
#endif


    void KeybindListener::checkCompletions(const std::set<char>& heldKeys,
                                           unsigned int heldModifiers,
                                           std::vector<Event>& pressedEvents)
    {
        for (const auto& [keybind, event] : keybinds)
        {
            unsigned int wanted = static_cast<unsigned int>(keybind.modifier);

            bool allHeld = (heldModifiers & wanted) == wanted
                && std::includes(heldKeys.begin(), heldKeys.end(),
                                 keybind.keys.begin(), keybind.keys.end());

            if (!allHeld)
            {
                /*
                 * A binding re-arms when one of its keys is released, so
                 * holding a binding down does not repeat its event.
                 */
                completedBindings.erase(keybind);
                continue;
            }

            if (completedBindings.insert(keybind).second)
                pressedEvents.emplace_back(event);
        }
    }


    void KeybindListener::run()
    {
        if (!isActive())
        {
            // @TODO - throw MIAException here.
            return;
        }

#if defined(IS_WINDOWS)
        std::set<char> heldKeys = getHeldKeys();
        unsigned int heldModifiers = getHeldModifiers();

        std::vector<Event> pressed;
        checkCompletions(heldKeys, heldModifiers, pressed);

        if (!pressed.empty())
            eventQueue.addEvents(pressed);

        timing::sleepMilliseconds(pollIntervalMs);

#elif defined(IS_LINUX)
        if (!display)
        {
            if (context != nullptr && context->verboseMode)
                std::cerr << "WARNING: KeybindListener run() has no X display connection."
                          << std::endl;
            return;
        }

        /*
         * The grabs still deliver key events, so the queue must be drained
         * even though the state below comes from the physical keyboard. The
         * MIA event queue is filled from the keyboard state regardless.
         */
        if (captureGlobally)
        {
            while (XPending(display))
            {
                XEvent xevent;
                XNextEvent(display, &xevent);
            }
        }

        /*
         * The held state is rebuilt from the physical keyboard each cycle and
         * completions are detected as state transitions, so the poll interval
         * bounds how long a fast tap can go unnoticed rather than whether a
         * key can get stuck held.
         */
        reconcileHeldKeys();

        std::set<char> heldKeys = getHeldKeys();
        unsigned int heldModifiers = getHeldModifiers();

        std::vector<Event> pressed;
        checkCompletions(heldKeys, heldModifiers, pressed);

        if (!pressed.empty())
            eventQueue.addEvents(pressed);

        timing::sleepMilliseconds(pollIntervalMs);
#endif
    }
} // namespace ui
