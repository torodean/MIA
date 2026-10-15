/**
 * @file PythonUIListener_T.cpp
 * @author Antonius Torode
 * @date 10/12/2026
 * @brief Unit tests for the PythonUIListener class and related features.
 */

#include <gtest/gtest.h>

// Include the associated file for testing.
#include "PythonUIListener.hpp"
#include "EventQueue.hpp"
#include "EventStorage.hpp"
#include "Timing.hpp"


namespace python_ui
{
    class PythonUIListener_T : public ::testing::Test
    {
    public:
        PythonUIListener_T() :
            module("testUI", __FILE__)
        { }

        /// The module containing the test python code.
        PythonModule module;
        /// The event queue to store any UI events received.
        ui::EventQueue queue;

    protected:
        void SetUp() override 
        {
            // Reset the uiEvents in the python file to empty.
            module.call("reset");
        }
    }; // class PythonUIListener_T


    /*
     * Test that the listener can be started and will correctly return an event.
     */
    TEST_F(PythonUIListener_T, listenerReturnsEvent)
    {
        // Construct the listener.
        python_ui::PythonUIListener listener(module, queue);

        // Setup a listener which monitors events.
        listener.start();

        ASSERT_TRUE(listener.isRunning())
            << "The listener should be started and running.";

        // Read whatever events the listener has polled so far.
        ui::EventStorage storage = queue.getAllEvents();

        ASSERT_FALSE(storage.hasEvents())
            << "The listener should not have any events until ui methods are called.";

        // Add an increment event by calling the UI's increment method.
        module.call("increment");

        /* 
         * The listener takes care of populating the queue, but it might take longer than 
         * this method's control flow to actually perform that work, so sleep a little.
         */
        timing::sleepMilliseconds(5);
        storage = queue.getAllEvents();

        ASSERT_TRUE(storage.hasEvents())
            << "The listener should have an events after calling UI method.";
        ASSERT_EQ(storage.getEvents().size(), 1)
            << "The event storage should contain exactly one event.";

        listener.stop();
    }


    /*
     * Test that the listener can be started and will correctly return multiple events.
     */
    TEST_F(PythonUIListener_T, listenerReturnsEvents)
    {
        // Construct the listener.
        python_ui::PythonUIListener listener(module, queue);

        // Setup a listener which monitors events.
        listener.start();

        // Read whatever events the listener has polled so far.
        ui::EventStorage storage = queue.getAllEvents();

        ASSERT_FALSE(storage.hasEvents())
            << "The listener should not have any events until ui methods are called.";

        // Add an increment event by calling the UI's increment method.
        module.call("increment");
        module.call("increment");
        module.call("decrement");

        /* 
         * The listener takes care of populating the queue, but it might take longer than 
         * this method's control flow to actually perform that work, so sleep a little.
         */
        timing::sleepMilliseconds(5);
        storage = queue.getAllEvents();

        ASSERT_TRUE(storage.hasEvents())
            << "The listener should have an events after calling UI method.";
        ASSERT_EQ(storage.getEvents().size(), 3)
            << "The event storage should contain exactly one event.";

        listener.stop();
    }


    /*
     * Test that the listener pollInterval works.
     */
    TEST_F(PythonUIListener_T, listenerWithPollInterval)
    {
        // Construct the listener with a 300ms pollInterval.
        python_ui::PythonUIListener listener(module, queue, "getAllEvents", 200);

        // Pre-add one event to the python module's queue.
        module.call("increment");

        // Storage for the retreived events.
        ui::EventStorage storage;

        // Setup a listener which monitors events.
        listener.start();

        /* 
         * The listener takes care of populating the queue, but it might take longer than 
         * this method's control flow to actually perform that work, so sleep a little.
         */
        timing::sleepMilliseconds(5);
        // Read whatever events the listener has polled so far.
        ASSERT_FALSE(storage.hasEvents())
            << "The listener should not have events yet.";
        storage = queue.getAllEvents();
        ASSERT_TRUE(storage.hasEvents())
            << "The listener should have the pre-queued events.";
        ASSERT_EQ(storage.getEvents().size(), 1)
            << "The event storage should contain exactly one event.";

        // Add a few more events.
        module.call("increment");
        module.call("decrement");

        timing::sleepMilliseconds(5);
        storage = queue.getAllEvents();
        ASSERT_EQ(storage.getEvents().size(), 0)
            << "The event storage should not contain any event.";

        timing::sleepMilliseconds(300);
        storage = queue.getAllEvents();

        // The listener should have the new events now since it's been over 200ms.
        ASSERT_EQ(storage.getEvents().size(), 2)
            << "The event storage should contain exactly two new events.";

        listener.stop();
    }
} // namespace python_ui
