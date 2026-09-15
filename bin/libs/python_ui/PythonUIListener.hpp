/**
 * @file PythonUIListener.hpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Declares a background listener which polls a python UI for events.
 */
#pragma once

#include <chrono>
#include <string>
#include <vector>

// The event types passed between the listener and the reader.
#include "Event.hpp"
#include "EventStorage.hpp"
// The shared queue this listener pushes polled events into.
#include "EventQueue.hpp"
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
     * (getAllEvents by default) and pushes the returned events into an
     * EventQueue shared with the reading thread. The reader takes the events
     * from the queue with getAllEvents() (non-blocking) or waitForEvents()
     * (blocks until events are available or the queue is closed). Each read
     * returns an EventStorage holding the read events. The python UI is
     * always polled; waitForEvents() only blocks the c++ side until polled
     * events actually arrive.
     *
     * The EventQueue is passed in and shared rather than owned so that other
     * event sources (e.g. an input listener) can push into the same queue,
     * giving the reader a single place to wait. The queue must outlive this
     * listener.
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
     * The listener converts the python result strings into ui::Events in run();
     * the python-string representation never leaves the listener, and the
     * EventStorage interface deals only in Events.
     *
     * The listener and the calling thread use the same PythonModule at the
     * same time (the worker thread polls while the main thread pushes UI
     * updates); PythonModule serializes that python work with the GIL, so it
     * is safe to share. The EventQueue needs its own mutex because it is
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
         * @param queue The event queue this listener pushes polled events
         *        into. The referenced queue must outlive this listener.
         * @param getEventsMethod The name of the module method to call each
         *        poll. The method must return a list of strings.
         * @param pollInterval The time to sleep after each poll. The default
         *        (zero) polls as fast as possible.
         * @param maxConsecutiveFailures The number of consecutive failed
         *        polls after which the listener stops itself. Zero (the default)
         *        means failures are reported but never stop the listener.
         */
        PythonUIListener(PythonModule& module,
                         ui::EventQueue& queue,
                         const std::string& getEventsMethod = "getAllEvents",
                         std::chrono::milliseconds pollInterval = std::chrono::milliseconds(0),
                         unsigned int maxConsecutiveFailures = 0);

        /**
         * Destructor. Stops the listener and joins the worker thread.
         */
        ~PythonUIListener() override = default;

        /*
         * Copying is deleted by the BackgroundTask base class; a listener owns
         * one thread which cannot be duplicated.
         */

        /**
         * Requests the listener to stop, closes the shared event queue, and
         * joins the worker thread. Closing the queue wakes any thread blocked
         * in waitForEvents() so it can observe the shutdown.
         */
        void stop() override;

    protected:

        /**
         * Performs one poll of the UI module.
         * Called repeatedly by the BackgroundTask loop. Each call retrieves
         * the module's pending events, converts them from python result
         * strings into Events, and pushes them into the shared queue. When
         * the consecutive failure limit is reached, this sets the stop flag
         * directly and closes the queue to wake any waiting reader (calling
         * stop() here would join the thread from itself).
         */
        void run() override;

    private:

        /**
         * Reports a failed poll to standard error when verbose mode is
         * enabled, through the RuntimeContext (see setContext()).
         *
         * @param result The failed result carrying the error description.
         */
        void reportFailure(const PythonResult& result);

        /// The python module implementing the UI. Owned by the caller.
        PythonModule& uiModule;

        /// The event queue shared with the reading thread. Owned by the caller.
        ui::EventQueue& eventQueue;

        /// The name of the module method which returns the pending events.
        std::string getEventsMethod;

        /// The time to sleep after each poll. Zero polls continuously.
        std::chrono::milliseconds pollInterval;

        /// The consecutive-failure limit. Zero means never stop on failures.
        unsigned int maxConsecutiveFailures;

        /// The number of failed polls since the last successful one.
        unsigned int consecutiveFailures{0};
    }; // class PythonUIListener


    /**
     * Converts a python result into events.
     * A Strings result yields one event per string, and a String result
     * yields a single event; any other result type (e.g. Void) yields no
     * events. This is the conversion used when filling storage from a poll
     * of a python UI, whether the polled method returns a batch of events
     * or one event per call.
     *
     * @param result The python result to convert.
     * @return The converted events, in the order of the result's strings.
     */
    std::vector<ui::Event> toEvents(const PythonResult& result);
} // namespace python_ui
