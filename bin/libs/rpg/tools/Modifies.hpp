/**
 * @file Modifies.hpp
 * @author Antonius Torode
 * @date 07/12/2025
 * @brief A class for storing/managing modifications to other objects in the RPG system.
 */
#pragma once

#include <iosfwd>
#include <string>

#include "DataType.hpp"

namespace rpg
{
    /**
     * An enum class representing the possible modification types.
     */
    enum class ModifyType
    {
        ADD_MAX,   ///< Adds the value to the target value.
        MULTIPLY,  ///< Scales the target by (1 + value), e.g. 0.1 = +10%.
        SET,       ///< Sets the target to the value.
        UNKNOWN    ///< Unknown or unspecified modification type.
    };

    /**
     * Converts a ModifyType enum to its string representation.
     *
     * @param type The ModifyType enum value.
     * @return A string corresponding to the ModifyType.
     *         Returns "UNKNOWN" if the type is not recognized.
     */
    std::string modifyTypeToString(const ModifyType& type);

    /**
     * Converts a string to a ModifyType enum.
     *
     * Transforms the input string to uppercase and matches it against known ModifyType values.
     * Returns ModifyType::UNKNOWN if the string does not correspond to any valid type.
     *
     * @param typeStr The string representation of the ModifyType.
     * @return The corresponding ModifyType enum value.
     */
    ModifyType stringToModifyType(const std::string& typeStr);

    /**
     * A struct to represent a modification to another object's value.
     */
    struct Modifies
    {
        rpg::DataType targetType;  ///< The type of the target object (e.g., "VITAL").
        std::string targetName;    ///< Name of the target object (e.g., "Health").
        ModifyType modifyType;     ///< Type of modification (e.g., ADD_MAX, MULTIPLY, SET).
        double modifyValuePer;     ///< Value applied. Potentially per unit (e.g., 5 per point of attribute).

        /// Default constructor.
        Modifies() = default;

        /**
         * Constructor for Modifies.
         *
         * @param target The name of the target object to modify.
         * @param type The type of modification.
         * @param valuePer The value to apply per unit of the source.
         */
        Modifies(rpg::DataType targetType,
                 const std::string& target,
                 ModifyType type,
                 double valuePer);

        /**
         * Equality operator for Modifies.
         *
         * Compares two Modifies objects on every field: targetType, targetName,
         * modifyType, and modifyValuePer.
         *
         * @param other The Modifies object to compare with.
         * @return true if all fields are equal; false otherwise.
         */
        bool operator==(const Modifies& other) const;

        /**
         * Serializes the Modifies to a string.
         * Format: "targetType:targetName:modifyType:modifyValuePer"
         *
         * @return A string representing the serialized Modifies.
         */
        std::string serialize() const;

        /**
         * Deserializes a Modifies instance from a string.
         * Expects format: "targetType:targetName:modifyType:modifyValuePer"
         *
         * @param data The serialized string data.
         * @return A reconstructed Modifies instance.
         * @throws error::MIAException if the data format is invalid.
         */
        static Modifies deserialize(const std::string& data);
    };

    /**
     * Stream insertion operator for Modifies.
     *
     * Formats the Modifies as: "Modifies{targetName=<name>, modifyType=<type>, modifyValuePer=<value>}"
     *
     * @param os The output stream to write to.
     * @param modifies The Modifies object to serialize.
     * @return The modified output stream.
     */
    std::ostream& operator<<(std::ostream& os, const Modifies& modifies);
} // namespace rpg
