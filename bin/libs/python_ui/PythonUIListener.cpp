/**
 * @file PythonUIListener.cpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Implements a background listener which polls a python UI for events.
 */

// The associated header file.
#include "PythonUIListener.hpp"

// Used for the poll interval sleep.
#include "Timing.hpp"


namespace python_ui
{
    PythonUIListener::PythonUIListener(PythonModule& module,
                                       const std::string& getEventsMethodName,
                                       std::chrono::milliseconds pollIntervalMs,
                                       unsigned int maxConsecutiveFailuresLimit)
        : uiModule(module),
          getEventsMethod(getEventsMethodName),
          pollInterval(pollIntervalMs),
          maxConsecutiveFailures(maxConsecutiveFailuresLimit)
    { }


    EventStorage PythonUIListener::getEvents()
    {
        std::vector<Event> drained;

        {
            std::lock_guard<std::mutex> lock(eventsMutex);
            events.swap(drained);
        }

        EventStorage storage;
        storage.append(drained);
        return storage;
    }


    EventStorage PythonUIListener::waitForEvents()
    {
        std::vector<Event> drained;

        {
            std::unique_lock<std::mutex> lock(eventsMutex);

            /*
             * The predicate covers both wake conditions: events became available,
             * or the listener stopped (so shutdown wakes the waiter instead of
             * leaving it blocked forever). The wait re-checks the predicate under
             * the lock, which also handles spurious wakeups and notifications
             * which arrive before the wait starts.
             */
            eventsCv.wait(lock, [this] {
                return !events.empty() || stopRequested.load();
            });

            drained.swap(events);
        }

        EventStorage storage;
        storage.append(drained);
        return storage;
    }


    void PythonUIListener::stop()
    {
        {
            std::lock_guard<std::mutex> lock(eventsMutex);
            stopRequested = true;
        }

        /*
         * Wake any thread blocked in waitForEvents() so it can observe the stop
         * instead of waiting for an event which will never arrive.
         */
        eventsCv.notify_all();

        BackgroundTask::stop();
    }


    void PythonUIListener::run()
    {
        // Get the pending events from the python UI module.
        PythonResult result = uiModule.call(getEventsMethod);

        if (!result.isValid())
        {
            ++consecutiveFailures;
            reportFailure(result);

            if (maxConsecutiveFailures > 0 && consecutiveFailures >= maxConsecutiveFailures)
            {
                // Sets the flag directly; stop() would join the thread from itself.
                stopRequested = true;
                eventsCv.notify_all();
            }
            return;
        }

        consecutiveFailures = 0;

        storeEvents(toEvents(result));

        if (pollInterval.count() > 0)
            timing::sleepMilliseconds(static_cast<int>(pollInterval.count()));
    }


    void PythonUIListener::storeEvents(const std::vector<Event>& newEvents)
    {
        {
            std::lock_guard<std::mutex> lock(eventsMutex);
            events.insert(events.end(), newEvents.begin(), newEvents.end());
        }

        // Notify outside the lock so a woken reader does not immediately block.
        eventsCv.notify_all();
    }


    void PythonUIListener::reportFailure(const PythonResult& result)
    {
        if (context == nullptr || !context->verboseMode)
            return;

        std::cerr << "WARNING: Python UI event poll '" << getEventsMethod
                  << "' failed: " << result.getError() << std::endl;
    }
} // namespace python_ui
