/**
 * @file Attributes.hpp
 * @author Antonius Torode
 * @date 07/11/2025
 * Description: Storage for a container of attributes.
 */
#pragma once

#include <string>
#include "Attribute.hpp"
#include "AttributeData.hpp"

namespace stats
{
    class Attributes
    {
    public:
    
        /**
         * Default constructor.
         */
        Attributes() = default;

        /**
         * Gets the Attribute associated with the given identifier. If the data object is not found
         * in the objects map, a default constructed data object will be returned.
         * Overloads allow querying by Attribute name, Attribute ID, or Attribute object (by reference).
         *
         * @param name[const std::string&] - The name of the Attribute (e.g., "health", "mana").
         *        id[uint32_t] - The ID of the Attribute.
         *        Attribute[const Attribute&] - The Attribute object (returns the matching stored Attribute or default if not found).
         * @return The Attribute associated with the identifier, or a default Attribute if not found.
         */
        const AttributeData& get(const std::string& name);
        const AttributeData& get(uint32_t id);
        const AttributeData& get(const Attribute& attribute);
        
        /**
         * Adds a new Attribute with specified values.
         *
         * @param name[const std::string&] The name of the Attribute.
         *        id[uint32_t] The ID of the Attribute.
         *        Attribute[const Attribute&] The Attribute object.
         * @param current[int] The initial current value.
         * @param min[int] The minimum value.
         * @param max[int] The base maximum value.
         */
        void add(const std::string& name, int current);
        void add(uint32_t id, int current);
        void add(const Attribute& attribute, int current);
        
        /**
         * Updates the current value of a Attribute Data object. 
         *
         * @param name[const std::string&] The name of the Attribute.
         *        id[uint32_t] The ID of the Attribute.
         *        Attribute[const Attribute&] The Attribute object.
         * @param value[int] The new current value.
         */
        void update(const std::string& name, int value);
        void update(uint32_t id, int value);
        void update(const Attribute& attribute, int value);
        
        /**
         * Adds a modifier to a Attribute's current value.
         *
         * @param name[const std::string&] The name of the Attribute.
         *        id[uint32_t] The ID of the Attribute.
         *        Attribute[const Attribute&] The Attribute object.
         * @param sourceID[uint32_t] ID of the source (e.g., attribute or item ID).
         * @param sourceType[ModifierSourceType] Type of source (e.g., ATTRIBUTE).
         * @param value[int32_t] The modifier value.
         */
        void addModifier(const std::string& name, 
                         uint32_t sourceID, 
                         rpg::ModifierSourceType sourceType, 
                         int32_t value);
        void addModifier(uint32_t id, 
                         uint32_t sourceID, 
                         rpg::ModifierSourceType sourceType, 
                         int32_t value);
        void addModifier(const Attribute& attribute, 
                         uint32_t sourceID, 
                         rpg::ModifierSourceType sourceType, 
                         int32_t value);

        /**
         * Removes a min or max modifier by source ID and type. 
         *
         * @param name[const std::string&] The name of the Attribute.
         *        id[uint32_t] The ID of the Attribute.
         *        Attribute[const Attribute&] The Attribute object.
         * @param sourceID[uint32_t] ID of the source.
         * @param sourceType[ModifierSourceType] Type of source.
         */
        void removeModifier(const std::string& name, 
                            uint32_t sourceID, 
                            rpg::ModifierSourceType sourceType);
        void removeModifier(uint32_t id, 
                            uint32_t sourceID, 
                            rpg::ModifierSourceType sourceType);
        void removeModifier(const Attribute& attribute, 
                            uint32_t sourceID, 
                            rpg::ModifierSourceType sourceType);

        /**
         * Removes a Attribute by identifier or Attribute object.
         *
         * @param name[const std::string&] The name of the Attribute.
         *        id[uint32_t] The ID of the Attribute.
         *        Attribute[const Attribute&] The Attribute object.
         * @return True if the Attribute was removed; false if it does not exist.
         */
        void remove(const std::string& name);
        void remove(uint32_t id);
        void remove(const Attribute& attribute);
    
        /**
         * Serializes the Attributes to a compact string enclosed by unique markers
         * for reliable extraction within a larger data stream.
         *
         * Format: [ATTRIBUTES_BEGIN]id:current,mod1SourceId,mod1SourceType,mod1Value;...[ATTRIBUTES_END]
         *
         * @return A string representing the serialized state of the Attributes.
         */
        std::string serialize() const;

        /**
         * Deserializes a Attributes instance from a string containing serialized data.
         * Searches for the block enclosed by [ATTRIBUTES_BEGIN] and [ATTRIBUTES_END] markers,
         * then parses and reconstructs the Attributes.
         *
         * @param data[const std::string&] - A string containing the serialized Attributes.
         * @return A reconstructed Attributes instance.
         */
        static Attributes deserialize(const std::string& data);
        
    private:
    
        /// Map of attribute ID to their attribute data instance.
        std::unordered_map<uint32_t, AttributeData> attributes;
        
    }; // class Attributes
} // namespace stats

