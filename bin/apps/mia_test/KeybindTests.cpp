/**
 * @file KeybindTests.cpp
 * @author Antonius Torode
 * @date 09/16/2026
 * @brief Implements the keybind listener test methods.
 */

#include <iostream>
#include <set>
#include <string>
#include <vector>

// The associated header file.
#include "KeybindTests.hpp"
// The listener under test.
#include "KeybindListener.hpp"
// The event queue read by this test.
#include "EventQueue.hpp"
// Used for the sleep between hold reports.
#include "Timing.hpp"
// Used for the RuntimeContext carrying the verbose flag.
#include "RuntimeContext.hpp"
// Used for the return codes.
#include "Constants.hpp"


namespace
{
    /// How often a held movement binding re-prints, in milliseconds.
    constexpr int HOLD_REPORT_INTERVAL_MS = 50;

    /// The bindings which report continuously while held.
    const std::set<char> MOVEMENT_KEYS = {'w', 'a', 's', 'd'};

    /**
     * Gets the movement bindings currently held, formatted for printing.
     *
     * @param listener The listener whose held state is read.
     * @return The held movement key labels, e.g. "w+a" when both are held.
     */
    std::string heldMovementString(const ui::KeybindListener& listener)
    {
        std::string held;

        for (char key : MOVEMENT_KEYS)
        {
            if (listener.isKeyHeld(key))
            {
                if (!held.empty())
                    held += "+";
                held += key;
            }
        }

        return held;
    }
}


/**
 * Runs the keybind listener loop with the given capture mode.
 *
 * Loads the keybinds from the MIATest.MIA config, starts a KeybindListener,
 * and prints each completed binding once and each held movement key roughly
 * every 50 milliseconds until interrupted.
 *
 * @param verboseMode Enables verbose listener output when true.
 * @param captureGlobally Whether the listener captures keys globally.
 * @return SUCCESS when the listener loop completes, FAILURE otherwise.
 */
int runKeybindListenerTest(bool verboseMode, bool captureGlobally)
{
    // Load the keybinds from the shared MIATest config.
    config::MIAConfig config("MIATest.MIA", constants::ConfigType::KEY_VALUE, verboseMode);
    config.initialize(verboseMode);

    // The queue the listener pushes events into and this test reads from.
    ui::EventQueue queue;

    // A local context carries the verbose flag into the listener's output.
    RuntimeContext context;
    context.verboseMode = verboseMode;

    ui::KeybindListener listener(config, queue, "keybind_", 5, captureGlobally);
    listener.setContext(context);
    listener.initialize();
    listener.start();

    std::cout << "Listening for keybinds (global capture "
              << (captureGlobally ? "on" : "off") << "): wasd = movement"
              << " (held keys report every " << HOLD_REPORT_INTERVAL_MS
              << "ms), g/h/j = pushes, alt+t = alt push, alt+shift+s ="
              << " double-modifier push."
              << (captureGlobally ? "" : " Keys also reach the focused window.")
              << " Ctrl+C to quit." << std::endl;

    while (listener.isRunning())
    {
        // Report every completed binding once.
        ui::EventStorage storage = queue.getAllEvents();
        for (const ui::Event& event : storage.getEvents())
            std::cout << "PUSH: " << event.getName() << std::endl;

        // Report the movement keys currently held while they stay held.
        std::string held = heldMovementString(listener);
        if (!held.empty())
            std::cout << "HELD: " << held << std::endl;

        timing::sleepMilliseconds(HOLD_REPORT_INTERVAL_MS);
    }

    listener.stop();
    return constants::SUCCESS;
}


int testKeybindListener(bool verboseMode)
{
    return runKeybindListenerTest(verboseMode, false);
}


int testKeybindListenerGlobalCapture(bool verboseMode)
{
    return runKeybindListenerTest(verboseMode, true);
}
