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

namespace stats
{
    class Vitals
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
         * @param name[const std::string&] - The name of the vital (e.g., "health", "mana").
         *        id[uint32_t] - The ID of the vital.
         *        vital[const Vital&] - The Vital object (returns the matching stored Vital or default if not found).
         * @return The Vital associated with the identifier, or a default Vital if not found.
         */
        const VitalData& getData(const std::string& name);
        const VitalData& getData(uint32_t id);
        const VitalData& getData(const Vital& vital);
        
        /**
         * Adds a new vital with specified values.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @param current[int] The initial current value.
         * @param min[int] The minimum value.
         * @param max[int] The base maximum value.
         * @return True if the vital was added; false if it exists or is invalid.
         */
        void addData(const std::string& name, int current, int min, int max);
        void addData(uint32_t id, int current, int min, int max);
        void addData(const Vital& vital, int current, int min, int max);

        /**
         * Updates a value of a Vital Data object. This uses the target to determine which 
         * value to modify.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @param target[VitalDataTarget] - The target value to modify.
         * @param value[int] The new current value.
         * @return True if the vital was updated; false if it does not exist.
         */
        void updateVital(const std::string& name, VitalDataTarget target, int value);
        void updateVital(uint32_t id, VitalDataTarget target, int value);
        void updateVital(const Vital& vital, VitalDataTarget target, int value);
        
        /**
         * Adds a modifier to a vital's max or min value. This uses the target to determine 
         * which value to modify.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @param sourceId[uint32_t] ID of the source (e.g., attribute or item ID).
         * @param sourceType[ModifierSourceType] Type of source (e.g., ATTRIBUTE).
         * @param value[int32_t] The modifier value.
         * @param target[pVitalDataTarget] - The target modifier type to modify.
         * @return True if the modifier was added; false if the vital does not exist.
         */
        void addVitalModifier(const std::string& name, uint32_t sourceId, 
                              rpg::ModifierSourceType sourceType, int32_t value,
                              VitalDataTarget target);
        void addVitalModifier(uint32_t id, uint32_t sourceId, 
                              rpg::ModifierSourceType sourceType, int32_t value,
                              VitalDataTarget target);
        void addVitalModifier(const Vital& vital, uint32_t sourceId, 
                              rpg::ModifierSourceType sourceType, int32_t value,
                              VitalDataTarget target);

        /**
         * Removes a min or max modifier by source ID and type. This uses the target to 
         * determine which value to modify.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @param sourceId[uint32_t] ID of the source.
         * @param sourceType[ModifierSourceType] Type of source.
         * @param target[pVitalDataTarget] - The target modifier type to modify.
         * @return True if a modifier was removed; false if not found.
         */
        void removeVitalModifier(const std::string& name, uint32_t sourceId, 
                                 rpg::ModifierSourceType sourceType,
                                 VitalDataTarget target);
        void removeVitalModifier(uint32_t id, uint32_t sourceId, 
                                 rpg::ModifierSourceType sourceType,
                                 VitalDataTarget target);
        void removeVitalModifier(const Vital& vital, uint32_t sourceId, 
                                 rpg::ModifierSourceType sourceType,
                                 VitalDataTarget target);

        /**
         * Removes a vital by identifier or Vital object.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @return True if the vital was removed; false if it does not exist.
         */
        void removeVital(const std::string& name);
        void removeVital(uint32_t id);
        void removeVital(const Vital& vital);

        /**
         * Serializes the Vitals to a compact string enclosed by unique markers
         * for reliable extraction within a larger data stream.
         *
         * Format: [CV_BEGIN]id:current,max,min;mod1SourceId,mod1SourceType,mod1Value;...[CV_END]
         *
         * @return A string representing the serialized state of the Vitals.
         */
        std::string serialize() const;

        /**
         * Deserializes a Vitals instance from a string containing serialized data.
         * Searches for the block enclosed by [CV_BEGIN] and [CV_END] markers,
         * then parses and reconstructs the vitals.
         *
         * @param data[const std::string&] - A string containing the serialized Vitals.
         * @return A reconstructed Vitals instance.
         */
        static Vitals deserialize(const std::string& data);

    private:
        std::unordered_map<uint32_t, VitalData> vitals; ///< Map of vital ID to their Vital instance.
    }; // class Vitals
} // namespace stats
