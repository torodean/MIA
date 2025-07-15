/**
 * @file Attributes.cpp
 * @author Antonius Torode
 * @date 07/11/2025
 * Description: Storage for a container of attributes.
 */
 
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
    void Attributes::addData(const Attribute& Attribute, int current)
    {
    
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
    void Attributes::updateAttribute(const Attribute& Attribute, int value)
    {
    
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
    void Attributes::addAttributeModifier(const Attribute& Attribute, 
                                          uint32_t sourceID, 
                                          rpg::ModifierSourceType sourceType, 
                                          int32_t value)
    {
    
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
    void Attributes::removeAttributeModifier(const Attribute& Attribute, 
                                             uint32_t sourceID, 
                                             rpg::ModifierSourceType sourceType)
    {
    
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
    void Attributes::removeAttribute(const Attribute& Attribute)
    {
    
    }


    std::string Attributes::serialize() const
    {
        // TODO
        return "";
    }


    Attributes Attributes::deserialize(const std::string& data)
    {
        // TODO
        return Attributes();
    }
} // namespace stats

