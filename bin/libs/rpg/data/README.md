# RPG - data folder

This folder contains the various data storage classes for the rpg elements.
  
## Structure
- **armor/**: Contains the various armor types.
  
- **classes/**: Contains the various classes.
  
- **currencies/**: Contains the various currency related classes.

- **items/**: Contains item classes.

- **professions/**: Contains the various professions.

- **progression/**: Contains the various values to keep track of progression trackers.

- **stats/**: Contains the various stat-related classes. 

---
  
## DataLoader

The `DataLoader.hpp` and `DataLoader.cpp` files contain a singleton class to manage loading of all game data registries from configuration files, ensuring centralized and consistent initialization of RPG data.

---

## BaseDataObject

Defines a base class for RPG data objects with core identifying information.

- Provides constructors and accessors for ID, name, and description.
- Implements `toJson()` and `fromJson()` methods for JSON serialization and deserialization.
- Designed to be subclassed by domain-specific data types (e.g., attributes, vitals).
- Encapsulates metadata shared across all RPG data elements.

---

## BaseDataObjectStorage

A generic abstract container class for managing collections of data objects.

- Template class parameterized by object type and associated storage type.
- Defines a `DataStorageMap` for storing and accessing data by ID.
- Requires derived classes to implement:
    - Object lookup by name, ID, or object reference.
    - Custom serialization logic.
    - A `dump()` method for debugging output.
- Provides protected access to the internal storage map via `getMap()` for extension or inspection.
