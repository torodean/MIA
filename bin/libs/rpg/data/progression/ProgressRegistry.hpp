/**
 * @file ProgressRegistry.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A singleton class to manage static ProgressMarker objects loaded from a config file.
 */
#pragma once

#include "Registry.hpp"
#include "ProgressMarker.hpp"

namespace progress
{
    class ProgressRegistry : public rpg::Registry<ProgressRegistry, ProgressMarker>
    {        
    protected:
    
        /// Returns the JSON key for the class's data array.
        std::string getJsonKey() const override { return "CURRENCY"; }
    
        /**
         * Parses a JSON object into a ProgressMarker instance.
         * Uses the ProgressMarker constructor to set all fields, preserving encapsulation.
         * @param json The JSON object representing a progress.
         * @return A ProgressMarker object initialized with the JSON data.
         */
        ProgressMarker parseJson(const nlohmann::json& json) override
        {
            auto id = json.at("id").get<uint32_t>(); // use at() to require id
            auto name = json.at("name").get<std::string>();
            auto description = json.value("description", "");

            ProgressMarker progress(id, name, description);
            return progress;
        }
        
        /**
         * Converts a ProgressMarker object into a human-readable string.
         * Uses public getter methods to access ProgressMarker fields.
         * @param progress The ProgressMarker object to convert.
         * @return A string representation of the ProgressMarker.
         */
        std::string toString(const ProgressMarker& progress) const override
        {
            return "Name: " + progress.getName() +
                   ", Description: " + progress.getDescription();
        }
        
    };
} // namespace progress
