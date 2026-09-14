/**
 * @file EventStorage.hpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Declares the batch of events returned by one read from a python UI listener.
 */
#pragma once

#include <string>
#include <vector>

// The event type stored by this container.
#include "Event.hpp"
// The python result type converted into events by this container's helpers.
#include "PythonResult.hpp"


namespace python_ui
{
    /**
     * The events returned by one read from a python UI listener.
     * A PythonUIListener returns this from getEvents() and waitForEvents().
     * The listener fills new storage through append(); a caller reads the
     * events through hasEvents() and getEvents(). Default construction always
     * starts empty, so the only way to obtain non-empty EventStorage is
     * through a listener read.
     *
     * This is a plain value type: it is filled by the listener, returned to
     * the caller, and never shared with other threads, so it needs no locking.
     */
    class EventStorage
    {
    public:

        /// Constructs empty storage.
        EventStorage() = default;

        /**
         * Appends events to this storage. This is the write interface the 
         * listener uses when filling storage from a poll.
         *
         * @param newEvents The events to append, in order.
         */
        void append(const std::vector<Event>& newEvents);

        /**
         * Checks whether this read produced any events.
         *
         * @return True if at least one event is stored.
         */
        bool hasEvents() const;

        /**
         * Gets the stored events.
         *
         * @return The events, in the order they were polled. Empty when
         *     hasEvents() is false.
         */
        const std::vector<Event>& getEvents() const;

    private:

        /// The events from one read, in the order they were polled.
        std::vector<Event> events;
    }; // class EventStorage


    /**
     * Converts a python result into events.
     * A Strings result yields one event per string, and a String result
     * yields a single event; any other result type (e.g. Void) yields no
     * events. This is the shared conversion used by handlers when filling
     * storage from a poll of a python UI, whether the polled method returns
     * a batch of events or one event per call.
     *
     * @param result The python result to convert.
     * @return The converted events, in the order of the result's strings.
     */
    std::vector<Event> toEvents(const PythonResult& result);
} // namespace python_ui
