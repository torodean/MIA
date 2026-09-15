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
                                       EventQueue& queue,
                                       const std::string& getEventsMethodName,
                                       std::chrono::milliseconds pollIntervalMs,
                                       unsigned int maxConsecutiveFailuresLimit)
        : uiModule(module),
          eventQueue(queue),
          getEventsMethod(getEventsMethodName),
          pollInterval(pollIntervalMs),
          maxConsecutiveFailures(maxConsecutiveFailuresLimit)
    { }


    void PythonUIListener::stop()
    {
        /*
         * Close the queue before joining so any thread blocked in
         * waitForEvents() wakes and observes the shutdown instead of waiting
         * for an event which will never arrive.
         */
        eventQueue.close();

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
                eventQueue.close();
            }
            return;
        }

        consecutiveFailures = 0;

        eventQueue.addEvents(toEvents(result));

        if (pollInterval.count() > 0)
            timing::sleepMilliseconds(static_cast<int>(pollInterval.count()));
    }


    void PythonUIListener::reportFailure(const PythonResult& result)
    {
        if (context == nullptr || !context->verboseMode)
            return;

        std::cerr << "WARNING: Python UI event poll '" << getEventsMethod
                  << "' failed: " << result.getError() << std::endl;
    }
} // namespace python_ui
