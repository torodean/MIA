/**
 * @file Attribute.cpp
 * @author Antonius Torode
 * @date 07/11/2025
 * @brief Base Attribute class for character attributes with categorized 
 *        types representing different behavior patterns.
 */

// Include the associated header file.
#include "Attribute.hpp"

#include <algorithm>

#include "Modifies.hpp"

namespace stats
{
    Attribute::Attribute(uint32_t id,
              const std::string& name,
              const std::string& description,
              int baseValue,
              const std::vector<rpg::Modifies>& modifies) :
        BaseDataObject(id, name, description),
        baseValue(baseValue),
        modifies(modifies)
    { }

    // Getters for the various data members.
    int Attribute::getBaseValue() const { return baseValue; }
    const std::vector<rpg::Modifies>& Attribute::getModifies() const { return modifies; }


    void Attribute::addModify(const rpg::Modifies& modify)
    {
        modifies.push_back(modify);
    }
    

    nlohmann::json Attribute::toJson() const
    {
        nlohmann::json json = BaseDataObject::toJson();
        json["baseValue"] = baseValue;

        if (!modifies.empty())
        {
            nlohmann::json modifiesJson = nlohmann::json::array();
            for (const auto& modify : modifies)
                modifiesJson.push_back(modify.toJson());
            json["modifies"] = modifiesJson;
        }

        return json;
    }
    

    Attribute Attribute::fromJson(const nlohmann::json& json)
    {
        BaseDataObject base = BaseDataObject::fromJson(json);
        int baseValue = json.at("baseValue").get<int>();
        std::vector<rpg::Modifies> modifies;

        if (json.contains("modifies"))
        {
            for (const auto& modJson : json.at("modifies"))
                modifies.push_back(rpg::Modifies::fromJson(modJson));
        }

        Attribute attribute(base.getID(), base.getName(), base.getDescription(), 
                            baseValue, modifies);
        return attribute;
    }
} // namespace stats
