/**
 * @file Modifier.hpp
 * @author Antonius Torode
 * @date 07/10/2025
 * @brief Data for representing modifiers to object values in the RPG system.
 */
#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>
#include <variant>
#include <vector>

#include "Modifies.hpp"

namespace rpg
{
    /**
     * An enum class representing the possible source types of a modifier.
     */
    enum class ModifierSourceType
    {
        ATTRIBUTE, ///< Modifier from an attribute (e.g., strength, dexterity).
        ITEM,      ///< Modifier from an item (e.g., equipment, potion).
        BUFF,      ///< Modifier from a temporary buff effect.
        DEBUFF,    ///< Modifier from a temporary debuff effect.
        UNKNOWN    ///< Unknown or unspecified source type.
    };

    /**
     * Converts a ModifierSourceType enum to its string representation.
     *
     * @param type The ModifierSourceType enum value.
     * @return A string corresponding to the ModifierSourceType.
     *         Returns "UNKNOWN" if the type is not recognized.
     */
    std::string modifierSourceTypeToString(const ModifierSourceType& type);

    /**
     * Converts a string to a ModifierSourceType enum.
     *
     * Transforms the input string to uppercase and matches it against known ModifierSourceType values.
     * Returns ModifierSourceType::UNKNOWN if the string does not correspond to any valid type.
     *
     * @param typeStr The string representation of the ModifierSourceType.
     * @return The corresponding ModifierSourceType enum value.
     */
    ModifierSourceType stringToModifierSourceType(const std::string& typeStr);

    /**
     * A modifier to an object's value.
     *
     * A modifier is a self-contained description of one effect; the data type it is
     * attached to combines it with its own value when computing an effective value.
     * The interpretation of the value field is defined by ModifyType: ADD_MAX and SET
     * hold an int amount, MULTIPLY holds a double multiplier bonus.
     */
    struct Modifier
    {
        /// The value alternatives; an int amount for ADD_MAX and SET, a double bonus for MULTIPLY.
        using Value = std::variant<int, double>;

        uint32_t sourceID;         ///< ID of the source (e.g., attribute ID, item ID, etc).
        ModifierSourceType source; ///< Type of source (e.g., "attribute", "item", "buff").
        Value value;               ///< The modifier value; which alternative is set follows modifyType.
        ModifyType modifyType;     ///< How this modifier combines with the target value.

        /**
         * Constructs a Modifier with an int value (ADD_MAX, SET).
         *
         * @param id The ID of the source (e.g., attribute ID, item ID).
         * @param src The type of the source.
         * @param val The modifier amount.
         * @param type The modification type; defaults to ADD_MAX.
         */
        Modifier(uint32_t id, ModifierSourceType src, int val,
                 ModifyType type = ModifyType::ADD_MAX);

        /**
         * Constructs a Modifier with a double value (MULTIPLY).
         *
         * @param id The ID of the source (e.g., attribute ID, item ID).
         * @param src The type of the source.
         * @param val The modifier multiplier bonus (0.1 = +10%).
         * @param type The modification type; defaults to MULTIPLY.
         */
        Modifier(uint32_t id, ModifierSourceType src, double val,
                 ModifyType type = ModifyType::MULTIPLY);

        /**
         * Constructs a Modifier from a pre-built variant value.
         * Used when the value's alternative is chosen at runtime, e.g. deserialization.
         *
         * @param id The ID of the source (e.g., attribute ID, item ID).
         * @param src The type of the source.
         * @param val The modifier value.
         * @param type The modification type.
         */
        Modifier(uint32_t id, ModifierSourceType src, Value val, ModifyType type);

        /**
         * Returns the modifier value as an int (ADD_MAX, SET).
         *
         * @return The int value stored in the modifier.
         * @throws error::MIAException if the value holds a double instead of an int.
         */
        int getValueAsInt() const;

        /**
         * Returns the modifier value as a double (MULTIPLY).
         *
         * @return The double value stored in the modifier.
         * @throws error::MIAException if the value holds an int instead of a double.
         */
        double getValueAsDouble() const;

        /**
         * Equality operator for Modifier.
         *
         * Compares two Modifier objects by sourceID, source, and modifyType, which identify
         * one effect from one source. The value field is intentionally excluded: two modifiers
         * from the same source with the same type are the same effect, so re-adding one with a
         * new value replaces the old one rather than stacking alongside it.
         *
         * @param other The Modifier object to compare with.
         * @return true if sourceID, source, and modifyType are equal; false otherwise.
         */
        bool operator==(const Modifier& other) const;
    }; // struct Modifier

    /**
     * Computes a base value with modifiers applied in three fixed phases: every MULTIPLY
     * scales the base first, then every ADD_MAX value is summed on, then a SET override
     * wins (the last attached SET takes effect). Applying multipliers before additions
     * keeps large additive stacks from amplifying each other. The result is rounded and
     * clamped to the range of int.
     *
     * @param base The unmodified value.
     * @param modifiers The modifiers to apply.
     * @return The base value with every modifier applied.
     */
    int computeModifiedValue(int base, const std::vector<Modifier>& modifiers);

    /**
     * Stream insertion operator for Modifier.
     * Formats the Modifier as:
     * "Modifier{sourceID=<id>, source=<source>, value=<value>, type=<type>}".
     *
     * @param os The output stream to write to.
     * @param modifier The Modifier object to serialize.
     * @return The modified output stream.
     */
    std::ostream& operator<<(std::ostream& os, const Modifier& modifier);
} // namespace rpg
