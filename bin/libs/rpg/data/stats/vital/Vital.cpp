/**
 * @file Vital.cpp
 * @author Antonius Torode
 * @date 07/09/2025
 * Description: Base vital class for character vitals with categorized types representing different behavior patterns.
 */

#include <algorithm>

#include "Vital.hpp"

namespace stats
{
    Vital::Vital(uint32_t id,
          const std::string& name,
          const std::string& description,
          VitalType type,
          int min,
          int max) :
        BaseDataObject(id, name, description),
        type(type),
        baseMin(min),
        baseMax(max)
    { }


    VitalType Vital::getType() const { return type; }
    int Vital::getBaseMin() const { return baseMin; }
    int Vital::getBaseMax() const { return baseMax; }


    nlohmann::json Vital::toJson() const
    {
        nlohmann::json json = BaseDataObject::toJson();
        json["type"] = vitalTypeToString(type);
        json["baseMin"] = baseMin;
        json["baseMax"] = baseMax;
        return json;
    }
    

    Vital Vital::fromJson(const nlohmann::json& json)
    {
        BaseDataObject base = BaseDataObject::fromJson(json);
        VitalType type = stringToVitalType(json.value("type", "UNKNOWN"));
        int baseMin = json.value("baseMin", 0);
        int baseMax = json.value("baseMax", 100);

        Vital vital(base.getID(), base.getName(), base.getDescription(), 
                    type, baseMin, baseMax);
        return vital;
    }
} // namespace stats

