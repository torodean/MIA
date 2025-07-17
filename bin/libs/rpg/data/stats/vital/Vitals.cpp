/**
 * @file Vitals.cpp
 * @author Antonius Torode
 * @date 07/07/2025
 * Description: A class representing the vitals for a character.
 */

#include <iostream>
#include <sstream>
#include <limits>
#include "Vitals.hpp"

#include "VitalRegistry.hpp"
#include "RegistryHelper.hpp"
#include "MIAException.hpp"
#include "Error.hpp"

namespace stats
{
    namespace helper_methods 
    {
        /**
         * Resolves and retrieves a Vital object from the VitalRegistry based on a given identifier.
         *
         * This is a wrapper around the generic rpg::helper_methods::getVitalFromRegistry, specialized
         * for the VitalRegistry and Vital types used within the stats namespace. It accepts an identifier
         * in the form of a name (std::string), ID (uint32_t), or a Vital object, and attempts to fetch
         * the corresponding Vital instance from the registry.
         *
         * @tparam T The type of identifier: std::string, uint32_t, or Vital.
         * @param identifier The identifier used to locate the Vital in the registry.
         * @return Pointer to the corresponding Vital object.
         * @throws MIA_THROW with Undefined_RPG_Value if the object is not found.
         */
        template<typename T>
        const Vital* getVitalFromRegistry(const T& identifier)
        {
            return rpg::helper_methods::getFromRegistry<VitalRegistry, Vital, T>(identifier);
        }

    } // namespace helper_methods

    // get(..) methods.
    const VitalData& Vitals::get(const std::string& name)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);            
        return get(*vital);
    }
    const VitalData& Vitals::get(uint32_t id)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);            
        return get(*vital);
    }
    const VitalData& Vitals::get(const Vital& vital)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // The data is not found so add a default one, then update the current.
            // TODO - setting the current here to baseMax... This may not always be best/desired.
            add(vital, vital.getBaseMax(), vital.getBaseMin(), vital.getBaseMax());
            it = vitals.find(vital.getID());
            return it->second;
        }
        else 
            return it->second;
    }
    
    
    // add(..) methods.
    void Vitals::add(const std::string& name, int current, int min, int max)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);            
        add(*vital, current, min, max);
    }
    void Vitals::add(uint32_t id, int current, int min, int max)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);            
        add(*vital, current, min, max);
    }
    void Vitals::add(const Vital& vital, int current, int min, int max)
    {
        if (current < min || current > max || min > max)
        {
            std::string err = "Inconsistent value set: " 
                            + std::to_string(min) + " < " 
                            + std::to_string(current) + " < " 
                            + std::to_string(max) + "\n";
            MIA_THROW(error::ErrorCode::Invalid_RPG_Data, err);
        }
        auto id = vital.getID();
        if (vitals.find(id) != vitals.end())            
            MIA_THROW(error::ErrorCode::Duplicate_RPG_Value);
            
        vitals.emplace(id, VitalData(current, min, max));
    }


    // update(..) methods.
    void Vitals::update(const std::string& name, VitalDataTarget target, int value)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        update(*vital, target, value);
    }
    void Vitals::update(uint32_t id, VitalDataTarget target, int value)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        update(*vital, target, value);
    }
    void Vitals::update(const Vital& vital, VitalDataTarget target, int value)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // The data is not found so add a default one, then update the current.
            // TODO - setting the current here to baseMax... This may not always be best/desired.
            add(vital, vital.getBaseMax(), vital.getBaseMin(), vital.getBaseMax());
        }
        
        if (target == VitalDataTarget::CURRENT)
        {
            it->second.setCurrent(value);
        }
        else if (target == VitalDataTarget::CURRENT_MIN)
        {
            if (value > it->second.getCurrentMax())
            {
                std::string err = "Inconsistent value set:  min(" 
                                + std::to_string(value) + ") > max(" 
                                + std::to_string(it->second.getCurrentMax()) + ")\n";
                MIA_THROW(error::ErrorCode::Invalid_RPG_Data, err);
            }
            it->second.setCurrentMin(value);
            it->second.setCurrent(it->second.getCurrent()); // If correction is needed.
        }
        else if (target == VitalDataTarget::CURRENT_MAX)
        {
            if (value < it->second.getCurrentMin())
            {
                std::string err = "Inconsistent value set:  min(" 
                                + std::to_string(it->second.getCurrentMin()) + ") > max(" 
                                + std::to_string(value) + ")\n";
                MIA_THROW(error::ErrorCode::Invalid_RPG_Data, err);
            }
            it->second.setCurrentMax(value);
            it->second.setCurrent(it->second.getCurrent()); // If correction is needed.
        }
    }
    
    
    // addModifier(..) methods.
    void Vitals::addModifier(const std::string& name, 
                             uint32_t sourceID, 
                             rpg::ModifierSourceType sourceType, 
                             int value,
                             VitalDataTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        addModifier(*vital, sourceID, sourceType, value, target);
    }
    void Vitals::addModifier(uint32_t id, 
                             uint32_t sourceID, 
                             rpg::ModifierSourceType sourceType, 
                             int value,
                             VitalDataTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        addModifier(*vital, sourceID, sourceType, value, target);
    }
    void Vitals::addModifier(const Vital& vital, 
                             uint32_t sourceID, 
                             rpg::ModifierSourceType sourceType, 
                             int value,
                             VitalDataTarget target)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // The data is not found so add a default one, then update the current.
            // TODO - setting the current here to baseMax... This may not always be best/desired.
            add(vital, vital.getBaseMax(), vital.getBaseMin(), vital.getBaseMax());
        }
        
        rpg::Modifier<int> mod = rpg::Modifier<int>(sourceID, sourceType, value);

        it->second.addModifier(mod, target);
    }


    // removeModifier(..) methods.
    void Vitals::removeModifier(const std::string& name, 
                                uint32_t sourceID, 
                                rpg::ModifierSourceType sourceType,
                                VitalDataTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        removeModifier(*vital, sourceID, sourceType, target);
    }
    void Vitals::removeModifier(uint32_t id, 
                                uint32_t sourceID, 
                                rpg::ModifierSourceType sourceType,
                                VitalDataTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        removeModifier(*vital, sourceID, sourceType, target);
    }
    void Vitals::removeModifier(const Vital& vital, 
                                uint32_t sourceID, 
                                rpg::ModifierSourceType sourceType,
                                VitalDataTarget target)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // The data is not found so no need to remove anything.
            return;
        }
        
        // removeModifier() doesn't check the value so setting it to zero here...
        rpg::Modifier<int> mod = rpg::Modifier<int>(sourceID, sourceType, 0);

        it->second.removeModifier(mod, target);
    }


    // remove(..) methods.
    void Vitals::remove(const std::string& name)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        remove(*vital);
    }
    void Vitals::remove(uint32_t id)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        remove(*vital);
    }
    void Vitals::remove(const Vital& vital)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
            return;

        vitals.erase(it);
    }
    
    bool Vitals::has(const std::string& name, int value) const    
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        return has(*vital, value);
    }
    bool Vitals::has(uint32_t id, int value) const
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        return has(*vital, value);
    }
    bool Vitals::has(const Vital& vital, int value) const
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
            return false;
            
        if (it->second.getCurrent() >= value)
            return true;
            
        return false;
    }
    
    
    std::string Vitals::serialize() const
    {
        std::stringstream ss;
        ss << "[VITALS_BEGIN]"; // Start of serialized content
        bool first = true;

        // Loop through all stored vitals
        for (const auto& [id, data] : vitals)
        {
            if (!first)
                ss << "|"; // Separate entries with '|'
            first = false;

            // Write the base vital data: <id>:<current>,<currentMin>,<currentMax>
            ss << id << ":" << data.getCurrent() << "," << data.getCurrentMin() << "," << data.getCurrentMax();

            // Serialize all min modifiers for this vital
            for (const auto& mod : data.getModifiers(VitalDataTarget::CURRENT_MIN))
            {
                ss << ";" << mod.sourceID << "," 
                   << rpg::modifierSourceTypeToString(mod.source) << "," 
                   << mod.value << "," 
                   << VitalDataTargetToString(VitalDataTarget::CURRENT_MIN);
            }

            // Serialize all max modifiers for this vital
            for (const auto& mod : data.getModifiers(VitalDataTarget::CURRENT_MAX))
            {
                ss << ";" << mod.sourceID << "," 
                   << rpg::modifierSourceTypeToString(mod.source) << "," 
                   << mod.value << "," 
                   << VitalDataTargetToString(VitalDataTarget::CURRENT_MAX);
            }
        }

        ss << "[VITALS_END]"; // End of serialized content
        return ss.str();
    }
    

    Vitals Vitals::deserialize(const std::string& data)
    {
        std::string beginString = "[VITALS_BEGIN]";
        size_t start = data.find(beginString);
        size_t end = data.find("[VITALS_END]");
        if (start == std::string::npos || end == std::string::npos || end <= start + beginString.size())
            throw std::invalid_argument("Invalid serialized data: missing or malformed [VITALS_BEGIN]/[VITALS_END]");

        std::string content = data.substr(start + beginString.size(), end - start - beginString.size());
        Vitals vitals;
        if (content.empty())
        {
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
                MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

            vitals.add(id, current, min, max);

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

                uint32_t sourceID = std::stoul(parts[0]);
                rpg::ModifierSourceType sourceType = rpg::stringToModifierSourceType(parts[1]);
                int value = std::stoi(parts[2]);
                VitalDataTarget target = stringToVitalDataTarget(parts[3]);

                if (target != VitalDataTarget::CURRENT_MIN && target != VitalDataTarget::CURRENT_MAX)
                    throw std::invalid_argument("Invalid modifier target");

                // The vitals are already calculated so this block bypasses the recaclulate() 
                // call in addModifier().
                auto& vitalData = vitals.vitals.at(id);
                rpg::Modifier<int> mod(sourceID, sourceType, value);                
                vitalData.addModifier(mod, target, false);
            }
        }

        return vitals;
    }

} // namespace stats
