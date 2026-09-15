/**
 * @file Event.cpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Implements a single event produced by a user interface.
 */

// The associated header file.
#include "Event.hpp"

#include <utility>


namespace ui
{
    Event::Event(std::string eventName)
        : name(std::move(eventName))
    { }


    const std::string& Event::getName() const
    {
        return name;
    }
} // namespace ui
