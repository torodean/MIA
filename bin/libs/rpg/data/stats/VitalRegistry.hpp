/**
 * @file VitalRegistry.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A singleton class to manage static Vital objects loaded from a config file.
 */
#pragma once

#include "Registry.hpp"
#include "Vital.hpp"

namespace stats
{
    class VitalRegistry : public rpg::Registry<VitalRegistry, Vital>
    {        
    protected:
    
        /// Returns the JSON key for the class's data array.
        std::string getJsonKey() const override { return "vital"; }
    
        /**
         * Parses a JSON object into a Vital instance.
         * Uses the Vital constructor to set all fields, preserving encapsulation.
         * @param json The JSON object representing a vital.
         * @return A Vital object initialized with the JSON data.
         */
        Vital parseJson(const nlohmann::json& json) override
        {
            auto id = json.at("id").get<uint32_t>(); // use at() to require id
            auto name = json.at("name").get<std::string>();
            auto description = json.value("description", "");
            auto type = stringToVitalType(json.value("type", "UNKNOWN"));
            auto BaseMin = json.value("BaseMin", 0);
            auto BaseMax = json.value("BaseMax", 100);

            Vital vital(id, name, description, type, BaseMin, BaseMax);
            return vital;
        }
        
        /**
         * Converts a Vital object into a human-readable string.
         * Uses public getter methods to access Vital fields.
         * @param vital The Vital object to convert.
         * @return A string representation of the Vital.
         */
        std::string toString(const Vital& vital) const override
        {
            return "Name: " + vital.getName() +
                   ", Type: " + vitalTypeToString(vital.getType()) +
                   ", BaseMin: " + std::to_string(vital.getBaseMin()) +
                   ", BaseMax: " + std::to_string(vital.getBaseMax()) +
                   ", Description: " + vital.getDescription();
        }        
    };
} // namespace stats
