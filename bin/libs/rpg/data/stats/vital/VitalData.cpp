/**
 * @file VitalData.cpp
 * @author Antonius Torode
 * @date 07/12/2025
 * @brief A class representing configurable vital Data for storing an active vital.
 */

#include <algorithm>
#include <string>
#include <vector>

#include "VitalData.hpp"

// Used for exception and error handling.
#include "MIAException.hpp"
#include "Error.hpp"

namespace stats
{
    std::string VitalDataTargetToString(const VitalDataTarget& type)
    {
        switch (type)
        {
            case VitalDataTarget::CURRENT:     return "CURRENT";
            case VitalDataTarget::CURRENT_MIN: return "CURRENT_MIN";
            case VitalDataTarget::CURRENT_MAX: return "CURRENT_MAX";
            default:                           return "UNKNOWN";
        }
    }


    VitalDataTarget stringToVitalDataTarget(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "CURRENT")     return VitalDataTarget::CURRENT;
        if (str == "CURRENT_MIN") return VitalDataTarget::CURRENT_MIN;
        if (str == "CURRENT_MAX") return VitalDataTarget::CURRENT_MAX;
        return VitalDataTarget::UNKNOWN;
    }


    VitalData::VitalData(int curr, int baseMin, int baseMax)
        : current(curr), baseMin(baseMin), baseMax(baseMax) {}


    VitalData::VitalData(VitalType type, int baseMin, int baseMax)
        : baseMin(baseMin), baseMax(baseMax)
    {
        if (type == VitalType::ACCUMULATIVE)
            current = baseMin;
        else if (type == VitalType::DEPLETIVE)
            current = baseMax;
        else if (type == VitalType::UNKNOWN)
            current = int( (baseMax - baseMin)/2 ); // Default to half-max.
    }


    VitalData::VitalData(int curr,
              int baseMin,
              int baseMax,
              const std::vector<rpg::Modifier>& maxMods,
              const std::vector<rpg::Modifier>& minMods)
        : current(curr),
          baseMin(baseMin),
          baseMax(baseMax),
          maxModifiers(maxMods),
          minModifiers(minMods) {}


    void VitalData::addMaxModifier(const rpg::Modifier& mod)
    {
        addModifier(mod, VitalDataTarget::CURRENT_MAX);
    }


    void VitalData::addModifier(const rpg::Modifier& mod, VitalDataTarget target)
    {
        std::vector<rpg::Modifier>* modifiers = nullptr;

        if (target == VitalDataTarget::CURRENT_MAX)
            modifiers = &maxModifiers;
        else if (target == VitalDataTarget::CURRENT_MIN)
            modifiers = &minModifiers;
        else
            return;

        // The same source, source type, and modify type is the same effect, so replace it.
        auto it = std::find(modifiers->begin(), modifiers->end(), mod);
        if (it != modifiers->end())
            modifiers->erase(it);

        modifiers->push_back(mod);
    }


    void VitalData::removeModifier(const rpg::Modifier& mod, VitalDataTarget target)
    {
        std::vector<rpg::Modifier>* modifiers = nullptr;

        if (target == VitalDataTarget::CURRENT_MAX)
            modifiers = &maxModifiers;
        else if (target == VitalDataTarget::CURRENT_MIN)
            modifiers = &minModifiers;
        else
            return;

        auto match = [&](const rpg::Modifier& m) {
            return m.sourceID == mod.sourceID && m.source == mod.source;
        };

        modifiers->erase(std::remove_if(modifiers->begin(), modifiers->end(), match),
                         modifiers->end());
    }


    const std::vector<rpg::Modifier>& VitalData::getModifiers(VitalDataTarget target) const
    {
        switch (target)
        {
            case VitalDataTarget::CURRENT_MIN:
                return minModifiers;
            case VitalDataTarget::CURRENT_MAX:
                return maxModifiers;
            default:
                MIA_THROW(error::Invalid_RPG_Data,
                          "Invalid target for getModifiers: " + VitalDataTargetToString(target));
        }
    }


    int VitalData::getCurrent() const
    {
        // The stored current value must always be read within the effective bounds.
        return std::clamp(current, getCurrentMin(), getCurrentMax());
    }


    void VitalData::setCurrent(int curr)
    {
        current = std::clamp(curr, getCurrentMin(), getCurrentMax());
    }


    int VitalData::getCurrentMin() const
    {
        // A minimum may never rise above the effective maximum.
        return std::min(computeModifiedValue(baseMin, minModifiers),
                        getCurrentMax());
    }


    int VitalData::getCurrentMax() const
    {
        return computeModifiedValue(baseMax, maxModifiers);
    }
} // namespace stats
