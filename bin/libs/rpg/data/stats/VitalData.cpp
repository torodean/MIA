/**
 * @file VitalData.cpp
 * @author Antonius Torode
 * @date 07/12/2025
 * Description: A class representing configurable vital Data for storing an active vital.
 */

#include <algorithm>
#include <string>
#include <vector>

#include "VitalData.hpp"

namespace stats
{
    std::string VitalDataTargetToString(const VitalDataTarget& type)
    {
        switch (type)
        {
            case VitalDataTarget::UNKNOWN:     return "UNKNOWN";
            case VitalDataTarget::CURRENT_MIN: return "CURRENT_MIN";
            case VitalDataTarget::CURRENT_MAX: return "CURRENT_MAX";
            default:                                  return "UNKNOWN";
        }
    }


    VitalDataTarget stringToVitalDataTarget(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "UNKNOWN")     return VitalDataTarget::UNKNOWN;
        if (str == "CURRENT_MIN") return VitalDataTarget::CURRENT_MIN;
        if (str == "CURRENT_MAX") return VitalDataTarget::CURRENT_MAX;
        return VitalDataTarget::UNKNOWN;
    }


    VitalData::VitalData(int c, int cMin, int cMax)
        : current(c), currentMin(cMin), currentMax(cMax) {}


    VitalData::VitalData(VitalType type, int cMin, int cMax)
        : currentMin(cMin), currentMax(cMax)
    {
        if (type == VitalType::ACCUMULATIVE)
            current = currentMin;
        else if (type == VitalType::DEPLETIVE)
            current = currentMax;
        else if (type == VitalType::UNKNOWN)
            current = int( (currentMax - currentMin)/2 ); // Default to half-max.
    }
    

    VitalData::VitalData(int c,
              int cMin,
              int cMax,
              const std::vector<rpg::Modifier<int>>& maxMods,
              const std::vector<rpg::Modifier<int>>& minMods)
        : current(c),
          currentMin(cMin),
          currentMax(cMax),
          maxModifiers(maxMods),
          minModifiers(minMods) {}
        

    void VitalData::addModifier(const rpg::Modifier<int>& mod, VitalDataTarget target, bool recalc)
    {
        if (target == VitalDataTarget::CURRENT_MAX)
        {
            maxModifiers.push_back(mod);
            
            if (recalc)
                recalculate(mod, target);
        }
        else if (target == VitalDataTarget::CURRENT_MIN)
        {
            minModifiers.push_back(mod);
            
            if (recalc)
                recalculate(mod, target);
        }
    }


    void VitalData::removeModifier(const rpg::Modifier<int>& mod, VitalDataTarget target)
    {
        auto match = [&](const rpg::Modifier<int>& m) {
            return m.sourceID == mod.sourceID && m.source == mod.source;
        };

        if (target == VitalDataTarget::CURRENT_MAX)
        {
            for (auto it = maxModifiers.begin(); it != maxModifiers.end(); )
            {
                if (match(*it))
                {
                    rpg::Modifier<int> modOut = mod;
                    modOut.value = 0 - it->value; // Negative because removing it.
                    it = maxModifiers.erase(it);
                    recalculate(modOut, target);
                }
                else
                    ++it;
            }
            recalculate(mod, target);
        }
        else if (target == VitalDataTarget::CURRENT_MIN)
        {
            for (auto it = minModifiers.begin(); it != minModifiers.end(); )
            {
                if (match(*it))
                {
                    rpg::Modifier<int> modOut = mod;
                    modOut.value = 0 - it->value; // Negative because removing it.
                    it = minModifiers.erase(it);
                    recalculate(modOut, target);
                }
                else
                    ++it;
            }
        }
    }
    
    
    const std::vector<rpg::Modifier<int>>& VitalData::getModifiers(VitalDataTarget target) const
    {
        switch (target)
        {
            case VitalDataTarget::CURRENT_MIN:
                return minModifiers;
            case VitalDataTarget::CURRENT_MAX:
                return maxModifiers;
            default:
                throw std::invalid_argument("Invalid target for getModifiers: " + VitalDataTargetToString(target));
        }
    }
    

    void VitalData::recalculate(const rpg::Modifier<int>& mod, VitalDataTarget target)
    {

        if (target == VitalDataTarget::CURRENT_MAX)
            currentMax += mod.value;
        else if (target == VitalDataTarget::CURRENT_MIN)
            currentMin += mod.value;
        // TODO - add sanity checks for values here.
    }
} // namespace stats
