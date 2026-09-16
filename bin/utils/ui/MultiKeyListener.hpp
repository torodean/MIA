/**
 * @file MultiKeyListener.hpp
 * @author Antonius Torodr
 * @date 09/15/26
 * @brief A Listener for detecting keyboard input.
 */
#pragma once

#include <string>

// Used for creating a threaded task for listening.
#include "BackgroundTask.hpp"
// Used for preprocessor definitions.
#include "Constants.hpp"
// The event types passed between the listener and the reader.
#include "Event.hpp"
#include "EventStorage.hpp"

#if defined(__linux__)
    #include <X11/Xlib.h>
#endif

namespace ui
{
    /**
     * Storage for mapping a keyboard button to a specific event.
     */
     using KeybindEventMap = std::map<char, ui::Event>;
    
    /**
     * This provides a utility for listening to keyboard events and populating
     * an Event queue based on those events.
     */
    class MultiKeyListener : public threading::BackgroundTask
    {
        /**
         * Constructs a MultiKeyListener from a configuration file which
         * should contain keybindings.
         * @param configFile A configuration file containing keybinds.
         * @param queue The event queue this listener pushes detected events
         *        into. The referenced queue must outlive this listener.
         */
        MultiKeyListener(const std::string& configFile,
                         ui::EventQueue& queue);
                         
        /**
         * Constructs a MultiKeyListener from a map of keys and events.
         * @param eventMappings A mapping of keybinds to events.
         * @param queue The event queue this listener pushes detected events
         *        into. The referenced queue must outlive this listener.
         */
        MultiKeyListener(const KeybindEventMap& eventMappings,
                         ui::EventQueue& queue);  

        /**
         * Destructor. Stops the listener and joins the worker thread.
         */
        ~MultiKeyListener() override = default;
    
        /**
         * @brief Initializes the key listener by registering key grabs with the system.
         *
         * This method sets up platform-specific key listening behavior. On Linux, 
         * it grabs the specified keys globally using XGrabKey and selects for key 
         * press events on the root window.
         */
        void initialize();

    protected:

        /**
         * TODO
         */
        void run() override;

    private:
        /// The keybind mappings to listen for with this object.
        KeybindEventMap keybinds;

        /// The event queue shared with the reading thread. Owned by the caller.
        ui::EventQueue& eventQueue;
    };
} // namespace ui
