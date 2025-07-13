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
#include "MIAException.hpp"
#include "Error.hpp"

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
    
    
    // getData(..) methods.
    const VitalData& Vitals::getData(const std::string& name)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);
            
        return getData(*vital);
    }
    const VitalData& Vitals::getData(uint32_t id)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);
            
        return getData(*vital);
    }
    const VitalData& Vitals::getData(const Vital& vital)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // The data is not found so add a default one, then update the current.
            // TODO - setting the current here to baseMax... This may not always be best/desired.
            addData(vital, vital.getBaseMax(), vital.getBaseMin(), vital.getBaseMax());
            it = vitals.find(vital.getID());
            return it->second;
        }
        else 
            return it->second;
    }
    
    
    // addData(..) methods.
    void Vitals::addData(const std::string& name, int current, int min, int max)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);
            
        addData(*vital, current, min, max);
    }
    void Vitals::addData(uint32_t id, int current, int min, int max)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);
            
        addData(*vital, current, min, max);
    }
    void Vitals::addData(const Vital& vital, int current, int min, int max)
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


    // updateVital(..) methods.
    void Vitals::updateVital(const std::string& name, VitalDataTarget target, int value)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

        updateVital(*vital, target, value);
    }
    void Vitals::updateVital(uint32_t id, VitalDataTarget target, int value)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

        updateVital(*vital, target, value);
    }
    void Vitals::updateVital(const Vital& vital, VitalDataTarget target, int value)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // The data is not found so add a default one, then update the current.
            // TODO - setting the current here to baseMax... This may not always be best/desired.
            addData(vital, vital.getBaseMax(), vital.getBaseMin(), vital.getBaseMax());
        }
        
        if (target == VitalDataTarget::CURRENT)
        {
            it->second.current = std::clamp(value, it->second.currentMin, it->second.currentMax);
        }
        else if (target == VitalDataTarget::CURRENT_MIN)
        {
            if (value > it->second.currentMax)
            {
                std::string err = "Inconsistent value set:  min(" 
                                + std::to_string(value) + ") > max(" 
                                + std::to_string(it->second.currentMax) + ")\n";
                MIA_THROW(error::ErrorCode::Invalid_RPG_Data, err);
            }
            it->second.currentMin = value;
            it->second.current = std::clamp(it->second.current, value, it->second.currentMax);
        }
        else if (target == VitalDataTarget::CURRENT_MAX)
        {
            if (value < it->second.currentMin)
            {
                std::string err = "Inconsistent value set:  min(" 
                                + std::to_string(it->second.currentMin) + ") > max(" 
                                + std::to_string(value) + ")\n";
                MIA_THROW(error::ErrorCode::Invalid_RPG_Data, err);
            }
            it->second.currentMax = value;
            it->second.current = std::clamp(it->second.current, it->second.currentMin, value);
        }
    }
    
    
    // addVitalModifier(..) methods.
    void Vitals::addVitalModifier(const std::string& name, uint32_t sourceId, 
                                  rpg::ModifierSourceType sourceType, int value,
                                  VitalDataTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

        addVitalModifier(*vital, sourceId, sourceType, value, target);
    }
    void Vitals::addVitalModifier(uint32_t id, uint32_t sourceId, 
                                  rpg::ModifierSourceType sourceType, int value,
                                  VitalDataTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

        addVitalModifier(*vital, sourceId, sourceType, value, target);
    }
    void Vitals::addVitalModifier(const Vital& vital, uint32_t sourceId, 
                                  rpg::ModifierSourceType sourceType, int value,
                                  VitalDataTarget target)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
        {
            // The data is not found so add a default one, then update the current.
            // TODO - setting the current here to baseMax... This may not always be best/desired.
            addData(vital, vital.getBaseMax(), vital.getBaseMin(), vital.getBaseMax());
        }
        
        rpg::Modifier<int> mod = rpg::Modifier<int>(sourceId, sourceType, value);

        it->second.addModifier(mod, target);
    }


    // removeVitalModifier(..) methods.
    void Vitals::removeVitalModifier(const std::string& name, uint32_t sourceId, 
                                     rpg::ModifierSourceType sourceType,
                                     VitalDataTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

        removeVitalModifier(*vital, sourceId, sourceType, target);
    }
    void Vitals::removeVitalModifier(uint32_t id, uint32_t sourceId, 
                                     rpg::ModifierSourceType sourceType,
                                     VitalDataTarget target)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

        removeVitalModifier(*vital, sourceId, sourceType, target);
    }
    void Vitals::removeVitalModifier(const Vital& vital, uint32_t sourceId, 
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
        rpg::Modifier<int> mod = rpg::Modifier<int>(sourceId, sourceType, 0);

        it->second.removeModifier(mod, target);
    }


    // removeVital(..) methods.
    void Vitals::removeVital(const std::string& name)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(name);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

        removeVital(*vital);
    }
    void Vitals::removeVital(uint32_t id)
    {
        const Vital* vital = helper_methods::getVitalFromRegistry(id);
        
        if (!vital)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

        removeVital(*vital);
    }
    void Vitals::removeVital(const Vital& vital)
    {
        auto it = vitals.find(vital.getID());
        if (it == vitals.end())
            return;

        vitals.erase(it);
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
                   << VitalDataTargetToString(VitalDataTarget::CURRENT_MIN);
            }

            // Serialize all max modifiers for this vital
            for (const auto& mod : data.maxModifiers)
            {
                ss << ";" << mod.sourceId << "," 
                   << rpg::modifierSourceTypeToString(mod.source) << "," 
                   << mod.value << "," 
                   << VitalDataTargetToString(VitalDataTarget::CURRENT_MAX);
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

            vitals.addData(id, current, min, max);

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
                VitalDataTarget target = stringToVitalDataTarget(parts[3]);

                if (target != VitalDataTarget::CURRENT_MIN && target != VitalDataTarget::CURRENT_MAX)
                    throw std::invalid_argument("Invalid modifier target");

                // The vitals are already calculated so this block bypasses the recaclulate() 
                // call in addVitalModifier().
                auto& vitalData = vitals.vitals.at(id);
                rpg::Modifier<int> mod(sourceId, sourceType, value);
                if (target == VitalDataTarget::CURRENT_MAX)
                    vitalData.maxModifiers.push_back(mod);
                else if (target == VitalDataTarget::CURRENT_MIN)
                    vitalData.minModifiers.push_back(mod);
                else
                {
                    std::string err = "Invalid VitalDataTarget when deserializing modifiers.";
                    MIA_THROW(error::ErrorCode::Invalid_RPG_Data, err);
                }
            }
        }

        return vitals;
    }

} // namespace stats
