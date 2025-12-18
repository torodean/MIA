/**
 * @file VitalType.hpp
 * @author Antonius Torode
 * @date 07/09/2025
 * Description: Storage for various vital types used for character vitals.
 */
#pragma once

#include <string>
#include <algorithm>

#include "BaseDataObject.hpp"

namespace stats
{
    /**
     * Enum class representing different vital behavior types.
     */
    enum class VitalType : uint8_t
    {
        UNKNOWN = 0,
        DEPLETIVE,    ///< Starts at max and is consumed (depletes). E.g., Health, Mana.        
        ACCUMULATIVE,  ///< Starts at zero and builds up (increases). E.g., Rage, combo points.
    };

    /**
     * Converts a string to a VitalType enum.
     * 
     * Transforms the input string to uppercase and matches it against known VitalType values.
     * Returns VitalType::UNKNOWN if the string does not correspond to any valid type.
     * 
     * @param typeStr The string representation of the VitalType.
     * @return The corresponding VitalType enum value.
     */
    inline VitalType stringToVitalType(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "DEPLETIVE")    return VitalType::DEPLETIVE;
        if (str == "ACCUMULATIVE") return VitalType::ACCUMULATIVE;
        return VitalType::UNKNOWN;
    }

    /**
     * Converts a VitalType enum to its string representation.
     * 
     * @param type The VitalType enum value.
     * @return A string corresponding to the VitalType.
     *         Returns "UNKNOWN" if the type is not recognized.
     */
    inline std::string vitalTypeToString(const VitalType& type)
    {
        switch (type)
        {
            case VitalType::DEPLETIVE:     return "DEPLETIVE";
            case VitalType::ACCUMULATIVE:  return "ACCUMULATIVE";
            default:                       return "UNKNOWN";
        }
    }
} // namespace stats

