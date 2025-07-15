/**
 * @file Attributes.cpp
 * @author Antonius Torode
 * @date 07/11/2025
 * Description: Storage for a container of attributes.
 */
 
#include <sstream>
#include <stdexcept>
 
#include "Attributes.hpp"
#include "AttributeRegistry.hpp"
#include "RegistryHelper.hpp"

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
    
    // getData() methods...
    const AttributeData& Attributes::getData(const std::string& name)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        return getData(*attribute);
    }
    const AttributeData& Attributes::getData(uint32_t id)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        return getData(*attribute);
    }
    const AttributeData& Attributes::getData(const Attribute& attribute)
    {
        auto it = attributes.find(attribute.getID());
        if (it == attributes.end())
        {
            // The data is not found so add a default one, then update the current.
            addData(attribute, attribute.getBaseValue());
            it = attributes.find(attribute.getID());
            return it->second;
        }
        else 
            return it->second;
    }
    
    
    // addData() methods...
    void Attributes::addData(const std::string& name, int current)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        addData(*attribute, current);
    }
    void Attributes::addData(uint32_t id, int current)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        addData(*attribute, current);
    }
    void Attributes::addData(const Attribute& attribute, int current)
    {
        auto id = attribute.getID();
        if (attributes.find(id) != attributes.end())            
            MIA_THROW(error::ErrorCode::Duplicate_RPG_Value);
            
        attributes.emplace(id, AttributeData(current));
    }
    
    
    // updateAttribute() methods...
    void Attributes::updateAttribute(const std::string& name, int value)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        updateAttribute(*attribute, value);
    }
    void Attributes::updateAttribute(uint32_t id, int value)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        updateAttribute(*attribute, value);
    }
    void Attributes::updateAttribute(const Attribute& attribute, int value)
    {
        auto it = attributes.find(attribute.getID());
        if (it == attributes.end())
        {
            // The data is not found so add it.
            addData(attribute, value);
        }
        else
        {
            it->second.setCurrent(value);
        }
    }
    
    
    // addAttributeModifier() methods...
    void Attributes::addAttributeModifier(const std::string& name, 
                                          uint32_t sourceID, 
                                          rpg::ModifierSourceType sourceType, 
                                          int32_t value)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        addAttributeModifier(*attribute, sourceID, sourceType, value);
    }
    void Attributes::addAttributeModifier(uint32_t id, 
                                          uint32_t sourceID, 
                                          rpg::ModifierSourceType sourceType, 
                                          int32_t value)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        addAttributeModifier(*attribute, sourceID, sourceType, value);
    }
    void Attributes::addAttributeModifier(const Attribute& attribute, 
                                          uint32_t sourceID, 
                                          rpg::ModifierSourceType sourceType, 
                                          int32_t value)
    {
        auto it = attributes.find(attribute.getID());
        if (it == attributes.end())
        {
            // The data is not found so add a default one, then update the current.
            addData(attribute, attribute.getBaseValue());
        }
        
        rpg::Modifier<int> mod = rpg::Modifier<int>(sourceID, sourceType, value);
        
        it->second.addModifier(mod);
    }

    
    // removeAttributeModifier() methods...
    void Attributes::removeAttributeModifier(const std::string& name, 
                                             uint32_t sourceID, 
                                             rpg::ModifierSourceType sourceType)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        removeAttributeModifier(*attribute, sourceID, sourceType);
    }
    void Attributes::removeAttributeModifier(uint32_t id, 
                                             uint32_t sourceID, 
                                             rpg::ModifierSourceType sourceType)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        removeAttributeModifier(*attribute, sourceID, sourceType);
    }
    void Attributes::removeAttributeModifier(const Attribute& attribute, 
                                             uint32_t sourceID, 
                                             rpg::ModifierSourceType sourceType)
    {
        auto it = attributes.find(attribute.getID());
        if (it == attributes.end())
        {
            // The data is not found so no need to remove anything.
            return;
        }
        
        // removeModifier() doesn't check the value so setting it to zero here...
        rpg::Modifier<int> mod = rpg::Modifier<int>(sourceID, sourceType, 0);
        
        it->second.removeModifier(mod);
    }

    
    // removeAttribute() methods...
    void Attributes::removeAttribute(const std::string& name)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(name);
        removeAttribute(*attribute);
    }
    void Attributes::removeAttribute(uint32_t id)
    {
        const Attribute* attribute = helper_methods::getAttributeFromRegistry(id);
        removeAttribute(*attribute);
    }
    void Attributes::removeAttribute(const Attribute& attribute)
    {
        auto it = attributes.find(attribute.getID());
        if (it == attributes.end())
            return;

        attributes.erase(it);
    }


    std::string Attributes::serialize() const
    {
        std::stringstream ss;
        ss << "[ATTRIBUTES_BEGIN]";

        bool firstAttribute = true;
        for (const auto& [id, attrData] : attributes)
        {
            if (!firstAttribute)
                ss << ";";
            firstAttribute = false;

            // Serialize id and current value
            ss << id << ":" << attrData.getCurrent();

            // Serialize modifiers
            const auto& modifiers = attrData.getModifiers();
            for (const auto& mod : modifiers)
            {
                ss << "," << mod.sourceID << ","
                   << rpg::modifierSourceTypeToString(mod.source) << ","
                   << mod.value;
            }
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
            // Invalid or missing markers, return empty Attributes
            return result;
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

            // Create AttributeData with current value
            std::vector<rpg::Modifier<int>> modifiers;
            while (std::getline(entryStream, segment, ','))
            {
                uint32_t sourceID;
                try
                {
                    sourceID = std::stoul(segment);
                }
                catch (const std::exception&)
                {
                    // Skip invalid sourceID
                    continue;
                }

                std::getline(entryStream, segment, ',');
                rpg::ModifierSourceType sourceType = rpg::stringToModifierSourceType(segment);

                std::getline(entryStream, segment, ',');
                int32_t modValue;
                try
                {
                    modValue = std::stoi(segment);
                }
                catch (const std::exception&)
                {
                    // Skip invalid modifier value
                    continue;
                }

                modifiers.emplace_back(sourceID, sourceType, modValue);
            }

            // Create and add AttributeData to the map
            result.attributes.emplace(id, AttributeData(current, modifiers));
        }

        return result;
    }
} // namespace stats

