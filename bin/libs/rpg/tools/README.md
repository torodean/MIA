# RPG -> Tools

This folder contains utility components used in the RPG system. It is structured to accommodate further additions.

---

## Modifier

Contains data for representing modifiers to vital values in the RPG system.

- Defines the `ModifierSourceType` enum class to specify sources of modifiers such as attributes, items, buffs, and debuffs.
- Provides conversion functions between `ModifierSourceType` and string representations.
- Implements a templated `Modifier<T>` struct representing a modifier with source ID, type, and value.
- Overloads the stream insertion operator for easy printing of `Modifier` instances.

---

## Registry

A templated base class for managing registries of game objects in the MIA RPG system.

- Implements a singleton pattern using CRTP to manage registries for different object types.
- Supports loading registry data from JSON files or JSON strings.
- Provides retrieval of objects by ID or name.
- Requires derived classes to implement JSON parsing and object-to-string conversion.
- Maintains internal maps of objects indexed by ID and name.
