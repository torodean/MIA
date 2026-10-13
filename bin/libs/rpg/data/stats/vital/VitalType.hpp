/**
 * @file VitalType.hpp
 * @author Antonius Torode
 * @date 07/09/2025
 * @brief Storage for various vital types used for character vitals.
 */
#pragma once

#include <string>

namespace stats
{
    /**
     * Enum class representing different vital behavior types.
     */
    enum class VitalType : uint8_t
    {
        UNKNOWN = 0,
        DEPLETIVE,     ///< Starts at max and is consumed (depletes). E.g., Health, Mana.        
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
    VitalType stringToVitalType(const std::string& typeStr);

    /**
     * Converts a VitalType enum to its string representation.
     * 
     * @param type The VitalType enum value.
     * @return A string corresponding to the VitalType.
     *         Returns "UNKNOWN" if the type is not recognized.
     */
    std::string vitalTypeToString(const VitalType& type);
} // namespace stats

