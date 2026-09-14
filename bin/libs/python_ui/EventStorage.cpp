/**
 * @file EventStorage.cpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Implements the batch of events returned by one read from a python UI listener.
 */

// The associated header file.
#include "EventStorage.hpp"


void EventStorage::append(const std::vector<Event>& newEvents)
{
    events.insert(events.end(), newEvents.begin(), newEvents.end());
}


bool EventStorage::hasEvents() const
{
    return !events.empty();
}


const std::vector<Event>& EventStorage::getEvents() const
{
    return events;
}
