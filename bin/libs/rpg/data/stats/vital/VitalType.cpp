/**
 * @file VitalType.cpp
 * @author Antonius Torode
 * @date 10/12/2026
 * @brief Storage for various vital types used for character vitals.
 */
 
// Include the associated header file.
#include "VitalType.hpp"

#include <algorithm>

namespace stats
{
    VitalType stringToVitalType(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "DEPLETIVE")    return VitalType::DEPLETIVE;
        if (str == "ACCUMULATIVE") return VitalType::ACCUMULATIVE;
        return VitalType::UNKNOWN;
    }


    std::string vitalTypeToString(const VitalType& type)
    {
        switch (type)
        {
            case VitalType::DEPLETIVE:     return "DEPLETIVE";
            case VitalType::ACCUMULATIVE:  return "ACCUMULATIVE";
            default:                       return "UNKNOWN";
        }
    }
} // namespace stats

