# Program

This folder contains core program components, including error handling and related descriptions, which form the foundation for application functionality.

## Error Handling

The `Error.hpp` and `Error.cpp` files implement the `Error` class responsible for managing error codes and reporting within the application. This module centralizes error handling logic to ensure consistent error reporting and management.

## Error Descriptions

The `ErrorDescriptions.hpp` and `ErrorDescriptions.cpp` files store mappings of error codes to human-readable descriptions. This separation allows easy maintenance and extension of error messages without modifying core error handling logic.

## MIAException

The `MIAException.hpp` file defines `MIAException`, a custom exception that carries an `ErrorCode` and a descriptive message. It inherits from `std::exception` and uses the error descriptions above to build a readable message, giving structured error propagation across the application.

## Application Framework

The application framework for MIA applications is defined by the `AppFramework.hpp`, `MIAApplication.cpp` and `MIAApplication.hpp` files. These provide a basic framework for applications.

## Command Option System

The `CommandOption.hpp` and `CommandOption.cpp` files provide type-safe command-line argument parsing for various possible command option types.

## Command Parser

The `CommandParser.hpp` and `CommandParser.cpp` files provide free functions for parsing raw `argc`/`argv` arguments into typed values (boolean flags, integer options, and similar). This is the lower-level parsing layer used to read command-line input into the command option system.

## Configuration System

The `MIAConfig.hpp` and `MIAConfig.cpp` files support multiple configuration formats (e.g., key-value, raw lines) with typed accessors and dynamic file path resolution for flexible, portable configuration management.

The configuration data backing types live in private headers used only by `MIAConfig`'s implementation:

- `ConfigData.hpp` defines the abstract `ConfigData` base class used through a PIMPL idiom to hide storage details.
- `KeyValueData.hpp` and `KeyValueData.cpp` implement `ConfigData` for key-value pair configs (entries separated by `=`).
- `RawLinesData.hpp` and `RawLinesData.cpp` implement `ConfigData` for configs stored as raw file lines.

## Logging Framework

The `Logger.hpp` and `Logger.cpp` files provide a logging class and free functions (`logToDefaultFile`, `logToFile`) which enable logging to customizable files, integrated with system paths.

## Threaded Background System

The `BackgroundTask.hpp` file introduces a generic threading utility that allows background tasks to run in separate threads with controlled lifecycle management. It defines an abstract base class that handles thread creation, termination, and safe stop signaling using atomic flags. Derived classes implement specific task logic by overriding the `run()` method, enabling consistent, reusable, and thread-safe background processing across the application.

## Constants and Paths

- `Constants.hpp` declares project-wide constants (such as the MIA version, sourced from CMake) and the `IS_WINDOWS` / `IS_LINUX` platform macros used for preprocessor checks.
- `Paths.hpp` declares project-wide directory and file paths (config directories, log locations, and similar) set at build time through CMake, so resource locations stay consistent across the codebase.

## Runtime Context

The `RuntimeContext.hpp` file defines a struct holding application runtime context shared across components: a logger instance, a verbose flag, and a debug level. It is intended to be passed to subsystems that need consistent access to application-wide settings.

## Basic Utilities

The `basic_utilities/` subfolder contains `BasicUtilities.hpp` and `BasicUtilities.cpp`, which provide lightweight, dependency-free string helpers (such as `findCharInString` and trim functions) used across the core codebase. It has its own `test/` subfolder.

## Tests

The `test/` subfolder holds GoogleTest sources for the core components, with one executable per module (e.g., `MIAException_T`, `CommandOption_T`, `CommandParser_T`, `MIAConfig_T`, `Logger_T`, `AppFramework_T`, `BackgroundTask_T`), declared in its `CMakeLists.txt`.


---

This module is designed to be extended with additional program components and core functionalities as the application grows.
