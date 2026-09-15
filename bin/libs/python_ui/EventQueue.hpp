/**
 * @file EventQueue.hpp
 * @author Antonius Torode
 * @date 09/15/2026
 * @brief Declares a thread-safe queue which collects events from producers for a reader.
 */
#pragma once

#include <condition_variable>
#include <mutex>
#include <vector>

// The event type stored by this queue.
#include "Event.hpp"
// The batch type returned by the queue's read methods.
#include "EventStorage.hpp"


namespace python_ui
{
    /**
     * A thread-safe queue which collects events from one or more producers
     * for a single reader.
     *
     * Producers (e.g. a PythonUIListener, and later an input listener) push
     * polled events through addEvents(); the reader reads them through
     * getAllEvents() (non-blocking) or waitForEvents() (blocks until events
     * are available or the queue is closed). Because all producers share one
     * queue, the reader has a single place to wait no matter which producer
     * the events came from.
     *
     * The queue holds no python state; it is plain c++-side state which the
     * GIL does not cover, so it carries its own mutex and condition variable.
     * Notifications are sent outside the lock so a woken reader does not
     * immediately block again.
     *
     * This is a plain value type shared by reference between producers and
     * the reader; it cannot be copied, since copying would duplicate the
     * mutex-protected state which the threads synchronize through.
     */
    class EventQueue
    {
    public:

        /// Constructs empty, open storage.
        EventQueue() = default;

        /*
         * Copying is deleted because producers and the reader synchronize
         * through this queue's mutex; a copy would not be shared with them.
         */

        /**
         * Appends events to the queue and wakes any thread waiting in
         * waitForEvents().
         *
         * @param newEvents The events to append, in order.
         */
        void addEvents(const std::vector<Event>& newEvents);

        /**
         * Gets all pending events and drains the queue.
         * This never blocks; when no events are pending it returns empty
         * storage.
         *
         * @return The pending events, in the order they were pushed.
         */
        EventStorage getAllEvents();

        /**
         * Gets all pending events and drains the queue, blocking until at
         * least one event is available or the queue is closed.
         * This lets the reader sleep instead of spinning on getAllEvents();
         * it is woken as soon as a producer pushes events. A close() while
         * waiting wakes the reader, which returns whatever was pending
         * (possibly nothing).
         *
         * @return The pending events, in order, or empty storage if the
         *         queue was closed with no pending events.
         */
        EventStorage waitForEvents();

        /**
         * Closes the queue and wakes any thread blocked in waitForEvents().
         * Closing is permanent: after this, waitForEvents() never blocks
         * again. Producers close the queue when they stop, so a waiting
         * reader observes shutdown instead of waiting for an event which
         * will never arrive.
         */
        void close();

        /**
         * Checks whether this queue has been closed.
         *
         * @return True if close() has been called.
         */
        bool isClosed() const;

    private:

        /// Guards the queued events and the closed flag.
        mutable std::mutex eventsMutex;

        /**
         * Wakes threads waiting in waitForEvents() when events arrive or the
         * queue is closed.
         */
        std::condition_variable eventsAvailable;

        /// The events pushed by producers which have not been read yet.
        std::vector<Event> events;

        /// Whether close() has been called. Once true, waitForEvents() never blocks.
        bool closed{false};
    }; // class EventQueue
} // namespace python_ui
