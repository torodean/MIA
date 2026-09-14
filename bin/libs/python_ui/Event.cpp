/**
 * @file Event.cpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Implements a single event produced by a python UI.
 */

// The associated header file.
#include "Event.hpp"

#include <utility>


Event::Event(std::string eventName)
    : name(std::move(eventName))
{ }


const std::string& Event::getName() const
{ 
    return name; 
}
