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
#include <algorithm>

#include "Modifies.hpp"

namespace stats
{
    class Attribute
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
                  const std::vector<rpg::Modifies>& modifies = {}) :
            id(id),
            name(name),
            description(description),
            baseValue(baseValue),
            modifies(modifies)
        { }

        /// Getters for the various data mambers.
        uint32_t getID() const { return id; }
        std::string getName() const { return name; }
        std::string getDescription() const { return description; }
        int getBaseValue() const { return baseValue; }
        const std::vector<rpg::Modifies>& getModifies() const { return modifies; }

        /**
         * Adds a modification to the attribute.
         *
         * @param modify The Modifies object to add.
         */
        void addModify(const rpg::Modifies& modify)
        {
            modifies.push_back(modify);
        }
        
        /**
         * Serializes the Attribute object to JSON.
         *
         * @return A JSON object containing the Attribute's properties.
         */
        nlohmann::json toJson() const
        {
            nlohmann::json j = {
                {"id", id},
                {"name", name},
                {"description", description},
                {"baseValue", baseValue}
            };

            if (!modifies.empty())
            {
                nlohmann::json modifiesJson = nlohmann::json::array();
                for (const auto& modify : modifies)
                {
                    modifiesJson.push_back({
                        {"targetType", rpg::dataTypeToString(modify.targetType)},
                        {"targetName", modify.targetName},
                        {"ModifyType", rpg::modifyTypeToString(modify.modifyType)},
                        {"ModifyValuePer", modify.modifyValuePer}
                    });
                }
                j["modifies"] = modifiesJson;
            }

            return j;
        }
        
        /**
         * Deserializes an Attribute object from JSON.
         *
         * @param j The JSON object containing Attribute properties.
         * @return The constructed Attribute object.
         */
        static Attribute fromJson(const nlohmann::json& j)
        {
            uint32_t id = j.at("id").get<uint32_t>();
            std::string name = j.at("name").get<std::string>();
            std::string description = j.at("description").get<std::string>();
            int baseValue = j.at("baseValue").get<int>();
            std::vector<rpg::Modifies> modifies;

            if (j.contains("modifies"))
            {
                for (const auto& modJson : j.at("modifies"))
                {
                    rpg::Modifies mod(
                        rpg::stringToDataType(modJson.at("targetType").get<std::string>()),
                        modJson.at("targetName").get<std::string>(),
                        rpg::stringToModifyType(modJson.at("ModifyType").get<std::string>()),
                        modJson.at("ModifyValuePer").get<double>()
                    );
                    modifies.push_back(mod);
                }
            }

            return Attribute(id, name, description, baseValue, modifies);
        }

    private:
    
        uint32_t id{};                  ///< Unique identifier for this Attribute.
        std::string name;               ///< Name of the Attribute.
        std::string description;        ///< Description of the Attribute.
        int baseValue{0};               ///< Base value of the Attribute.
        std::vector<rpg::Modifies> modifies; ///< This stores which other rpg objects this attribute modifies.
    };
} // namespace stats

