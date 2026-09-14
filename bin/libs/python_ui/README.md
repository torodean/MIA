# Python UI Library

This folder is reserved for a library providing graphical UIs for c++ applications, built with python (Tkinter) and driven through the embedded python interpreter. There is no library code here yet; the starting work is being tested in the `mia_test` app.

## Current Approach (Under Development)

The working prototype lives in `bin/apps/mia_test/`:

- **testUI.py**: A Tkinter UI whose button callbacks report user actions as event strings to a module-level queue, rather than acting on them. It also exposes a `setValue()` method so an external backend can push state updates into the UI.
- **MIATest.cpp**: The c++ side (`testPythonUI()` and the `PythonUIListener` class). A `BackgroundTask` polls the python module's `getAllEvents()` on a background thread and hands the events to the main thread through a mutex-guarded queue, while the main loop processes the events, updates its own state, and calls `setValue()` to refresh the UI.

The design points this exercises, which the library is expected to keep:

- The python UI owns no application state; the c++ backend owns the state and the UI only displays it.
- UI actions are reported as events which the backend retrieves and interprets.
- The UI and backend run on separate threads of control (Tk's event loop vs the c++ logic), so they communicate through the event queue and setter calls instead of calling into each other freely.

## Status

Experimental. The library structure, CMake target, and API do not exist yet; expect this folder's contents and this description to change as the code migrates over from `mia_test`.
