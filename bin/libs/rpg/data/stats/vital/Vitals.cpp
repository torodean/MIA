/**
 * @file Vitals.cpp
 * @author Antonius Torode
 * @date 07/07/2025
 * @brief A class representing the vitals for a character.
 */

#include <cmath>
#include <sstream>
#include <variant>
#include "Vitals.hpp"

#include "VitalRegistry.hpp"
#include "RegistryHelper.hpp"
#include "MIAException.hpp"
#include "Error.hpp"

namespace stats
{
    namespace 
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

    } // namespace

    // get(..) methods.
    VitalData& Vitals::get(const std::string& name)
    {
        const Vital* vital = getVitalFromRegistry(name);            
        return get(*vital);
    }
    VitalData& Vitals::get(uint32_t id)
    {
        const Vital* vital = getVitalFromRegistry(id);            
        return get(*vital);
    }
    VitalData& Vitals::get(const Vital& vital)
    {
        auto it = dataStore.find(vital.getID());
        if (it == dataStore.end())
        {
            // The data is not found so add a default one, then return it.
            // TODO - setting the current here to baseMax... This may not always be best/desired.
            add(vital, vital.getBaseMax(), vital.getBaseMin(), vital.getBaseMax());
            it = dataStore.find(vital.getID());
            return it->second;
        }
        else 
            return it->second;
    }
    
    
    // add(..) methods.
    void Vitals::add(const std::string& name, int32_t current, int32_t baseMin, int32_t baseMax)
    {
        const Vital* vital = getVitalFromRegistry(name);
        add(*vital, current, baseMin, baseMax);
    }
    void Vitals::add(uint32_t id, int32_t current, int32_t baseMin, int32_t baseMax)
    {
        const Vital* vital = getVitalFromRegistry(id);
        add(*vital, current, baseMin, baseMax);
    }
    void Vitals::add(const Vital& vital, int32_t current, int32_t baseMin, int32_t baseMax)
    {
        auto id = vital.getID();
        if (dataStore.find(id) != dataStore.end())
            MIA_THROW(error::ErrorCode::Duplicate_RPG_Value);

        dataStore.emplace(id, VitalData(current, baseMin, baseMax));
    }


    // update(..) methods.
    void Vitals::update(const std::string& name, int32_t value)
    {
        const Vital* vital = getVitalFromRegistry(name);
        update(*vital, value);
    }
    void Vitals::update(uint32_t id, int32_t value)
    {
        const Vital* vital = getVitalFromRegistry(id);
        update(*vital, value);
    }
    void Vitals::update(const Vital& vital, int32_t value)
    {
        auto it = dataStore.find(vital.getID());
        if (it == dataStore.end())
        {
            // The data is not found so add a default one, then update the current.
            // TODO - setting the current here to baseMax... This may not always be best/desired.
            add(vital, vital.getBaseMax(), vital.getBaseMin(), vital.getBaseMax());
            // The insert may have rehashed the map, so the iterator must be refreshed.
            it = dataStore.find(vital.getID());
        }

        it->second.setCurrent(value);
    }
    
    
    // addModifier(..) methods.
    void Vitals::addModifier(const std::string& name,
                             uint32_t sourceID,
                             rpg::ModifierSourceType sourceType,
                             int32_t value,
                             VitalDataTarget target,
                             rpg::ModifyType modifyType)
    {
        const Vital* vital = getVitalFromRegistry(name);
        addModifier(*vital, sourceID, sourceType, value, target, modifyType);
    }
    void Vitals::addModifier(uint32_t id,
                             uint32_t sourceID,
                             rpg::ModifierSourceType sourceType,
                             int32_t value,
                             VitalDataTarget target,
                             rpg::ModifyType modifyType)
    {
        const Vital* vital = getVitalFromRegistry(id);
        addModifier(*vital, sourceID, sourceType, value, target, modifyType);
    }
    void Vitals::addModifier(const Vital& vital,
                             uint32_t sourceID,
                             rpg::ModifierSourceType sourceType,
                             int32_t value,
                             VitalDataTarget target,
                             rpg::ModifyType modifyType)
    {
        auto it = dataStore.find(vital.getID());
        if (it == dataStore.end())
        {
            // The data is not found so add a default one, then attach the modifier.
            // TODO - setting the current here to baseMax... This may not always be best/desired.
            add(vital, vital.getBaseMax(), vital.getBaseMin(), vital.getBaseMax());
            // The insert may have rehashed the map, so the iterator must be refreshed.
            it = dataStore.find(vital.getID());
        }

        rpg::Modifier mod = rpg::Modifier(sourceID, sourceType, value, modifyType);

        it->second.addModifier(mod, target);
    }
    void Vitals::addModifier(const std::string& name,
                             const rpg::Modifier& mod,
                             VitalDataTarget target)
    {
        const Vital* vital = getVitalFromRegistry(name);
        addModifier(*vital, mod, target);
    }
    void Vitals::addModifier(uint32_t id,
                             const rpg::Modifier& mod,
                             VitalDataTarget target)
    {
        const Vital* vital = getVitalFromRegistry(id);
        addModifier(*vital, mod, target);
    }
    void Vitals::addModifier(const Vital& vital,
                             const rpg::Modifier& mod,
                             VitalDataTarget target)
    {
        auto it = dataStore.find(vital.getID());
        if (it == dataStore.end())
        {
            // The data is not found so add a default one, then attach the modifier.
            // TODO - setting the current here to baseMax... This may not always be best/desired.
            add(vital, vital.getBaseMax(), vital.getBaseMin(), vital.getBaseMax());
            // The insert may have rehashed the map, so the iterator must be refreshed.
            it = dataStore.find(vital.getID());
        }

        it->second.addModifier(mod, target);
    }


    // removeModifier(..) methods.
    void Vitals::removeModifier(const std::string& name, 
                                uint32_t sourceID, 
                                rpg::ModifierSourceType sourceType,
                                VitalDataTarget target)
    {
        const Vital* vital = getVitalFromRegistry(name);
        removeModifier(*vital, sourceID, sourceType, target);
    }
    void Vitals::removeModifier(uint32_t id, 
                                uint32_t sourceID, 
                                rpg::ModifierSourceType sourceType,
                                VitalDataTarget target)
    {
        const Vital* vital = getVitalFromRegistry(id);
        removeModifier(*vital, sourceID, sourceType, target);
    }
    void Vitals::removeModifier(const Vital& vital, 
                                uint32_t sourceID, 
                                rpg::ModifierSourceType sourceType,
                                VitalDataTarget target)
    {
        auto it = dataStore.find(vital.getID());
        if (it == dataStore.end())
        {
            // The data is not found so no need to remove anything.
            return;
        }
        
        // removeModifier() matches on source ID and type only, so the value here is a placeholder.
        rpg::Modifier mod = rpg::Modifier(sourceID, sourceType, 0);

        it->second.removeModifier(mod, target);
    }


    // remove(..) methods.
    void Vitals::remove(const std::string& name)
    {
        const Vital* vital = getVitalFromRegistry(name);
        remove(*vital);
    }
    void Vitals::remove(uint32_t id)
    {
        const Vital* vital = getVitalFromRegistry(id);
        remove(*vital);
    }
    void Vitals::remove(const Vital& vital)
    {
        auto it = dataStore.find(vital.getID());
        if (it == dataStore.end())
            return;

        dataStore.erase(it);
    }
    
    bool Vitals::has(const std::string& name, int32_t value) const    
    {
        const Vital* vital = getVitalFromRegistry(name);
        return has(*vital, value);
    }
    bool Vitals::has(uint32_t id, int32_t value) const
    {
        const Vital* vital = getVitalFromRegistry(id);
        return has(*vital, value);
    }
    bool Vitals::has(const Vital& vital, int32_t value) const
    {
        auto it = dataStore.find(vital.getID());
        if (it == dataStore.end())
            return false;
            
        if (it->second.getCurrent() >= value)
            return true;
            
        return false;
    }

    void Vitals::dump(std::ostream& os) const
    {
        for (const auto& [id, vitalData] : dataStore) 
        {
            auto vital = VitalRegistry::getInstance().getByID(id);
            os << "Vital: " << vital->getName()
               << ", Min: " << vitalData.getCurrentMin()
               << ", Max: " << vitalData.getCurrentMax()
               << ", Current: " << vitalData.getCurrent()
               << "\n";
        }
    }
    
    
    std::string Vitals::serialize() const
    {
        std::stringstream ss;
        ss << "[VITALS_BEGIN]"; // Start of serialized content
        bool first = true;

        // Loop through all stored dataStore
        for (const auto& [id, data] : dataStore)
        {
            if (!first)
                ss << "|"; // Separate entries with '|'
            first = false;

            // Write the base vital data: <id>:<current>,<baseMin>,<baseMax>
            ss << id << ":" << data.getCurrent() << "," << data.getBaseMin() << ","
               << data.getBaseMax();

            // Serialize the min and max modifiers with their modify types.
            for (const auto target : {VitalDataTarget::CURRENT_MIN, VitalDataTarget::CURRENT_MAX})
            {
                for (const auto& mod : data.getModifiers(target))
                {
                    std::ostringstream valueStream;
                    if (std::holds_alternative<int>(mod.value))
                        valueStream << std::get<int>(mod.value);
                    else
                        valueStream << std::get<double>(mod.value);

                    ss << ";" << mod.sourceID << ","
                       << rpg::modifierSourceTypeToString(mod.source) << ","
                       << valueStream.str() << ","
                       << VitalDataTargetToString(target) << ","
                       << rpg::modifyTypeToString(mod.modifyType);
                }
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
            MIA_THROW(error::ErrorCode::Invalid_RPG_Data,
                  "Invalid serialized data: missing or malformed [VITALS_BEGIN]/[VITALS_END]");

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
                MIA_THROW(error::ErrorCode::Invalid_RPG_Data,
                          "Malformed vital entry: missing base info");

            size_t colon = baseInfo.find(':');
            if (colon == std::string::npos)
                MIA_THROW(error::ErrorCode::Invalid_RPG_Data,
                          "Malformed base info: missing ':' separator");

            uint32_t id;
            try
            {
                id = std::stoul(baseInfo.substr(0, colon));
            }
            catch (...)
            {
                MIA_THROW(error::ErrorCode::Invalid_RPG_Data, "Invalid vital id");
            }

            std::stringstream baseStream(baseInfo.substr(colon + 1));
            int32_t current, min, max;
            char comma1, comma2;
            if (!(baseStream >> current >> comma1 >> min >> comma2 >> max) || comma1 != ',' || comma2 != ',')
                MIA_THROW(error::ErrorCode::Invalid_RPG_Data, "Invalid vital values format");

            const Vital* vital = getVitalFromRegistry(id);
        
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

                if (parts.size() != 5)
                    MIA_THROW(error::ErrorCode::Invalid_RPG_Data, "Invalid modifier format");

                uint32_t sourceID = std::stoul(parts[0]);
                rpg::ModifierSourceType sourceType = rpg::stringToModifierSourceType(parts[1]);
                double parsedValue = std::stod(parts[2]);
                VitalDataTarget target = stringToVitalDataTarget(parts[3]);
                rpg::ModifyType modifyType = rpg::stringToModifyType(parts[4]);

                if (target != VitalDataTarget::CURRENT_MIN && target != VitalDataTarget::CURRENT_MAX)
                    MIA_THROW(error::ErrorCode::Invalid_RPG_Data, "Invalid modifier target");

                // Store the value as an int for ADD_MAX and SET, and as a double for MULTIPLY.
                rpg::Modifier mod(sourceID, sourceType,
                                  (modifyType == rpg::ModifyType::MULTIPLY)
                                      ? rpg::Modifier::Value(parsedValue)
                                      : rpg::Modifier::Value(static_cast<int>(std::round(parsedValue))),
                                  modifyType);
                vitals.dataStore.at(id).addModifier(mod, target);
            }
        }

        return vitals;
    }

} // namespace stats
