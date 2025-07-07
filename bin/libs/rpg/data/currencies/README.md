# Data -> Currencies

## purpose

This folder contains all of the classes and configuration related to currencies of this rpg system.

## Structure
- **Currency**: The `Currency.hpp` file defines a base `Currency` class and supporting enum for representing various types of currencies (e.g., coin, fiat, token) in the rpg system. It includes core attributes (name, description, type, tradeability, icon) and provides a utility function to convert strings to enum values. Intended as a foundation for all currency types in the application.
- **CurrencyContainer**: The `CurrencyContainer.hpp` file defines a container class for managing multiple currencies and their quantities. It supports adding, removing, querying, and checking currency amounts, using an internal map keyed by currency ID. Useful for inventory or wallet-like systems where various currency types must be tracked.
- **CurrencyRegistry**: The `CurrencyRegistry.hpp` file defines a singleton class for managing static `Currency` objects loaded from a configuration file. It allows lookup by currency ID or name and provides methods for loading and dumping the registry. Acts as a centralized repository for all defined currencies in the system.
- **Currencies**: The `Currencies.hpp` file declares predefined global pointers to commonly used currency instances (e.g., copperCoin, goldCoin). It includes an initialization function to set these pointers after loading currencies from the registry, enabling easy and consistent access throughout the codebase.
- **currencies.json**: A JSON configuration file listing predefined currencies with properties like name, description, type, tradeability, and icon. Used by the `CurrencyRegistry` to load and initialize static currency definitions for the RPG system.
