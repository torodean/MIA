/**
 * @file EventStorage.cpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Implements the batch of events returned by one read from a python UI listener.
 */

// The associated header file.
#include "EventStorage.hpp"


namespace python_ui
{
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


    std::vector<Event> toEvents(const PythonResult& result)
    {
        std::vector<Event> events;

        if (result.getType() == PythonResult::Type::Strings)
        {
            const std::vector<std::string>& strings = result.asStrings();
            events.reserve(strings.size());
            for (const std::string& value : strings)
                events.emplace_back(value);
        }        
        else if (result.getType() == PythonResult::Type::String)
        {
            events.emplace_back(result.asString());
        }

        return events;
    }
} // namespace python_ui
