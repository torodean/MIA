/**
 * @file Modifies.cpp
 * @author Antonius Torode
 * @date 10/08/2026
 * @brief Implements the modification data declared in Modifies.hpp.
 */

#include "Modifies.hpp"

// Used for exception and error handling.
#include "MIAException.hpp"
#include "Error.hpp"

#include <algorithm>
#include <cctype>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace rpg
{
    std::string modifyTypeToString(const ModifyType& type)
    {
        switch (type)
        {
            case ModifyType::ADD_MAX:  return "ADD_MAX";
            case ModifyType::MULTIPLY: return "MULTIPLY";
            case ModifyType::SET:      return "SET";
            default:                   return "UNKNOWN";
        }
    }

    ModifyType stringToModifyType(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "ADD_MAX")  return ModifyType::ADD_MAX;
        if (str == "MULTIPLY") return ModifyType::MULTIPLY;
        if (str == "SET")      return ModifyType::SET;
        return ModifyType::UNKNOWN;
    }

    std::string modifierStackPolicyToString(const ModifierStackPolicy& policy)
    {
        switch (policy)
        {
            case ModifierStackPolicy::REPLACE:        return "REPLACE";
            case ModifierStackPolicy::STACK:          return "STACK";
            case ModifierStackPolicy::KEEP_STRONGEST: return "KEEP_STRONGEST";
            default:                                  return "UNKNOWN";
        }
    }

    ModifierStackPolicy stringToModifierStackPolicy(const std::string& policyStr)
    {
        std::string str = policyStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "REPLACE")        return ModifierStackPolicy::REPLACE;
        if (str == "STACK")          return ModifierStackPolicy::STACK;
        if (str == "KEEP_STRONGEST") return ModifierStackPolicy::KEEP_STRONGEST;
        return ModifierStackPolicy::UNKNOWN;
    }

    Modifies::Modifies(rpg::DataType targetType,
                       const std::string& target,
                       ModifyType type,
                       double valuePer,
                       ModifierStackPolicy policy)
        : targetType(targetType),
          targetName(target),
          modifyType(type),
          modifyValuePer(valuePer),
          stackPolicy(policy) {}

    bool Modifies::operator==(const Modifies& other) const
    {
        return targetType == other.targetType &&
               targetName == other.targetName &&
               modifyType == other.modifyType &&
               modifyValuePer == other.modifyValuePer &&
               stackPolicy == other.stackPolicy;
    }

    nlohmann::json Modifies::toJson() const
    {
        return {
            {"targetType", rpg::dataTypeToString(targetType)},
            {"targetName", targetName},
            {"ModifyType", rpg::modifyTypeToString(modifyType)},
            {"ModifyValuePer", modifyValuePer},
            {"StackPolicy", rpg::modifierStackPolicyToString(stackPolicy)}
        };
    }

    Modifies Modifies::fromJson(const nlohmann::json& json)
    {
        // The stack policy is optional and defaults to REPLACE.
        ModifierStackPolicy policy = ModifierStackPolicy::REPLACE;
        if (json.contains("StackPolicy"))
            policy = stringToModifierStackPolicy(
                json.at("StackPolicy").get<std::string>());

        return Modifies(rpg::stringToDataType(json.at("targetType").get<std::string>()),
                        json.at("targetName").get<std::string>(),
                        stringToModifyType(json.at("ModifyType").get<std::string>()),
                        json.at("ModifyValuePer").get<double>(),
                        policy);
    }

    std::string Modifies::serialize() const
    {
        return rpg::dataTypeToString(targetType) + ":" +
               targetName + ":" +
               modifyTypeToString(modifyType) + ":" +
               std::to_string(modifyValuePer) + ":" +
               modifierStackPolicyToString(stackPolicy);
    }

    Modifies Modifies::deserialize(const std::string& data)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(data);

        while (std::getline(tokenStream, token, ':'))
            tokens.push_back(token);

        if (tokens.size() != 5)
            MIA_THROW(error::Invalid_RPG_Data, "Invalid Modifies data format");

        double valuePer;
        try
        {
            valuePer = std::stod(tokens[3]);
        }
        catch (const std::exception&)
        {
            MIA_THROW(error::Invalid_RPG_Data, "Invalid Modifies numeric value: " + data);
        }

        return Modifies(rpg::stringToDataType(tokens[0]),
                        tokens[1],
                        stringToModifyType(tokens[2]),
                        valuePer,
                        stringToModifierStackPolicy(tokens[4]));
    }

    std::ostream& operator<<(std::ostream& os, const Modifies& modifies)
    {
        os << "Modifies{targetName=" << modifies.targetName
           << ", modifyType=" << modifyTypeToString(modifies.modifyType)
           << ", modifyValuePer=" << modifies.modifyValuePer
           << ", stackPolicy=" << modifierStackPolicyToString(modifies.stackPolicy) << "}";
        return os;
    }
} // namespace rpg
