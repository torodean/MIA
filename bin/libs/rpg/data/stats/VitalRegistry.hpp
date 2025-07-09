/**
 * File: VitalRegistry.hpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A singleton class to manage static Vital objects loaded from a config file.
 */
#pragma once

#include "Registry.hpp"
#include "Vital.hpp"

namespace rpg
{
    class VitalRegistry : public rpg::Registry<VitalRegistry, Currency>
    {        
    protected:
    
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
            auto current = json.value("current", 0);
            auto min = json.value("min", 0);
            auto max = json.value("max", 0);

            Vital vital(id, name, description, type, current, max, min);
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
                   ", Type: " + vitalTypeToString(vital.getVitalType()) +
                   ", Current: " + vital.getCurrent() +
                   ", Max: " + vital.getMax() +
                   ", Min: " + vital.getMin() +
                   ", Description: " + vital.getDescription();
        }        
    };
} // namespace rpg
