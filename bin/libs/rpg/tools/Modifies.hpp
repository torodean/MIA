/**
 * @file Modifies.hpp
 * @author Antonius Torode
 * @date 07/12/2025
 * Description: A class for storing and managing modifications to other objects in the RPG system.
 */
#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "DataType.hpp"

namespace rpg
{
    /**
     * An enum class representing the possible modification types.
     */
    enum class ModifyType
    {
        ADD,       ///< Adds a value to the target.
        MULTIPLY,  ///< Multiplies the target by a value.
        SET,       ///< Sets the target to a specific value.
        UNKNOWN    ///< Unknown or unspecified modification type.
    };

    /**
     * Converts a ModifyType enum to its string representation.
     *
     * @param type The ModifyType enum value.
     * @return A string corresponding to the ModifyType.
     *         Returns "UNKNOWN" if the type is not recognized.
     */
    inline std::string modifyTypeToString(const ModifyType& type)
    {
        switch (type)
        {
            case ModifyType::ADD:      return "ADD";
            case ModifyType::MULTIPLY: return "MULTIPLY";
            case ModifyType::SET:      return "SET";
            default:                   return "UNKNOWN";
        }
    }

    /**
     * Converts a string to a ModifyType enum.
     *
     * Transforms the input string to uppercase and matches it against known ModifyType values.
     * Returns ModifyType::UNKNOWN if the string does not correspond to any valid type.
     *
     * @param typeStr The string representation of the ModifyType.
     * @return The corresponding ModifyType enum value.
     */
    inline ModifyType stringToModifyType(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "ADD")      return ModifyType::ADD;
        if (str == "MULTIPLY") return ModifyType::MULTIPLY;
        if (str == "SET")      return ModifyType::SET;
        return ModifyType::UNKNOWN;
    }

    /**
     * A struct to represent a modification to another object's value.
     */
    struct Modifies
    {
        rpg::DataType targetType;  ///< The type of the target object (e.g., "VITAL"). 
        std::string targetName;    ///< Name of the target object (e.g., "Health").
        ModifyType modifyType;     ///< Type of modification (e.g., ADD, MULTIPLY, SET).
        double modifyValuePer;     ///< Value applied per unit (e.g., 5 per point of attribute).

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
                 double valuePer) :
            targetType(targetType),
            targetName(target), 
            modifyType(type), 
            modifyValuePer(valuePer) {}

        /**
         * Equality operator for Modifies.
         *
         * Compares two Modifies objects based on targetName and modifyType.
         * The modifyValuePer is excluded from comparison.
         *
         * @param other The Modifies object to compare with.
         * @return true if targetName and modifyType are equal; false otherwise.
         */
        bool operator==(const Modifies& other) const
        {
            return targetType == other.targetType && 
                   targetName == other.targetName && 
                   modifyType == other.modifyType &&
                   modifyValuePer == other.modifyValuePer;
        }

        /**
         * Serializes the Modifies to a string.         *
         * Format: "targetType:targetName:modifyType:modifyValuePer"
         *
         * @return A string representing the serialized Modifies.
         */
        std::string serialize() const
        {
            return rpg::dataTypeToString(targetType) + ":" + 
                   targetName + ":" + 
                   modifyTypeToString(modifyType) + ":" + 
                   std::to_string(modifyValuePer);
        }

        /**
         * Deserializes a Modifies instance from a string.         *
         * Expects format: "targetType:targetName:modifyType:modifyValuePer"
         *
         * @param data The serialized string data.
         * @return A reconstructed Modifies instance.
         * @throws std::invalid_argument if the data format is invalid.
         */
        static Modifies deserialize(const std::string& data)
        {
            std::vector<std::string> tokens;
            std::string token;
            std::istringstream tokenStream(data);
            
            while (std::getline(tokenStream, token, ':'))
                tokens.push_back(token);

            if (tokens.size() != 4)
                throw std::invalid_argument("Invalid Modifies data format");

            return Modifies(rpg::stringToDataType(tokens[0]), 
                            tokens[1], 
                            stringToModifyType(tokens[2]), 
                            std::stod(tokens[3]));
        }
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
    inline std::ostream& operator<<(std::ostream& os, const Modifies& modifies)
    {
        os << "Modifies{targetName=" << modifies.targetName
           << ", modifyType=" << modifyTypeToString(modifies.modifyType)
           << ", modifyValuePer=" << modifies.modifyValuePer << "}";
        return os;
    }
} // namespace rpg
