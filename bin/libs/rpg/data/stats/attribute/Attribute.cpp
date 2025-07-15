/**
 * @file Attribute.cpp
 * @author Antonius Torode
 * @date 07/11/2025
 * Description: Base Attribute class for character attributes with categorized 
 *     types representing different behavior patterns.
 */

#include <algorithm>

#include "Attribute.hpp"
#include "Modifies.hpp"

namespace stats
{

    Attribute::Attribute(uint32_t id,
              const std::string& name,
              const std::string& description,
              int baseValue,
              const std::vector<rpg::Modifies>& modifies) :
        id(id),
        name(name),
        description(description),
        baseValue(baseValue),
        modifies(modifies)
    { }

    // Getters for the various data mambers.
    uint32_t Attribute::getID() const { return id; }
    std::string Attribute::getName() const { return name; }
    std::string Attribute::getDescription() const { return description; }
    int Attribute::getBaseValue() const { return baseValue; }
    const std::vector<rpg::Modifies>& Attribute::getModifies() const { return modifies; }


    void Attribute::addModify(const rpg::Modifies& modify)
    {
        modifies.push_back(modify);
    }
    

    nlohmann::json Attribute::toJson() const
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
    

    Attribute Attribute::fromJson(const nlohmann::json& j)
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

} // namespace stats

