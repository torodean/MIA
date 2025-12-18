/**
 * @file Attribute.hpp
 * @author Antonius Torode
 * @date 07/11/2025
 * Description: Base Attribute class for character attributes with categorized 
 *     types representing different behavior patterns.
 */
#pragma once

#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>

#include "Modifies.hpp"
#include "BaseDataObject.hpp"

namespace stats
{
    class Attribute : public rpg::BaseDataObject
    {
    public:
        /// Default constructor.
        Attribute() = default;
                
        /**
         * Constructs an Attribute with specified properties.
         *
         * @param id[uint32_t] - Unique identifier for the Attribute.
         * @param name[const std::string&] Human-readable name of the Attribute.
         * @param description[const std::string&] A brief description of the Attribute.
         * @param baseValue[int] - The base attribute value.
         * @param modifies[std::vector<rpg::Modifies>] - The modifications this attribute applies.
         */
        Attribute(uint32_t id,
                  const std::string& name,
                  const std::string& description,
                  int baseValue = 0,
                  const std::vector<rpg::Modifies>& modifies = {});

        /// Getters for the various data mambers.
        int getBaseValue() const;
        const std::vector<rpg::Modifies>& getModifies() const;

        /**
         * Adds a modification to the attribute.
         *
         * @param modify The Modifies object to add.
         */
        void addModify(const rpg::Modifies& modify);
        
        /**
         * Serializes the Attribute object to JSON.
         *
         * @return A JSON object containing the Attribute's properties.
         */
        nlohmann::json toJson() const;
        
        /**
         * Deserializes an Attribute object from JSON.
         *
         * @param j The JSON object containing Attribute properties.
         * @return The constructed Attribute object.
         */
        static Attribute fromJson(const nlohmann::json& j);

    private:
    
        int baseValue{0};               ///< Base value of the Attribute.
        std::vector<rpg::Modifies> modifies; ///< This stores which other rpg objects this attribute modifies.
    };
} // namespace stats

