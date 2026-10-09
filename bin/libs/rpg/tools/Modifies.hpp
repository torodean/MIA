/**
 * @file Modifies.hpp
 * @author Antonius Torode
 * @date 07/12/2025
 * @brief A class for storing/managing modifications to other objects in the RPG system.
 */
#pragma once

#include <iosfwd>
#include <string>

#include <nlohmann/json.hpp>

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
     * An enum class describing how a modifier combines with other modifiers already
     * attached to the same target.
     */
    enum class ModifierStackPolicy
    {
        REPLACE,        ///< A new modifier from the same source replaces the existing one.
        STACK,          ///< Modifiers accumulate alongside the ones already attached.
        KEEP_STRONGEST, ///< Only the strongest modifier of its kind is kept on the target.
        UNKNOWN         ///< Unknown or unspecified stack policy.
    };

    /**
     * Converts a ModifierStackPolicy enum to its string representation.
     *
     * @param policy The ModifierStackPolicy enum value.
     * @return A string corresponding to the ModifierStackPolicy.
     *         Returns "UNKNOWN" if the policy is not recognized.
     */
    std::string modifierStackPolicyToString(const ModifierStackPolicy& policy);

    /**
     * Converts a string to a ModifierStackPolicy enum.
     *
     * Transforms the input string to uppercase and matches it against known
     * ModifierStackPolicy values. Returns ModifierStackPolicy::UNKNOWN if the string
     * does not correspond to any valid policy.
     *
     * @param policyStr The string representation of the ModifierStackPolicy.
     * @return The corresponding ModifierStackPolicy enum value.
     */
    ModifierStackPolicy stringToModifierStackPolicy(const std::string& policyStr);

    /**
     * A struct to represent a modification to another object's value.
     */
    struct Modifies
    {
        rpg::DataType targetType;  ///< The type of the target object (e.g., "VITAL").
        std::string targetName;    ///< Name of the target object (e.g., "Health").
        ModifyType modifyType;     ///< Type of modification (e.g., ADD_MAX, MULTIPLY, SET).
        double modifyValuePer;     ///< Value applied. Potentially per unit (e.g., 5 per point of attribute).
        
        /// How this modifier combines with others on the target.
        ModifierStackPolicy stackPolicy{ModifierStackPolicy::REPLACE}; 

        /// Default constructor.
        Modifies() = default;

        /**
         * Constructor for Modifies.
         *
         * @param target The name of the target object to modify.
         * @param type The type of modification.
         * @param valuePer The value to apply per unit of the source.
         * @param policy How the modifier combines with others on the target;
         *        defaults to REPLACE.
         */
        Modifies(rpg::DataType targetType,
                 const std::string& target,
                 ModifyType type,
                 double valuePer,
                 ModifierStackPolicy policy = ModifierStackPolicy::REPLACE);

        /**
         * Equality operator for Modifies.
         *
         * Compares two Modifies objects on every field: targetType, targetName,
         * modifyType, modifyValuePer, and stackPolicy.
         *
         * @param other The Modifies object to compare with.
         * @return true if all fields are equal; false otherwise.
         */
        bool operator==(const Modifies& other) const;

        /**
         * Serializes the Modifies to a JSON object.
         *
         * @return A JSON object containing the Modifies' properties.
         */
        nlohmann::json toJson() const;

        /**
         * Deserializes a Modifies object from JSON.
         *
         * @param json The JSON object containing Modifies properties.
         * @return The constructed Modifies object.
         */
        static Modifies fromJson(const nlohmann::json& json);

        /**
         * Serializes the Modifies to a string.
         * Format: "targetType:targetName:modifyType:modifyValuePer:stackPolicy"
         *
         * @return A string representing the serialized Modifies.
         */
        std::string serialize() const;

        /**
         * Deserializes a Modifies instance from a string.
         * Expects format: "targetType:targetName:modifyType:modifyValuePer:stackPolicy"
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
     * Formats the Modifies as: "Modifies{targetName=<name>, modifyType=<type>,
     * modifyValuePer=<value>, stackPolicy=<policy>}"
     *
     * @param os The output stream to write to.
     * @param modifies The Modifies object to serialize.
     * @return The modified output stream.
     */
    std::ostream& operator<<(std::ostream& os, const Modifies& modifies);
} // namespace rpg
