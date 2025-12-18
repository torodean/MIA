/**
 * @file Vital.hpp
 * @author Antonius Torode
 * @date 07/09/2025
 * Description: Base vital class for character vitals with categorized types representing different behavior patterns.
 */
#pragma once

#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>

#include "VitalType.hpp"
#include "BaseDataObject.hpp"

namespace stats
{
    /**
     * Base class representing a configurable Vital resource, inheriting from BaseDataObject.
     *
     * Vitals represent character resources such as health, mana, or rage.
     * They have a type defining their behavior (e.g., consumable or accumulative),
     * and track current, max, and min values.
     */
    class Vital : public rpg::BaseDataObject
    {
    public:
        /// Default constructor.
        Vital() = default;

        /**
         * Constructs a Vital with specified properties.
         *
         * @param id Unique identifier for the Vital.
         * @param name Human-readable name of the Vital.
         * @param description A brief description of the Vital.
         * @param type The VitalType defining behavior (DEPLETIVE or ACCUMULATIVE).
         * @param min The minimum possible value of the Vital.
         * @param max The maximum possible value of the Vital.
         */
        Vital(uint32_t id,
              const std::string& name,
              const std::string& description,
              VitalType type,
              int min = 0,
              int max = 100);

        /// Getters for the various data mambers.
        VitalType getType() const;
        int getBaseMin() const;
        int getBaseMax() const;

        /**
         * Serializes the Vital object to JSON, including base class properties.
         *
         * @return A JSON object containing the Vital's properties.
         */
        nlohmann::json toJson() const override;
        
        /**
         * Deserializes a Vital object from JSON.
         *
         * @param json The JSON object containing Vital properties.
         * @return The constructed Vital object.
         */
        static Vital fromJson(const nlohmann::json& json);

    private:
    
        VitalType type{VitalType::UNKNOWN}; ///< Behavior type of the Vital.
        int baseMin{0};                     ///< Minimum value of the Vital.
        int baseMax{100};                   ///< Maximum value of the Vital.
    };
} // namespace stats

