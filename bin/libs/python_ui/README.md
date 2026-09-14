# Python UI Library

This folder contains a library for providing graphical UIs for c++ applications, built with python (Tkinter) and driven through the embedded python interpreter. The starting code has been written here; the working prototype it is based on lives in the `mia_test` app.

## Current Contents

- **Event.hpp/.cpp**: A single event produced by a python UI. Currently wraps the string the python UI reports for each user action (e.g. "increment", "stop"), as its own type so the representation can gain structure later without changing the API of the classes which pass events around.
- **EventStorage.hpp/.cpp**: The batch of events returned by one read from a listener. Exposes `append()` (the listener's write interface), `hasEvents()`, and `getEvents()`. Also defines the `toEvents()` free function, the shared conversion from a `PythonResult` (a list of strings or a single string) into events.
- **PythonUIListener.hpp/.cpp**: A `BackgroundTask` which polls the python UI module's event-retrieval method (`getAllEvents` by default, overridable) and queues the resulting events for the main thread. Reads drain the queue through `getEvents()` (non-blocking) or `waitForEvents()` (blocks on a condition variable until events arrive or the listener stops). Failed polls are reported to stderr when verbose mode is enabled and never kill the listener; an optional consecutive-failure limit can stop the listener after N failures in a row. The poll interval defaults to zero (poll as fast as possible) with an option to sleep between polls.

There is no CMake target yet; this folder's sources are not built by the project.

## Current Approach (Under Development)

The working prototype lives in `bin/apps/mia_test/`:

- **testUI.py**: A Tkinter UI whose button callbacks report user actions as event strings to a module-level queue, rather than acting on them. It also exposes a `setValue()` method so an external backend can push state updates into the UI.
- **MIATest.cpp**: The c++ side (`testPythonUI()` and an earlier `PythonUIListener` class). A `BackgroundTask` polls the python module's `getAllEvents()` on a background thread and hands the events to the main thread through a mutex-guarded queue, while the main loop processes the events, updates its own state, and calls `setValue()` to refresh the UI.

The design points this exercises, which the library keeps:

- The python UI owns no application state; the c++ backend owns the state and the UI only displays it.
- UI actions are reported as events which the backend retrieves and interprets.
- The UI and backend run on separate threads of control (Tk's event loop vs the c++ logic), so they communicate through the event queue and setter calls instead of calling into each other freely.

## Status

UNTESTED. The library code in this folder has not been compiled or run; it was written against the interfaces of `BackgroundTask` and `PythonModule` and the working `mia_test` prototype, but no build target references it yet. The CMake target, unit tests, and a first real UI module are the expected next steps, so expect this folder's contents and this description to change.
