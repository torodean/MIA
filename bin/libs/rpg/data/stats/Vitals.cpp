/**
 * File: Vitals.cpp
 * Author: Antonius Torode
 * Created on: 07/07/2025
 * Description: A class representing the vitals for a character.
 */

#include <iostream>
#include <sstream>
#include <limits>
#include "Vitals.hpp"

#include "VitalRegistry.hpp"

namespace stats
{
    /**
     * This namespace defines some helper methods for code which is re-used
     * through this file.
     */
    namespace helper_methods
    {
        /**
         * Helper template method to validate and retrieve a Vital object from VitalRegistry.
         *
         * @tparam T The type of the identifier (std::string for name, uint32_t for ID, or Vital for object).
         * @param identifier The identifier used to query the Vital (name, ID, or Vital object).
         * @return Pointer to the Vital object if found; nullptr otherwise.
         */
        template<typename T>
        const Vital* getVitalFromRegistry(const T& identifier)
        {
            const Vital* vital = nullptr;
            if constexpr (std::is_same_v<T, std::string>)
            {
                vital = VitalRegistry::getInstance().getByName(identifier);
            }
            else if constexpr (std::is_same_v<T, uint32_t>)
            {
                vital = VitalRegistry::getInstance().getById(identifier);
            }
            else if constexpr (std::is_same_v<T, Vital>)
            {
                vital = VitalRegistry::getInstance().getById(identifier.getID());
            }
            if (!vital)
            {
                // TODO - Add MIA Exception here.      
                return nullptr;
            }
            else
            {
                return vital;
            }
        }
    } // namespace helper_methods
    
    
    // getVitalData(..) methods.
    VitalData Vitals::getVitalData(const std::string& name) const
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        if (!vital)
        {
            return VitalData();
        }
        return getVitalData(*vital);
    }
    VitalData Vitals::getVitalData(uint32_t id) const
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        if (!vital)
        {
            return VitalData();
        }
        return getVitalData(*vital);
    }
    VitalData Vitals::getVitalData(const Vital& vital) const
    {            
        auto it = vitals.find(vital.getID());
        return it != vitals.end() ? it->second : VitalData();
    }
    
    
    // addVitalData(..) methods.
    bool Vitals::addVitalData(const std::string& name, int current, int min, int max)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        if (!vital)
        {
            return false;
        }
        return addVitalData(*vital, current, min, max);
    }
    bool Vitals::addVitalData(uint32_t id, int current, int min, int max)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        if (!vital)
        {
            return false;
        }
        return addVitalData(*vital, current, min, max);
    }
    bool Vitals::addVitalData(const Vital& vital, int current, int min, int max)
    {
        if (current < min || current > max || min > max)
        {
            // TODO - add MIA exception here.
            std::cerr << "Inconsistent value set: " 
                      << min << " < " << current << " < " << max << std::endl;
            return false;
        }
        auto id = vital.getID();
        if (vitals.find(id) != vitals.end())
        {
            // TODO - add MIA exception here.
            return false;
        }
        vitals.emplace(id, VitalData(current, min, max));
        return true;
    }
    
    
    // updateVitalCurrent(..) methods.
    bool Vitals::updateVitalCurrent(const std::string& name, int current)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        if (!vital)
        {
            return false;
        }
        return updateVitalCurrent(*vital, current);
    }
    bool Vitals::updateVitalCurrent(uint32_t id, int current)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        if (!vital)
        {
            return false;
        }
        return updateVitalCurrent(*vital, current);
    }
    bool Vitals::updateVitalCurrent(const Vital& vital, int current)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // TODO - add MIA exception here.
            return false;
        }
        int effectiveMax = getVitalEffectiveMax(vital);
        it->second.current = std::clamp(current, it->second.currentMin, effectiveMax);
        return true;
    }


    // updateVitalMin(..) methods.
    bool Vitals::updateVitalMin(const std::string& name, int min)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        if (!vital)
        {
            return false;
        }
        return updateVitalMin(*vital, min);
    }
    bool Vitals::updateVitalMin(uint32_t id, int min)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        if (!vital)
        {
            return false;
        }
        return updateVitalMin(*vital, min);
    }
    bool Vitals::updateVitalMin(const Vital& vital, int min)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // TODO - add MIA exception here.
            return false;
        }
        if (min > it->second.currentMax)
        {
            // TODO - add MIA exception here.
            return false;
        }
        it->second.currentMin = min;
        it->second.current = std::clamp(it->second.current, min, getVitalEffectiveMax(vital));
        return true;
    }


    // updateVitalMax(..) methods.
    bool Vitals::updateVitalMax(const std::string& name, int max)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        if (!vital)
        {
            return false;
        }
        return updateVitalMax(*vital, max);
    }
    bool Vitals::updateVitalMax(uint32_t id, int max)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        if (!vital)
        {
            return false;
        }
        return updateVitalMax(*vital, max);
    }
    bool Vitals::updateVitalMax(const Vital& vital, int max)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // TODO - add MIA exception here.
            return false;
        }
        if (max < it->second.currentMin)
        {
            // TODO - add MIA exception here.
            return false;
        }
        it->second.currentMax = max;
        it->second.current = std::clamp(it->second.current, it->second.currentMin, getVitalEffectiveMax(vital));
        return true;
    }
    
    
    // addVitalModifier(..) methods.
    bool Vitals::addVitalModifier(const std::string& name, uint32_t sourceId, 
                                  rpg::ModifierSourceType sourceType, int32_t value,
                                  VitalDataModifierTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        if (!vital)
        {
            return false;
        }
        return addVitalModifier(*vital, sourceId, sourceType, value, target);
    }
    bool Vitals::addVitalModifier(uint32_t id, uint32_t sourceId, 
                                  rpg::ModifierSourceType sourceType, int32_t value,
                                  VitalDataModifierTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        if (!vital)
        {
            return false;
        }
        return addVitalModifier(*vital, sourceId, sourceType, value, target);
    }
    bool Vitals::addVitalModifier(const Vital& vital, uint32_t sourceId, 
                                  rpg::ModifierSourceType sourceType, int32_t value,
                                  VitalDataModifierTarget target)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // TODO - add MIA exception here.
            return false;
        }
        if (target == VitalDataModifierTarget::CURRENT_MAX)
        {
            int effectiveMax = getVitalEffectiveMax(vital) + value;
            if (effectiveMax > std::numeric_limits<int>::max() || effectiveMax < it->second.currentMin)
            {
                // TODO - add MIA exception here.
                return false;
            }
            it->second.maxModifiers.emplace_back(sourceId, sourceType, value);
            it->second.current = std::clamp(it->second.current, getVitalEffectiveMin(vital), effectiveMax);
        }
        else if (target == VitalDataModifierTarget::CURRENT_MIN)
        {
            int effectiveMin = getVitalEffectiveMin(vital) + value;
            if (effectiveMin > it->second.currentMax)
            {
                // TODO - add MIA exception here.
                return false;
            }
            it->second.minModifiers.emplace_back(sourceId, sourceType, value);
            it->second.current = std::clamp(it->second.current, effectiveMin, getVitalEffectiveMax(vital));
        }
        else
        {
            // TODO - add MIA exception here.
            return false; // UNKNOWN target
        }
        return true;
    }


    // removeVitalModifier(..) methods.
    bool Vitals::removeVitalModifier(const std::string& name, uint32_t sourceId, 
                                     rpg::ModifierSourceType sourceType,
                                     VitalDataModifierTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        if (!vital)
        {
            return false;
        }
        return removeVitalModifier(*vital, sourceId, sourceType, target);
    }
    bool Vitals::removeVitalModifier(uint32_t id, uint32_t sourceId, 
                                     rpg::ModifierSourceType sourceType,
                                     VitalDataModifierTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        if (!vital)
        {
            return false;
        }
        return removeVitalModifier(*vital, sourceId, sourceType, target);
    }
    bool Vitals::removeVitalModifier(const Vital& vital, uint32_t sourceId, 
                                     rpg::ModifierSourceType sourceType,
                                     VitalDataModifierTarget target)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // TODO - add MIA exception here.
            return false;
        }
        
        std::vector<rpg::Modifier<int>>* modifiers = nullptr;
        if (target == VitalDataModifierTarget::CURRENT_MAX)
            modifiers = &it->second.maxModifiers;
        if (target == VitalDataModifierTarget::CURRENT_MIN)
            modifiers = &it->second.minModifiers;
        
        auto initialSize = modifiers->size();
        
        modifiers->erase(std::remove_if(modifiers->begin(), modifiers->end(),
            [sourceId, sourceType](const rpg::Modifier<int>& mod) {
                return mod.sourceId == sourceId && mod.source == sourceType;
            }), modifiers->end());
            
        if (modifiers->size() < initialSize)
        {
            it->second.current = std::clamp(it->second.current, getVitalEffectiveMin(vital), getVitalEffectiveMax(vital));
            return true;
        }
        return false;
    }


    // removeVital(..) methods.
    bool Vitals::removeVital(const std::string& name)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        if (!vital)
        {
            return false;
        }
        return removeVital(*vital);
    }
    bool Vitals::removeVital(uint32_t id)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        if (!vital)
        {
            return false;
        }
        return removeVital(*vital);
    }
    bool Vitals::removeVital(const Vital& vital)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            return false;
        }
        vitals.erase(it);
        return true;
    }
    
    
    // getVitalEffectiveMax(..) methods.
    int Vitals::getVitalEffectiveMax(const std::string& name) const
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        if (!vital)
        {
            return 0;
        }
        return getVitalEffectiveMax(*vital);
    }
    int Vitals::getVitalEffectiveMax(uint32_t id) const
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        if (!vital)
        {
            return 0;
        }
        return getVitalEffectiveMax(*vital);
    }
    int Vitals::getVitalEffectiveMax(const Vital& vital) const
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            return 0;
        }
        int effectiveMax = it->second.currentMax;
        for (const auto& mod : it->second.maxModifiers)
        {
            effectiveMax += mod.value;
        }
        return static_cast<int>(std::max<int64_t>(effectiveMax, it->second.currentMin));
    }
    
    
    // getVitalEffectiveMin(..) methods.
    int Vitals::getVitalEffectiveMin(const std::string& name) const
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        if (!vital)
        {
            return 0;
        }
        return getVitalEffectiveMin(*vital);
    }
    int Vitals::getVitalEffectiveMin(uint32_t id) const
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        if (!vital)
        {
            return 0;
        }
        return getVitalEffectiveMin(*vital);
    }
    int Vitals::getVitalEffectiveMin(const Vital& vital) const
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            return 0;
        }
        int effectiveMin = it->second.currentMin;
        for (const auto& mod : it->second.minModifiers)
        {
            effectiveMin += mod.value;
        }
        return static_cast<int>(std::min<int>(effectiveMin, it->second.currentMax));
    }
    
    
    std::string Vitals::serialize() const
    {
        std::stringstream ss;
        ss << "[CV_BEGIN]"; // Start of serialized content
        bool first = true;

        // Loop through all stored vitals
        for (const auto& [id, data] : vitals)
        {
            if (!first)
                ss << "|"; // Separate entries with '|'
            first = false;

            // Write the base vital data: <id>:<current>,<currentMin>,<currentMax>
            ss << id << ":" << data.current << "," << data.currentMin << "," << data.currentMax;

            // Serialize all min modifiers for this vital
            for (const auto& mod : data.minModifiers)
            {
                ss << ";" << mod.sourceId << "," 
                   << rpg::modifierSourceTypeToString(mod.source) << "," 
                   << mod.value << "," 
                   << vitalDataModifierTargetToString(VitalDataModifierTarget::CURRENT_MIN);
            }

            // Serialize all max modifiers for this vital
            for (const auto& mod : data.maxModifiers)
            {
                ss << ";" << mod.sourceId << "," 
                   << rpg::modifierSourceTypeToString(mod.source) << "," 
                   << mod.value << "," 
                   << vitalDataModifierTargetToString(VitalDataModifierTarget::CURRENT_MAX);
            }
        }

        ss << "[CV_END]"; // End of serialized content
        return ss.str();
    }
    

    Vitals Vitals::deserialize(const std::string& data)
    {
        size_t start = data.find("[CV_BEGIN]");
        size_t end = data.find("[CV_END]");
        if (start == std::string::npos || end == std::string::npos || end <= start + 10)
            throw std::invalid_argument("Invalid serialized data: missing or malformed [CV_BEGIN]/[CV_END]");

        std::string content = data.substr(start + 10, end - start - 10);
        Vitals vitals;
        if (content.empty())
        {
            vitals.clear();
            return vitals;
        }

        std::stringstream ss(content);
        std::string vitalEntry;

        while (std::getline(ss, vitalEntry, '|'))
        {
            std::stringstream entryStream(vitalEntry);
            std::string baseInfo;
            if (!std::getline(entryStream, baseInfo, ';'))
                throw std::invalid_argument("Malformed vital entry: missing base info");

            size_t colon = baseInfo.find(':');
            if (colon == std::string::npos)
                throw std::invalid_argument("Malformed base info: missing ':' separator");

            uint32_t id;
            try
            {
                id = std::stoul(baseInfo.substr(0, colon));
            }
            catch (...)
            {
                throw std::invalid_argument("Invalid vital id");
            }

            std::stringstream baseStream(baseInfo.substr(colon + 1));
            int current, min, max;
            char comma1, comma2;
            if (!(baseStream >> current >> comma1 >> min >> comma2 >> max) || comma1 != ',' || comma2 != ',')
                throw std::invalid_argument("Invalid vital values format");

            const Vital* vital = helper_methods::getVitalFromRegistry(id);
            if (!vital)
                throw std::invalid_argument("Vital ID not found in registry");

            if (!vitals.addVitalData(id, current, min, max))
                throw std::runtime_error("Failed to add vital data");

            std::string modStr;
            while (std::getline(entryStream, modStr, ';'))
            {
                std::vector<std::string> parts;
                std::stringstream modStream(modStr);
                std::string token;
                while (std::getline(modStream, token, ','))
                    parts.push_back(token);

                if (parts.size() != 4)
                    throw std::invalid_argument("Invalid modifier format");

                uint32_t sourceId = std::stoul(parts[0]);
                rpg::ModifierSourceType sourceType = rpg::stringToModifierSourceType(parts[1]);
                int value = std::stoi(parts[2]);
                VitalDataModifierTarget target = stringToVitalDataModifierTarget(parts[3]);

                if (target != VitalDataModifierTarget::CURRENT_MIN && target != VitalDataModifierTarget::CURRENT_MAX)
                    throw std::invalid_argument("Invalid modifier target");

                if (!vitals.addVitalModifier(id, sourceId, sourceType, value, target))
                    throw std::runtime_error("Failed to add vital modifier");
            }
        }

        return vitals;
    }

} // namespace stats
