/**
 * @file AttributeRegistry.hpp
 * @author Antonius Torode
 * @date 07/13/2025
 * Description: A registry class for managing Attribute objects in the RPG system.
 */
#pragma once

#include <sstream>

#include "Registry.hpp"
#include "Attribute.hpp"

namespace stats
{
    class AttributeRegistry : public rpg::Registry<AttributeRegistry, stats::Attribute>
    {
    protected:
        /**
         * Returns the JSON key for Attribute data array.
         * @return The string "ATTRIBUTE".
         */
        std::string getJsonKey() const override
        {
            return "ATTRIBUTE";
        }

        /**
         * Parses a JSON object into an Attribute.
         * @param json[const nlohmann::json&] - The JSON object containing Attribute data.
         * @return [stats::Attribute] - The parsed Attribute object.
         */
        stats::Attribute parseJson(const nlohmann::json& json) override
        {
            return stats::Attribute::fromJson(json);
        }

        /**
         * Converts an Attribute to a string for dumping.
         * @param obj[const stats::Attribute&] - The Attribute object.
         * @return [std::string] - String representation of the Attribute.
         */
        std::string toString(const stats::Attribute& obj) const override
        {
            std::ostringstream oss;
            oss << "Name: " << obj.getName() 
                << ", Description: " << obj.getDescription()
                << ", BaseValue: " << obj.getBaseValue();
            
            const auto& modifies = obj.getModifies();
            if (!modifies.empty())
            {
                oss << ", Modifies: [";
                for (size_t i = 0; i < modifies.size(); ++i)
                {
                    if (i > 0)
                        oss << ", ";
                    oss << "Modifies{targetType=" << dataTypeToString(modifies[i].targetType)
                        << ", targetName=" << modifies[i].targetName
                        << ", modifyType=" << modifyTypeToString(modifies[i].modifyType)
                        << ", modifyValuePer=" << modifies[i].modifyValuePer << "}";
                }
                oss << "]";
            }
            
            return oss.str();
        }
    };
} // namespace stats
