/**
 * @file Attributes.cpp
 * @author Antonius Torode
 * @date 07/11/2025
 * @brief Storage for a container of attributes.
 */

// Include the associated header file.
#include "Attributes.hpp"

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <variant>

#include "AttributeRegistry.hpp"
#include "RegistryHelper.hpp"
#include "MIAException.hpp"

namespace stats
{
    namespace helper_methods 
    {
        /**
         * Resolves and retrieves a Attribute object from the AttributeRegistry based on a given identifier.
         *
         * This is a wrapper around the generic rpg::helper_methods::getFromRegistry, specialized
         * for the AttributeRegistry and Attribute types used within the stats namespace. It accepts an identifier
         * in the form of a name (std::string), ID (uint32_t), or a Attribute object, and attempts to fetch
         * the corresponding Attribute instance from the registry.
         *
         * @tparam T The type of identifier: std::string, uint32_t, or Attribute.
         * @param identifier The identifier used to locate the Attribute in the registry.
         * @return Pointer to the corresponding Attribute object.
         * @throws MIA_THROW with Undefined_RPG_Value if the object is not found.
         */
        template<typename T>
        const Attribute* getAttributeFromRegistry(const T& identifier)
        {
            return rpg::helper_methods::getFromRegistry<AttributeRegistry, Attribute, T>(identifier);
        }

    } // namespace helper_methods
    
    // get() methods...
    AttributeData& Attributes::get(const std::string& name)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        return get(*attribute);
    }
    AttributeData& Attributes::get(uint32_t id)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        return get(*attribute);
    }
    AttributeData& Attributes::get(const Attribute& attribute)
    {
        auto it = dataStore.find(attribute.getID());
        if (it == dataStore.end())
        {
            // The data is not found so add a default one, then return it.
            add(attribute, attribute.getBaseValue());
            it = dataStore.find(attribute.getID());
            return it->second;
        }
        else
            return it->second;
    }
    
    
    // add() methods...
    void Attributes::add(const std::string& name, int baseValue)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        add(*attribute, baseValue);
    }
    void Attributes::add(uint32_t id, int baseValue)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        add(*attribute, baseValue);
    }
    void Attributes::add(const Attribute& attribute, int baseValue)
    {
        auto id = attribute.getID();
        if (dataStore.find(id) != dataStore.end())            
            MIA_THROW(error::ErrorCode::Duplicate_RPG_Value);
            
        dataStore.emplace(id, AttributeData(baseValue));
    }
    
    
    // update() methods...
    void Attributes::update(const std::string& name, int value)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        update(*attribute, value);
    }
    void Attributes::update(uint32_t id, int value)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        update(*attribute, value);
    }
    void Attributes::update(const Attribute& attribute, int value)
    {
        auto it = dataStore.find(attribute.getID());
        if (it == dataStore.end())
        {
            // The data is not found so add it.
            add(attribute, value);
        }
        else
        {
            it->second.setBaseValue(value);
        }
    }
    
    
    // addModifier() methods...
    void Attributes::addModifier(const std::string& name,
                                 uint32_t sourceID,
                                 rpg::ModifierSourceType sourceType,
                                 int32_t value,
                                 rpg::ModifyType modifyType)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        addModifier(*attribute, sourceID, sourceType, value, modifyType);
    }
    void Attributes::addModifier(uint32_t id,
                                 uint32_t sourceID,
                                 rpg::ModifierSourceType sourceType,
                                 int32_t value,
                                 rpg::ModifyType modifyType)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        addModifier(*attribute, sourceID, sourceType, value, modifyType);
    }
    void Attributes::addModifier(const Attribute& attribute,
                                 uint32_t sourceID,
                                 rpg::ModifierSourceType sourceType,
                                 int32_t value,
                                 rpg::ModifyType modifyType)
    {
        auto it = dataStore.find(attribute.getID());
        if (it == dataStore.end())
        {
            // The data is not found so add a default one, then attach the modifier.
            add(attribute, attribute.getBaseValue());
            // The insert may have rehashed the map, so the iterator must be refreshed.
            it = dataStore.find(attribute.getID());
        }

        rpg::Modifier mod = rpg::Modifier(sourceID, sourceType, value, modifyType);

        it->second.addModifier(mod);
    }
    void Attributes::addModifier(const std::string& name,
                                 const rpg::Modifier& mod)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        addModifier(*attribute, mod);
    }
    void Attributes::addModifier(uint32_t id,
                                 const rpg::Modifier& mod)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        addModifier(*attribute, mod);
    }
    void Attributes::addModifier(const Attribute& attribute,
                                 const rpg::Modifier& mod)
    {
        auto it = dataStore.find(attribute.getID());
        if (it == dataStore.end())
        {
            // The data is not found so add a default one, then attach the modifier.
            add(attribute, attribute.getBaseValue());
            // The insert may have rehashed the map, so the iterator must be refreshed.
            it = dataStore.find(attribute.getID());
        }

        it->second.addModifier(mod);
    }

    
    // removeModifier() methods...
    void Attributes::removeModifier(const std::string& name, 
                                    uint32_t sourceID, 
                                    rpg::ModifierSourceType sourceType)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        removeModifier(*attribute, sourceID, sourceType);
    }
    void Attributes::removeModifier(uint32_t id, 
                                    uint32_t sourceID, 
                                    rpg::ModifierSourceType sourceType)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        removeModifier(*attribute, sourceID, sourceType);
    }
    void Attributes::removeModifier(const Attribute& attribute, 
                                    uint32_t sourceID, 
                                    rpg::ModifierSourceType sourceType)
    {
        auto it = dataStore.find(attribute.getID());
        if (it == dataStore.end())
        {
            // The data is not found so no need to remove anything.
            return;
        }
        
        // removeModifier() matches on source ID and type only, so the value here is a placeholder.
        rpg::Modifier mod = rpg::Modifier(sourceID, sourceType, 0);

        it->second.removeModifier(mod);
    }

    
    // remove() methods...
    void Attributes::remove(const std::string& name)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        remove(*attribute);
    }
    void Attributes::remove(uint32_t id)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        remove(*attribute);
    }
    void Attributes::remove(const Attribute& attribute)
    {
        auto it = dataStore.find(attribute.getID());
        if (it == dataStore.end())
            return;

        dataStore.erase(it);
    }


    void Attributes::dump(std::ostream& os) const
    {
        for (const auto& [id, attributeData] : dataStore) 
        {
            auto attribute = AttributeRegistry::getInstance().getByID(id);
            os << "Attribute: " << attribute->getName()
               << ", getCurrent: " << attributeData.getCurrent()
               << "\n";
        }
    }


    std::string Attributes::serialize() const
    {
        std::stringstream ss;
        ss << "[ATTRIBUTES_BEGIN]";

        bool firstAttribute = true;
        for (const auto& [id, attrData] : dataStore)
        {
            if (!firstAttribute)
                ss << ";";
            firstAttribute = false;

            // Serialize id and base value
            ss << id << ":" << attrData.getBaseValue();

            // Serialize modifiers; each is one self-contained ','-delimited unit.
            const auto& modifiers = attrData.getModifiers();
            for (const auto& mod : modifiers)
                ss << "," << mod.serialize();
        }

        ss << "[ATTRIBUTES_END]";
        return ss.str();
    }
    

    Attributes Attributes::deserialize(const std::string& data)
    {
        Attributes result;

        // Find the markers
        const std::string beginMarker = "[ATTRIBUTES_BEGIN]";
        const std::string endMarker = "[ATTRIBUTES_END]";
        auto startPos = data.find(beginMarker);
        auto endPos = data.find(endMarker);

        if (startPos == std::string::npos || endPos == std::string::npos || endPos < startPos)
        {
            // Invalid or missing markers; the save data is corrupt, not merely empty.
            MIA_THROW(error::ErrorCode::Invalid_RPG_Data,
                      "Attributes block not found.");
        }

        // Extract the content between markers
        startPos += beginMarker.length();
        std::string content = data.substr(startPos, endPos - startPos);

        if (content.empty())
        {
            return result;
        }

        // Split content by semicolons to get attribute entries
        std::stringstream contentStream(content);
        std::string attributeEntry;
        while (std::getline(contentStream, attributeEntry, ';'))
        {
            std::stringstream entryStream(attributeEntry);
            std::string segment;

            // Get id and current value
            std::getline(entryStream, segment, ':');
            uint32_t id;
            try
            {
                id = std::stoul(segment);
            }
            catch (const std::exception&)
            {
                // Skip invalid id
                continue;
            }

            std::getline(entryStream, segment, ',');
            int current;
            try
            {
                current = std::stoi(segment);
            }
            catch (const std::exception&)
            {
                // Skip invalid current value
                continue;
            }

            // Each remaining ','-delimited unit is one serialized Modifier.
            std::vector<rpg::Modifier> modifiers;
            std::string modifierEntry;
            while (std::getline(entryStream, modifierEntry, ','))
                modifiers.push_back(rpg::Modifier::deserialize(modifierEntry));

            // Create and add AttributeData to the map.
            result.dataStore.emplace(id, AttributeData(current, modifiers));
        }

        return result;
    }
} // namespace stats
