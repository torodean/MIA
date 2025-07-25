/**
 * @file Modifier.hpp
 * @author Antonius Torode
 * @date 07/10/2025
 * Description: Data for representing modifiers to object values in the RPG system.
 */
#pragma once

#include <algorithm>
#include <string>
#include <cstdint>
#include <iostream>

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
    inline std::string modifierSourceTypeToString(const ModifierSourceType& type)
    {
        switch (type)
        {
            case ModifierSourceType::ATTRIBUTE: return "ATTRIBUTE";
            case ModifierSourceType::ITEM:     return "ITEM";
            case ModifierSourceType::BUFF:     return "BUFF";
            case ModifierSourceType::DEBUFF:   return "DEBUFF";
            default:                           return "UNKNOWN";
        }
    }

    /**
     * Converts a string to a ModifierSourceType enum.
     *
     * Transforms the input string to uppercase and matches it against known ModifierSourceType values.
     * Returns ModifierSourceType::UNKNOWN if the string does not correspond to any valid type.
     *
     * @param typeStr The string representation of the ModifierSourceType.
     * @return The corresponding ModifierSourceType enum value.
     */
    inline ModifierSourceType stringToModifierSourceType(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "ATTRIBUTE") return ModifierSourceType::ATTRIBUTE;
        if (str == "ITEM")     return ModifierSourceType::ITEM;
        if (str == "BUFF")     return ModifierSourceType::BUFF;
        if (str == "DEBUFF")   return ModifierSourceType::DEBUFF;
        return ModifierSourceType::UNKNOWN;
    }
    
    /**
     * A templated struct to represent a modifier to an object's value.
     */
    template<typename T>
    struct Modifier
    {
        uint32_t sourceID;         ///< ID of the source (e.g., attribute ID, item ID).
        ModifierSourceType source; ///< Type of source (e.g., "attribute", "item", "buff").
        T value;                   ///< The modifier value (positive or negative, depending on type).

        Modifier(uint32_t id, ModifierSourceType src, T val)
            : sourceID(id), source(src), value(val) {}
            
        /**
         * Equality operator for Modifier.
         *
         * Compares two Modifier objects for equality based on their sourceID and source fields.
         * The value field is intentionally excluded from the comparison.
         *
         * @param other The Modifier object to compare with.
         * @return true if both sourceID and source are equal; false otherwise.
         */
        bool operator==(const Modifier<T>& other) const
        {
            return sourceID == other.sourceID && source == other.source;
        }

    };
            
    /**
     * Stream insertion operator for Modifier.
     * Formats the Modifier as: "Modifier{sourceID=<id>, source=<source>, value=<value>}".
     *
     * @param os The output stream to write to.
     * @param modifier The Modifier object to serialize.
     * @return The modified output stream.
     */
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Modifier<T>& modifier)
    {
        os << "Modifier{sourceID=" << modifier.sourceID
           << ", source=" << modifierSourceTypeToString(modifier.source)
           << ", value=" << modifier.value << "}";
        return os;
    }
} // namespace rpg
