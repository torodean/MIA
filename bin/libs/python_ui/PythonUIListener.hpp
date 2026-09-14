/**
 * @file PythonUIListener.hpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Declares a background listener which polls a python UI for events.
 */
#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>

// The event types returned by listener reads.
#include "Event.hpp"
#include "EventStorage.hpp"
// The base class which provides the background thread and stop signaling.
#include "BackgroundTask.hpp"
// The python interpreter used to poll the UI module.
#include "PythonModule.hpp"


namespace python_ui
{
    /**
     * A background task which listens for events produced by a python UI.
     *
     * The listener repeatedly calls the UI module's event-retrieval method
     * (getAllEvents by default) and stores the returned events in an internal
     * queue. The main thread reads them with getEvents() (non-blocking drain)
     * or waitForEvents() (blocks until events are available or the listener
     * stops). Each read returns an EventStorage holding the drained events.
     * The python UI is always polled; waitForEvents() only blocks the c++
     * side until polled events actually arrive.
     *
     * This class assumes the UI module is shared with other handlers (e.g. a
     * setter for pushing UI updates), so it holds a reference to an
     * externally-owned PythonModule rather than owning its own. The referenced
     * module must outlive this listener.
     *
     * A poll which fails (a missing method, a python exception, a conversion
     * failure) does not kill the listener: the failure is reported to standard
     * error and polling continues. If maxConsecutiveFailures is non-zero, the
     * listener stops itself once that many polls fail in a row. A successful
     * poll resets the consecutive failure count.
     *
     * By default there is no pause between polls, since UI responsiveness is
     * the priority. A pollInterval can be set to trade a little latency for
     * less CPU use and less GIL contention.
     *
     * The listener converts the python result strings into Events in run();
     * the python-string representation never leaves the listener, and the
     * EventStorage interface deals only in Events.
     *
     * The listener and the calling thread use the same PythonModule at the
     * same time (the worker thread polls while the main thread pushes UI
     * updates); PythonModule serializes that python work with the GIL, so it
     * is safe to share. The event queue needs its own mutex because it is
     * c++-side state which the GIL does not cover.
     */
    class PythonUIListener : public threading::BackgroundTask
    {
    public:

        /**
         * Constructs a listener for the given python UI module.
         *
         * @param module The python module implementing the UI. The referenced
         *        module must outlive this listener.
         * @param getEventsMethod The name of the module method to call each
         *        poll. The method must return a list of strings.
         * @param pollInterval The time to sleep after each poll. The default
         *        (zero) polls as fast as possible.
         * @param maxConsecutiveFailures The number of consecutive failed
         *        polls after which the listener stops itself. Zero (the default)
         *        means failures are reported but never stop the listener.
         */
        PythonUIListener(PythonModule& module,
                         const std::string& getEventsMethod = "getAllEvents",
                         std::chrono::milliseconds pollInterval = std::chrono::milliseconds(0),
                         unsigned int maxConsecutiveFailures = 0);

        /**
         * Destructor. Stops the listener and joins the worker thread.
         */
        ~PythonUIListener() override = default;

        /*
         * Copying is deleted by the BackgroundTask base class; a listener owns
         * one thread and a mutex-protected queue which cannot be duplicated.
         */

        /**
         * Gets all pending events and drains the queue.
         * This never blocks; when no events are pending it returns empty
         * storage.
         *
         * @return The pending events, in the order they were polled.
         */
        EventStorage getEvents();

        /**
         * Gets all pending events and drains the queue, blocking until at
         * least one event is available or the listener is stopped.
         * This lets the calling thread sleep instead of spinning on
         * getEvents(); the listener wakes it as soon as polled events are
         * stored. A listener which is stopped while waiting wakes up and
         * returns whatever was pending (possibly nothing).
         *
         * @return The pending events, in order, or empty storage if the
         *         listener stopped with no pending events.
         */
        EventStorage waitForEvents();

        /**
         * Requests the listener to stop and wakes any thread blocked in
         * waitForEvents(), then joins the worker thread.
         */
        void stop() override;

    protected:

        /**
         * Performs one poll of the UI module.
         * Called repeatedly by the BackgroundTask loop. Each call retrieves
         * the module's pending events, converts them from python result
         * strings into Events, appends them to the internal queue, and wakes
         * any thread waiting in waitForEvents(). When the consecutive failure
         * limit is reached, this sets the stop flag directly and wakes any
         * waiting reader (calling stop() here would join the thread from
         * itself).
         */
        void run() override;

    private:

        /**
         * Appends polled events to the queue and wakes any waiting reader.
         *
         * @param newEvents The events returned by the latest poll.
         */
        void storeEvents(const std::vector<Event>& newEvents);

        /**
         * Reports a failed poll to standard error when verbose mode is
         * enabled, through the RuntimeContext (see setContext()).
         *
         * @param result The failed result carrying the error description.
         */
        void reportFailure(const PythonResult& result);

        /// The python module implementing the UI. Owned by the caller.
        PythonModule& uiModule;

        /// The name of the module method which returns the pending events.
        std::string getEventsMethod;

        /// The time to sleep after each poll. Zero polls continuously.
        std::chrono::milliseconds pollInterval;

        /// The consecutive-failure limit. Zero means never stop on failures.
        unsigned int maxConsecutiveFailures;

        /// The number of failed polls since the last successful one.
        unsigned int consecutiveFailures{0};

        /// Guards the event queue, which is shared with the calling thread.
        mutable std::mutex eventsMutex;

        /**
         * Wakes threads waiting in waitForEvents() when events arrive or the
         * listener stops. Always notify while not holding eventsMutex's lock
         * (after the queue update) to avoid waking a thread which would
         * immediately block again.
         */
        std::condition_variable eventsCv;

        /// The events polled from the UI which have not been read yet.
        std::vector<Event> events;
    }; // class PythonUIListener
} // namespace python_ui
