/**
 * File: Attribute.hpp
 * Author: Antonius Torode
 * Created on: 07/11/2025
 * Description: Base Attribute class for character attributes with categorized 
 *     types representing different behavior patterns.
 */
#pragma once

#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>
#include <algorithm>

namespace stats
{
    class Attribute
    {
    public:
        /// Default constructor.
        Attribute() = default;
                
        /**
         * Constructs a Attribute with specified properties.
         *
         * @param id[uint32_t] - Unique identifier for the Attribute.
         * @param name[const std::string&] Human-readable name of the Attribute.
         * @param description[const std::string&] A brief description of the Attribute.
         * @param baseValue[int] - The base attribute value.
         */
        Attribute(uint32_t id,
                  const std::string& name,
                  const std::string& description,
                  int baseValue = 0) :
            id(id),
            name(name),
            description(description),
            baseValue(baseValue)
        { }

        /// Getters for the various data mambers.
        uint32_t getID() const { return id; }
        std::string getName() const { return name; }
        std::string getDescription() const { return description; }
        int getBaseValue() const { return baseValue; }

        /**
         * Serializes the Attribute object to JSON.
         *
         * @return A JSON object containing the Attribute's properties.
         */
        nlohmann::json toJson() const
        {
            return
            {
                {"id", id},
                {"name", name},
                {"description", description},
                {"baseValue", baseValue}
            };
        }

    private:
    
        uint32_t id{};                      ///< Unique identifier for this Attribute.
        std::string name;                   ///< Name of the Attribute.
        std::string description;            ///< Description of the Attribute.
        int baseValue{0};                   ///< Base value of the Attribute.
    };
} // namespace stats

