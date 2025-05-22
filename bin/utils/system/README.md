# Utilities - System

This folder contains system-related utility functions designed to support common system-level operations across applications.

## Timing

The `Timing.hpp` and `Timing.cpp` files provide a set of cross-platform utilities related to time management and thread pausing. The module is designed to offer precise control over execution delays and timing operations, and is intended to be expanded with additional time-related functions as needed. These functions use C++ standard library threading and chrono utilities to ensure consistent behavior on Windows and Linux platforms.

## VirtualKeyStrokes

The VirtualKeyStrokes module provides a cross-platform interface for simulating keyboard and mouse input. It supports low-level key press emulation, string typing, mouse movement, and basic UI interaction on both Windows and Linux (via xdotool). The module is designed to abstract platform-specific APIs and expose a consistent interface for automation and scripting tasks. It includes specialized routines for games and applications, which should eventually be migrated into application-specific modules.

---

This module is intended to be extended with additional system-level utilities as needed.
