# Program

This folder contains core program components, including error handling and related descriptions, which form the foundation for application functionality.

## Error Handling

The `Error.hpp` and `Error.cpp` files implement the `Error` class responsible for managing error codes and reporting within the application. This module centralizes error handling logic to ensure consistent error reporting and management.

## Error Descriptions

The `ErrorDescriptions.hpp` and `ErrorDescriptions.cpp` files store mappings of error codes to human-readable descriptions. This separation allows easy maintenance and extension of error messages without modifying core error handling logic.


## Application Framework

The application framework for MIA applications is defined by the `AppFramework.hpp`, `MIAApplication.cpp` and `MIAApplication.hpp` files. These provide a basic framework for applications.

## Command Option System

The `CommandOption.hpp`, `CommandOption.hpp` and `CommandOption.cpp` files provide type-safe command-line argument parsing for various possible command option types..

## Configuration System

The `MIAConfig.hpp` and `MIAConfig.cpp` files support multiple configuration formats (e.g., key-value, raw lines) with typed accessors and dynamic file path resolution for flexible, portable configuration management.

## Logging Framework

The `Logger.hpp` and `Logger.cpp` files provide a logging class and free functions (`logToDefaultFile`, `logToFile`) which enable logging to customizable files, integrated with system paths.


---

This module is designed to be extended with additional program components and core functionalities as the application grows.
