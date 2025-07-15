# RPG -> Data -> Stats -> Attribute

This folder contains all of the Attribute related classes for this rpg system.

---

## Structure
- **Attribute**: TODO

- **Attributes**: TODO

- **AttributeRegistry**: The `AttributeRegistry.hpp` file defines a singleton `AttributeRegistry` class for managing static `Attribute` instances loaded from a JSON configuration file. It extends a generic `Registry` base and implements custom parsing and string formatting logic. Each `Attribute` is constructed using validated and optionally defaulted JSON fields. Useful for loading predefined Attribute (e.g., strength, constitution) into the system at runtime.
