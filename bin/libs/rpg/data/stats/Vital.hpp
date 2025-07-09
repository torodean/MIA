/**
 * File: Vital.hpp
 * Author: Antonius Torode
 * Created on: 07/09/2025
 * Description: Base vital class for character vitals with categorized types representing different behavior patterns.
 */
#pragma once

#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>
#include <algorithm>

namespace rpg
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

    /**
     * Base class representing a configurable Vital resource.
     *
     * Vitals represent character resources such as health, mana, or rage.
     * They have a type defining their behavior (e.g., consumable or accumulative),
     * and track current, max, and min values.
     */
    class Vital
    {
    public:
        /// Default constructor.
        Vital() = default;

        /**
         * Constructs a Vital with specified properties.
         *
         * If the current value is not explicitly set:
         * - For DEPLETIVE vitals, current defaults to max (starts full).
         * - For ACCUMULATIVE vitals, current defaults to 0 (starts empty).
         *
         * @param id Unique identifier for the Vital.
         * @param name Human-readable name of the Vital.
         * @param description A brief description of the Vital.
         * @param type The VitalType defining behavior (DEPLETIVE or ACCUMULATIVE).
         * @param current The initial current value of the Vital.
         * @param max The maximum possible value of the Vital.
         * @param min The minimum possible value of the Vital.
         */
        Vital(uint32_t id,
              const std::string& name,
              const std::string& description,
              VitalType type,
              int current = 0,
              int max = 100,
              int min = 0) :
            id(id),
            name(name),
            description(description),
            type(type),
            current(current),
            max(max),
            min(min)
        {
            // Adjust starting current value based on type
            if (type == VitalType::DEPLETIVE && current == 0)
                this->current = max;  // start full if consumable and no explicit current
        }

        /// Getters for the various data mambers.
        uint32_t getID() const { return id; }
        std::string getName() const { return name; }
        std::string getDescription() const { return description; }
        VitalType getVitalType() const { return type; }
        int getCurrent() const { return current; }
        int getMax() const { return max; }
        int getMin() const { return min; }
        
        /// Setters for the changeable data members.
        void setCurrent(int val) { current = val; }
        void setMax(int val) { max = val; }
        void setMin(int val) { min = val; }

        /**
         * Serializes the Vital object to JSON.
         *
         * @return A JSON object containing the Vital's properties.
         */
        nlohmann::json toJson() const
        {
            return
            {
                {"id", id},
                {"name", name},
                {"description", description},
                {"type", vitalTypeToString(type)},
                {"current", current},
                {"max", max},
                {"min", min}
            };
        }

    private:
    
        uint32_t id{};                      ///< Unique identifier for this Vital.
        std::string name;                   ///< Name of the Vital.
        std::string description;            ///< Description of the Vital.
        VitalType type{VitalType::UNKNOWN}; ///< Behavior type of the Vital.
        int current{0};                     ///< Current value of the Vital.
        int max{100};                       ///< Maximum value of the Vital.
        int min{0};                         ///< Minimum value of the Vital.
    };
} // namespace rpg

