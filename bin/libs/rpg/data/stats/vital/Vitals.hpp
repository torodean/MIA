/**
 * @file Vitals.hpp
 * @author Antonius Torode
 * @date 07/07/2025
 * Description: A class representing configurable vitals for a character.
 */
#pragma once

#include <unordered_map>
#include <string>

#include <Modifier.hpp>
#include "Vital.hpp"
#include "VitalData.hpp"
#include "BaseDataObjectStorage.hpp"

namespace stats
{
    class Vitals : public data::BaseDataObjectStorage<Vital, VitalData>
    {
    public:    
        /**
         * Default constructor.
         */
        Vitals() = default;

        /**
         * Gets the Vital associated with the given identifier. If the data object is not found
         * in the objects map, a default constructed data object will be returned.
         * Overloads allow querying by vital name, vital ID, or Vital object (by reference).
         *
         * @param name The name of the vital (e.g., "health", "mana").
         *        id The ID of the vital.
         *        vital The Vital object (returns the matching stored Vital or default if not found).
         * @return The Vital associated with the identifier, or a default Vital if not found.
         */
        VitalData& get(const std::string& name) override;
        VitalData& get(uint32_t id) override;
        VitalData& get(const Vital& vital) override;
        
        /**
         * Adds a new vital with specified values.
         *
         * @param name The name of the vital.
         *        id The ID of the vital.
         *        vital The Vital object.
         * @param current The initial current value.
         * @param min The minimum value.
         * @param max The base maximum value.
         */
        void add(const std::string& name, int current, int min, int max);
        void add(uint32_t id, int current, int min, int max);
        void add(const Vital& vital, int current, int min, int max);

        /**
         * Updates a value of a Vital Data object. This uses the target to determine which 
         * value to modify.
         *
         * @param name The name of the vital.
         *        id The ID of the vital.
         *        vital The Vital object.
         * @param target The target value to modify.
         * @param value The new current value.
         */
        void update(const std::string& name, VitalDataTarget target, int value);
        void update(uint32_t id, VitalDataTarget target, int value);
        void update(const Vital& vital, VitalDataTarget target, int value);
        
        /**
         * Adds a modifier to a vital's max or min value. This uses the target to determine 
         * which value to modify.
         *
         * @param name The name of the vital.
         *        id The ID of the vital.
         *        vital The Vital object.
         * @param sourceID ID of the source (e.g., attribute or item ID).
         * @param sourceTypeType of source (e.g., ATTRIBUTE).
         * @param valueThe modifier value.
         * @param target The target modifier type to modify.
         */
        void addModifier(const std::string& name, 
                         uint32_t sourceID, 
                         rpg::ModifierSourceType sourceType, 
                         int32_t value,
                         VitalDataTarget target = VitalDataTarget::CURRENT_MAX);
        void addModifier(uint32_t id, 
                         uint32_t sourceID, 
                         rpg::ModifierSourceType sourceType, 
                         int32_t value,
                         VitalDataTarget target = VitalDataTarget::CURRENT_MAX);
        void addModifier(const Vital& vital, 
                         uint32_t sourceID, 
                         rpg::ModifierSourceType sourceType, 
                         int32_t value,
                         VitalDataTarget target = VitalDataTarget::CURRENT_MAX);
                         
        /**
         * Adds a modifier to a vital's max or min value. This uses the target to determine 
         * which value to modify.
         *
         * @param name The name of the vital.
         *        id The ID of the vital.
         *        vital The Vital object.
         * @param mod The modifier to apply.
         * @param target The target modifier type to modify.
         */
        void addModifier(const std::string& name, 
                         rpg::Modifier<int>& mod,
                         VitalDataTarget target = VitalDataTarget::CURRENT_MAX);
        void addModifier(uint32_t id, 
                         rpg::Modifier<int>& mod,
                         VitalDataTarget target = VitalDataTarget::CURRENT_MAX);
        void addModifier(const Vital& vital, 
                         rpg::Modifier<int>& mod,
                         VitalDataTarget target = VitalDataTarget::CURRENT_MAX);

        /**
         * Removes a min or max modifier by source ID and type. This uses the target to 
         * determine which value to modify.
         *
         * @param name The name of the vital.
         *        id The ID of the vital.
         *        vital The Vital object.
         * @param sourceID ID of the source.
         * @param sourceType[ModifierSourceType] Type of source.
         * @param target The target modifier type to modify.
         */
        void removeModifier(const std::string& name, 
                            uint32_t sourceID, 
                            rpg::ModifierSourceType sourceType,
                            VitalDataTarget target);
        void removeModifier(uint32_t id, 
                            uint32_t sourceID, 
                            rpg::ModifierSourceType sourceType,
                            VitalDataTarget target);
        void removeModifier(const Vital& vital, 
                            uint32_t sourceID, 
                            rpg::ModifierSourceType sourceType,
                            VitalDataTarget target);

        /**
         * Removes a vital by identifier or Vital object.
         *
         * @param name The name of the vital.
         *        id The ID of the vital.
         *        vital The Vital object.
         */
        void remove(const std::string& name);
        void remove(uint32_t id);
        void remove(const Vital& vital);        
        
        /**
         * Checks if the current value has at least the specified quantity.
         * Overloads allow checking by Vital object, Vital ID, or Vital name.
         *
         * @param name The name (std::string)
         *        id The ID (uint32_t)
         *        vital The vital (Vital object)
         * @param value The required amount.
         * @return True if the current vital has enough; false otherwise.
         */
        bool has(const std::string& name, int value) const;
        bool has(uint32_t id, int value) const;
        bool has(const Vital& vital, int value) const;

        /**
         * Dumps the container contents to a stream, primarily for debugging.
         * @param os The output stream to write to (defaults to std::cout).
         */
        void dump(std::ostream& os) const override;

        /**
         * Serializes the Vitals to a compact string enclosed by unique markers
         * for reliable extraction within a larger data stream.
         *
         * Format: [VITALS_BEGIN]id:current,max,min;mod1sourceID,mod1SourceType,mod1Value;...[VITALS_END]
         *
         * @return A string representing the serialized state of the Vitals.
         */
        std::string serialize() const override;

        /**
         * Deserializes a Vitals instance from a string containing serialized data.
         * Searches for the block enclosed by [VITALS_BEGIN] and [VITALS_END] markers,
         * then parses and reconstructs the vitals.
         *
         * @param data A string containing the serialized Vitals.
         * @return A reconstructed Vitals instance.
         */
        static Vitals deserialize(const std::string& data);
        
    }; // class Vitals
} // namespace stats
