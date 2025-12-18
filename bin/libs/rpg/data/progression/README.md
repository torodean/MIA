# RPG -> Data -> Progression

## purpose

This folder contains all of the classes and configuration related to progress markers of this rpg system.

## Structure
- **ProgressMarker**: The `ProgressMarker.hpp` file defines a base `ProgressMarker` class for representing various types of progress markers (e.g., xp, level skills, etc) in the rpg system. It includes core attributes (id, name, description). Intended as a foundation for all ProgressMarker types in the application.

- **ProgressValue**: The `ProgressValue.hpp` file defines the `ProgressValue` struct, which stores the current value of a progress marker. It provides methods to get, set, and increment the stored value. This class represents the active state of a single progress type at runtime.

- **ProgressMarkers**: The `ProgressMarkers.hpp` file defines the `ProgressMarkers` class, which manages a collection of `ProgressMarker` and `ProgressValue` pairs. It allows querying, adding, updating, removing, and checking progress markers by ID, name, or object. It also includes serialization and deserialization methods using special markers ([PROGRESS_BEGIN] and [PROGRESS_END]) for safe embedding in larger data streams.

- **ProgressRegistry**: The `ProgressRegistry.hpp` file defines a singleton `ProgressRegistry` class that loads and stores static `ProgressMarker` instances from a configuration file. It extends a generic Registry base, implements custom parsing, and supports formatted string output for each marker. Useful for loading predefined progress types (e.g., experience, currency, faction standing) at runtime.
