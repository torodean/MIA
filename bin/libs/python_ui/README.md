# Python UI Library

This folder contains a library for providing graphical UIs for c++ applications, built with python (Tkinter) and driven through the embedded python interpreter. The library code started as a prototype in the `mia_test` app.

## Current Contents

- **Event.hpp/.cpp**: A single event produced by a python UI. Currently wraps the string the python UI reports for each user action (e.g., "increment", "stop"), as its own type so the representation can gain structure later without changing the API of the classes which pass events around.
- **EventStorage.hpp/.cpp**: The batch of events returned by one read from a listener. Provides `append()` (the listener's write interface), `hasEvents()`, and `getEvents()`. Also defines the `toEvents()` free function, the shared conversion from a `PythonResult` (a list of strings or a single string) into events.
- **PythonUIListener.hpp/.cpp**: A `BackgroundTask` which polls the python UI module's event-retrieval method (`getAllEvents` by default, overridable) and queues the resulting events for the main thread. Reads drain the queue through `getEvents()` (non-blocking) or `waitForEvents()` (blocks on a condition variable until events arrive or the listener stops). Failed polls are reported to stderr when verbose mode is enabled and never kill the listener; an optional consecutive-failure limit can stop the listener after N failures in a row. The poll interval defaults to zero (poll as fast as possible) with an option to sleep between polls.

The library is built as `Py_UI_LIB`.

## Design Points

The design points this library is built around, which come from the working prototype:

- The python UI owns no application state; the c++ backend owns the state and the UI only displays it.
- UI actions are reported as events which the backend retrieves and interprets.
- The UI and backend run on separate threads of control (Tk's event loop vs the c++ logic), so they communicate through the event queue and setter calls instead of calling into each other freely.

## Status

This library is still in development. The CMake target and the first listener are in place; unit tests and a first real UI module are expected next steps, so expect this folder's contents and this description to change.
