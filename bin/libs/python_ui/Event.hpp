/**
 * @file Event.hpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Declares a single event produced by a python UI.
 */
#pragma once

#include <string>


namespace python_ui
{
    /**
     * A single event produced by a python UI.
     * This currently wraps the plain string which the python UI reports for
     * each user action (e.g. "increment", "stop"). It exists as its own type
     * so the event representation can later gain structure (e.g. a name with
     * a payload) without changing the API of the classes which pass events
     * around.
     */
    class Event
    {
    public:

        /// Constructs an event from its string representation.
        Event(std::string name);

        /**
         * Gets the string representation of this event.
         *
         * @return The event string, as reported by the python UI.
         */
        const std::string& getName() const;

    private:

        /// The string representation reported by the python UI.
        std::string name;
    }; // class Event
} // namespace python_ui
