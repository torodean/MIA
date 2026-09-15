/**
 * @file PythonUITests.cpp
 * @author Antonius Torode
 * @date 09/14/2026
 * @brief Implements the python UI test methods.
 */

#include <cstdint>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

// The associated header file.
#include "PythonUITests.hpp"
// Used for the Python module wrapper being tested.
#include "PythonModule.hpp"
// Used for testing the python UI library.
#include "EventQueue.hpp"
#include "PythonUIListener.hpp"
// Used for the in-file test listener.
#include "BackgroundTask.hpp"
// Used for the return codes.
#include "Constants.hpp"


/*
 * This class is designed to 'listen' to a python UI in a background thread and collect
 * events that it sends to then be processed by the c++ backend.
 */
class PythonUIListener : public threading::BackgroundTask
{
public:
    PythonUIListener(PythonModule& module) :
        uiModule(module)
    { };

    std::vector<std::string> getEvents()
    {
        std::lock_guard<std::mutex> lock(eventsMutex);

        std::vector<std::string> result;
        result.swap(events);

        return result;
    }

protected:

    void run() override
    {
        // Get all events from the python module.
        PythonResult result = uiModule.call("getAllEvents");

        // Append all events to the stored events queue.
        std::vector<std::string> strings = result.asStrings();
        for (size_t i=0; i<strings.size(); i++)
        {
            addEvent(strings[i]);
        }
    }
private:

    void addEvent(const std::string& event)
    {
        std::lock_guard<std::mutex> lock(eventsMutex);
        events.push_back(event);
    }

    /// The UI module that is loaded with this listener.
    PythonModule& uiModule;

    /// Storage for the events received from the UI.
    std::vector<std::string> events;
    std::mutex eventsMutex;

}; // class PythonUIListener


int testPythonUI()
{
    // Load the test python UI module.
    PythonModule module("testUI", __FILE__);
    PythonUIListener listener(module);

    // Setup a listener which monitors events.
    listener.start();

    // Show the UI.
    module.call("createUI");

    // 8 bits should be plenty to store events between polls.
    uint8_t increments = 0;
    uint8_t decrements = 0;
    int currentDisplayVal = 0;

    while (listener.isRunning())
    {
        std::vector<std::string> events = listener.getEvents();
        for (size_t i=0; i<events.size(); i++)
        {
            if (events[i] == "increment")
                increments++;
            else if (events[i] == "decrement")
                decrements++;
            else if (events[i] == "stop")
            {
                listener.stop();
                break;
            }
        }

        // Update the value based on events.
        currentDisplayVal += increments - decrements;
        increments = decrements = 0;

        // Update the UI.
        module.call("setValue", currentDisplayVal);
    }

    return constants::SUCCESS;
}


int testPythonUILibrary()
{
    // Load the test python UI module.
    PythonModule module("testUI", __FILE__);

    // The queue the listener pushes events into and this test reads from.
    ui::EventQueue queue;
    python_ui::PythonUIListener listener(module, queue);

    // Setup a listener which monitors events.
    listener.start();

    // Show the UI.
    module.call("createUI");

    // 8 bits should be plenty to store events between polls.
    uint8_t increments = 0;
    uint8_t decrements = 0;
    int currentDisplayVal = 0;

    while (listener.isRunning())
    {
        // Read whatever events the listener has polled so far.
        ui::EventStorage storage = queue.getAllEvents();

        for (const ui::Event& event : storage.getEvents())
        {
            if (event.getName() == "increment")
                increments++;
            else if (event.getName() == "decrement")
                decrements++;
            else if (event.getName() == "stop")
            {
                listener.stop();
                break;
            }
        }

        // Update the value based on events.
        currentDisplayVal += increments - decrements;
        increments = decrements = 0;

        // Update the UI.
        module.call("setValue", currentDisplayVal);
    }

    return constants::SUCCESS;
}
