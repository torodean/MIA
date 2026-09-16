# Utilities - UI

This folder contains utilities for user interface input: event types, a shared event queue, key listeners which detect key presses and combinations, etc.

## Event

The `Event` class wraps the plain string which a user interface reports for one user action (e.g., "increment" or "stop"). It exists as its own type so the event representation can gain structure later without changing the API of the classes which pass events around.

## EventQueue and EventStorage

The `EventQueue` class is a thread-safe queue which collects events from one or more producers for a single reader. Producers push events through `addEvents()`; the reader drains them through `getAllEvents()` (non-blocking) or `waitForEvents()` (blocks until events arrive or the queue is closed). Because all producers share one queue, the reader has a single place to wait.

The `EventStorage` class is the batch of events returned by one read from the queue. The queue fills new storage; the caller reads it through `hasEvents()` and `getEvents()`.

## SingleKeyListener

The `SingleKeyListener` class is a background task which watches one key and toggles an internal condition each time that key is pressed. The key is set as a character and converted to a platform-specific key code. On Linux the key is grabbed globally with XGrabKey; on Windows the key state is polled with GetAsyncKeyState.

## KeybindListener

The `KeybindListener` class is a background task which watches keys and key combinations and pushes an event into an `EventQueue` when a binding completes. Bindings are read either from a `MIAConfig` (pairs whose keys start with a prefix, e.g. `keybind_a+b` = `increment`) or from a map of key sets to events. A binding fires when all of its keys are held at the same time, optionally together with a named modifier (`alt`, `ctrl`, or `shift`). A binding fires once per completion and does not repeat until one of its keys is released. On Linux, detection polls the physical keyboard state, so presses are seen no matter which window has focus, and keys are only captured when `captureGlobally` is set. On Windows the keys are polled with GetAsyncKeyState and never captured.

The listener also answers state queries: `isKeyHeld()` checks one key, and `isBindingHeld()` checks whether every key of a set (and its modifier, if any) is currently held.

## Build notes

The `Event` classes always build. The two key listeners are optional: they build on Windows, and on Linux only when the libxdo and X11 development libraries are found (see `cmake/xdo.cmake`).

## test

The `test/` folder contains the unit tests for this module's classes.
