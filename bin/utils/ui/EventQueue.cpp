/**
 * @file EventQueue.cpp
 * @author Antonius Torode
 * @date 09/15/2026
 * @brief Implements a thread-safe queue which collects events for the reader.
 */

// The associated header file.
#include "EventQueue.hpp"


namespace ui
{
    void EventQueue::addEvents(const std::vector<Event>& newEvents)
    {
        {
            std::lock_guard<std::mutex> lock(eventsMutex);
            events.insert(events.end(), newEvents.begin(), newEvents.end());
        }

        // Notify outside the lock so a woken reader does not immediately block.
        eventsAvailable.notify_all();
    }


    EventStorage EventQueue::getAllEvents()
    {
        std::vector<Event> readEvents;

        {
            std::lock_guard<std::mutex> lock(eventsMutex);
            events.swap(readEvents);
        }

        EventStorage storage;
        storage.append(readEvents);
        return storage;
    }


    EventStorage EventQueue::waitForEvents()
    {
        std::vector<Event> readEvents;

        {
            std::unique_lock<std::mutex> lock(eventsMutex);

            /*
             * The wait condition covers both wake conditions: events became
             * available, or the queue was closed (so shutdown wakes the reader
             * instead of leaving it blocked forever). The wait re-checks the
             * condition under the lock, which also handles spurious wakeups and
             * notifications which arrive before the wait starts.
             */
            eventsAvailable.wait(lock, [this] {
                return !events.empty() || closed;
            });

            events.swap(readEvents);
        }

        EventStorage storage;
        storage.append(readEvents);
        return storage;
    }


    void EventQueue::close()
    {
        {
            std::lock_guard<std::mutex> lock(eventsMutex);
            closed = true;
        }

        /*
         * Wake any thread blocked in waitForEvents() so it can observe the
         * close instead of waiting for an event which will never arrive.
         */
        eventsAvailable.notify_all();
    }


    bool EventQueue::isClosed() const
    {
        std::lock_guard<std::mutex> lock(eventsMutex);
        return closed;
    }
} // namespace ui
