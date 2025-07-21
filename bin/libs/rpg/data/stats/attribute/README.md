# RPG -> Data -> Stats -> Attribute

This folder contains all of the Attribute related classes for this rpg system.

---

## Structure
- **Attribute**: The `Attribute.hpp` file defines the base `Attribute` class, representing a single character attribute (e.g., strength, agility). It includes a base value and a list of Modifies objects indicating how the attribute affects other systems. The class supports JSON serialization and deserialization, and provides methods to access and mutate its properties.

- **AttributeData**: The `AttributeData.hpp` file defines the `AttributeData` struct, which stores the current value of an active attribute and manages runtime modifiers. It provides methods to add or remove modifiers and automatically recalculates the current value accordingly. This class is used to maintain per-instance attribute state during gameplay.

- **Attributes**: The `Attributes.hpp` file defines the `Attributes` class, a container for managing multiple `Attribute` and `AttributeData` pairs. It supports querying, adding, updating, and removing attributes by ID, name, or object reference. It also includes functionality for applying and removing modifiers, dumping debug output, and serializing/deserializing the full state using special markers ([ATTRIBUTES_BEGIN] and [ATTRIBUTES_END]) for data extraction from larger serialized blobs.

- **AttributeRegistry**: The `AttributeRegistry.hpp` file defines a singleton `AttributeRegistry` class for managing static `Attribute` instances loaded from a JSON configuration file. It extends a generic `Registry` base and implements custom parsing and string formatting logic. Each `Attribute` is constructed using validated and optionally defaulted JSON fields. Useful for loading predefined Attribute (e.g., strength, constitution) into the system at runtime.
