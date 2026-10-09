/**
 * @file Attributes.hpp
 * @author Antonius Torode
 * @date 07/11/2025
 * @brief Storage for a container of attributes.
 */
#pragma once

#include <string>

#include "Attribute.hpp"
#include "AttributeData.hpp"
#include "BaseDataObjectStorage.hpp"

namespace stats
{
    /**
     * A container for the runtime state of a character's attributes.
     *
     * Each entry pairs a registered Attribute with its AttributeData, which stores the
     * attribute's base value along with the modifiers attached to it. The effective
     * current value always derives from the stored base, so lookups by name, ID, or
     * Attribute object return the live data rather than a copy. Attribute definitions
     * live in the AttributeRegistry; this class only holds per-instance values.
     */
    class Attributes : public data::BaseDataObjectStorage<Attribute, AttributeData>
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
         * @param name The name of the Attribute (e.g., "health", "mana").
         *        id The ID of the Attribute.
         *        Attribute The Attribute object (returns the matching stored Attribute or default if not found).
         * @return The Attribute associated with the identifier, or a default Attribute if not found.
         */
        AttributeData& get(const std::string& name) override;
        AttributeData& get(uint32_t id) override;
        AttributeData& get(const Attribute& attribute) override;

        /**
         * Adds a new Attribute with specified values.
         *
         * @param name The name of the Attribute.
         *        id The ID of the Attribute.
         *        Attribute The Attribute object.
         * @param baseValue The initial base value.
         */
        void add(const std::string& name, int baseValue);
        void add(uint32_t id, int baseValue);
        void add(const Attribute& attribute, int baseValue);

        /**
         * Updates the base value of an Attribute Data object. The effective current
         * value recomputes from the base and the attached modifiers.
         *
         * @param name The name of the Attribute.
         *        id The ID of the Attribute.
         *        Attribute The Attribute object.
         * @param value The new base value.
         */
        void update(const std::string& name, int value);
        void update(uint32_t id, int value);
        void update(const Attribute& attribute, int value);

        /**
         * Adds a modifier to an Attribute's current value. The modifier value is an int,
         * which suits the ADD_MAX and SET types; MULTIPLY modifiers (a double multiplier
         * bonus) are attached through the Modifier overloads.
         *
         * @param name The name of the Attribute.
         *        id The ID of the Attribute.
         *        Attribute The Attribute object.
         * @param sourceID ID of the source (e.g., attribute or item ID).
         * @param sourceType Type of source (e.g., ATTRIBUTE).
         * @param value The modifier value.
         * @param modifyType The modification type (defaults to ADD_MAX).
         */
        void addModifier(const std::string& name,
                         uint32_t sourceID,
                         rpg::ModifierSourceType sourceType,
                         int32_t value,
                         rpg::ModifyType modifyType = rpg::ModifyType::ADD_MAX);
        void addModifier(uint32_t id,
                         uint32_t sourceID,
                         rpg::ModifierSourceType sourceType,
                         int32_t value,
                         rpg::ModifyType modifyType = rpg::ModifyType::ADD_MAX);
        void addModifier(const Attribute& attribute,
                         uint32_t sourceID,
                         rpg::ModifierSourceType sourceType,
                         int32_t value,
                         rpg::ModifyType modifyType = rpg::ModifyType::ADD_MAX);

        /**
         * Adds a modifier to an Attribute's current value.
         *
         * @param name The name of the Attribute.
         *        id The ID of the Attribute.
         *        Attribute The Attribute object.
         * @param mod The modifier to apply.
         */
        void addModifier(const std::string& name,
                         const rpg::Modifier& mod);
        void addModifier(uint32_t id,
                         const rpg::Modifier& mod);
        void addModifier(const Attribute& attribute,
                         const rpg::Modifier& mod);

        /**
         * Removes a min or max modifier by source ID and type.
         *
         * @param name The name of the Attribute.
         *        id The ID of the Attribute.
         *        Attribute The Attribute object.
         * @param sourceID ID of the source.
         * @param sourceType Type of source.
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
         * Removes an Attribute by identifier or Attribute object.
         *
         * @param name The name of the Attribute.
         *        id The ID of the Attribute.
         *        Attribute The Attribute object.
         */
        void remove(const std::string& name);
        void remove(uint32_t id);
        void remove(const Attribute& attribute);

        /**
         * Dumps the container contents to a stream, primarily for debugging.
         *
         * @param os The output stream to write to (defaults to std::cout).
         */
        void dump(std::ostream& os) const override;

        /**
         * Serializes the Attributes to a compact string enclosed by unique markers
         * for reliable extraction within a larger data stream. The serialized value is
         * the base value, so the effective value recomputes from the modifiers on
         * deserialization.
         *
         * Format: [ATTRIBUTES_BEGIN]id:base,sourceID:SOURCE,value:TYPE:POLICY;...[ATTRIBUTES_END]
         *
         * @return A string representing the serialized state of the Attributes.
         */
        std::string serialize() const override;

        /**
         * Deserializes a Attributes instance from a string containing serialized data.
         * Searches for the block enclosed by [ATTRIBUTES_BEGIN] and [ATTRIBUTES_END] markers,
         * then parses and reconstructs the Attributes.
         *
         * @param data A string containing the serialized Attributes.
         * @return A reconstructed Attributes instance.
         * @throws MIAException if no valid serialized block is found.
         */
        static Attributes deserialize(const std::string& data);

    }; // class Attributes
} // namespace stats
