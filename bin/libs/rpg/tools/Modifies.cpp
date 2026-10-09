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

    Modifies::Modifies(rpg::DataType targetType,
                       const std::string& target,
                       ModifyType type,
                       double valuePer)
        : targetType(targetType),
          targetName(target),
          modifyType(type),
          modifyValuePer(valuePer) {}

    bool Modifies::operator==(const Modifies& other) const
    {
        return targetType == other.targetType &&
               targetName == other.targetName &&
               modifyType == other.modifyType &&
               modifyValuePer == other.modifyValuePer;
    }

    std::string Modifies::serialize() const
    {
        return rpg::dataTypeToString(targetType) + ":" +
               targetName + ":" +
               modifyTypeToString(modifyType) + ":" +
               std::to_string(modifyValuePer);
    }

    Modifies Modifies::deserialize(const std::string& data)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(data);

        while (std::getline(tokenStream, token, ':'))
            tokens.push_back(token);

        if (tokens.size() != 4)
            MIA_THROW(error::Invalid_RPG_Data, "Invalid Modifies data format");

        return Modifies(rpg::stringToDataType(tokens[0]),
                        tokens[1],
                        stringToModifyType(tokens[2]),
                        std::stod(tokens[3]));
    }

    std::ostream& operator<<(std::ostream& os, const Modifies& modifies)
    {
        os << "Modifies{targetName=" << modifies.targetName
           << ", modifyType=" << modifyTypeToString(modifies.modifyType)
           << ", modifyValuePer=" << modifies.modifyValuePer << "}";
        return os;
    }
} // namespace rpg
