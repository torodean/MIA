# RPG -> Data -> Stats -> Vital

This folder contains all of the vital related classes for this rpg system.

---

## Structure
- **Vital**: The `Vital.hpp` file defines a base `Vital` class representing a single character resource (e.g., health, mana, rage). Each vital has a behavior type (`DEPLETIVE` or `ACCUMULATIVE`), a name, description, current/max/min values, and supports JSON serialization. It includes utility functions for converting between enum types and strings.

- **VitalData**: The `VitalData.hpp` file defines the `VitalData` struct, which stores the dynamic state of a `Vital` instance, including `current`, `minimum`, and `maximum` values. It supports modifier-based adjustments using the Modifier system and includes methods to add or remove modifiers with optional recalculation. The file also defines the `VitalDataTarget` enum for specifying modifier application targets (`CURRENT`, `CURRENT_MIN`, `CURRENT_MAX`) and provides utility functions for enum-string conversion.

- **Vitals**: The `Vitals.hpp` file defines a `Vitals` class that acts as a container for multiple `Vital` instances. It provides methods to add, retrieve, update, and remove vitals by ID, name, or `Vital` object. It also includes custom serialization and deserialization logic using special markers (`[VITAL_BEGIN]` and `[VITAL_END]`) for embedding or extracting vital data from larger serialized strings.

- **VitalRegistry**: The `VitalRegistry.hpp` file defines a singleton `VitalRegistry` class for managing static `Vital` instances loaded from a JSON configuration file. It extends a generic `Registry` base and implements custom parsing and string formatting logic. Each `Vital` is constructed using validated and optionally defaulted JSON fields. Useful for loading predefined vitals (e.g., health, mana) into the system at runtime.
