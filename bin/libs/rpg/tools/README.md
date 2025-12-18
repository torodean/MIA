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

## Modifies

Defines data structures and utilities for representing and applying modifications to other RPG objects.

- Declares the `ModifyType` enum class to represent types of modifications (e.g., `ADD_MAX`, `MULTIPLY`, `SET`).
- Provides string conversion functions for `ModifyType`.
- Implements the Modifies struct to represent a modification with fields for target type, name, modification type, and per-unit value.
- Supports serialization and deserialization of Modifies instances to and from string representations.
- Overloads the stream insertion operator for readable output formatting.

---

## Registry

A templated base class for managing registries of game objects in the MIA RPG system.

- Implements a singleton pattern using CRTP to manage registries for different object types.
- Supports loading registry data from JSON files or JSON strings.
- Provides retrieval of objects by ID or name.
- Requires derived classes to implement JSON parsing and object-to-string conversion.
- Maintains internal maps of objects indexed by ID and name.

---

## RegistryHelper

Contains reusable helper methods for interacting with registry-managed RPG objects.

- Provides the templated `getFromRegistry` function, which validates and retrieves objects from a registry using an identifier (ID, name, or object instance).
- Throws an exception with `Undefined_RPG_Value` if the object is not found.
- Centralizes and simplifies common registry access logic across the RPG system.

---

## ModifierApplicator

Templated utility for applying modifiers from source objects (e.g., attributes) to target objects (e.g., vitals) based on declared Modifies data.

- Iterates through source storage data, retrieves associated modification rules, and computes their effect.
- Applies `ADD_MAX` type modifiers using the Modifier struct and adds them to the appropriate target.
- Designed for general use with registry-backed source/target pairs and their runtime storage types.
- Throws `MIAException` if referenced source or target objects are missing.
