/**
 * File: Vitals.hpp
 * Author: Antonius Torode
 * Created on: 07/07/2025
 * Description: A class representing configurable vitals for a character.
 */
#pragma once

#include <algorithm>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>

#include <Modifier.hpp>
#include "Vital.hpp"

namespace stats
{

    /**
     * An enum to track which type of modifier is being added or removed.
     */
    enum class VitalDataModifierTarget
    {
        UNKNOWN,      ///< Unknown or unspecified modifier target.
        CURRENT_MIN,  ///< Modifier targets the minimum value of a vital.
        CURRENT_MAX,  ///< Modifier targets the base maximum value of a vital.
    };
    
    /**
     * Converts a VitalDataModifierTarget enum to its string representation.
     *
     * @param type The VitalDataModifierTarget enum value.
     * @return A string corresponding to the VitalDataModifierTarget.
     *         Returns "UNKNOWN" if the type is not recognized.
     */
    inline std::string vitalDataModifierTargetToString(const VitalDataModifierTarget& type)
    {
        switch (type)
        {
            case VitalDataModifierTarget::UNKNOWN:     return "UNKNOWN";
            case VitalDataModifierTarget::CURRENT_MIN: return "CURRENT_MIN";
            case VitalDataModifierTarget::CURRENT_MAX: return "CURRENT_MAX";
            default:                                  return "UNKNOWN";
        }
    }

    /**
     * Converts a string to a VitalDataModifierTarget enum.
     *
     * Transforms the input string to uppercase and matches it against known VitalDataModifierTarget values.
     * Returns VitalDataModifierTarget::UNKNOWN if the string does not correspond to any valid type.
     *
     * @param typeStr The string representation of the VitalDataModifierTarget.
     * @return The corresponding VitalDataModifierTarget enum value.
     */
    inline VitalDataModifierTarget stringToVitalDataModifierTarget(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "UNKNOWN")     return VitalDataModifierTarget::UNKNOWN;
        if (str == "CURRENT_MIN") return VitalDataModifierTarget::CURRENT_MIN;
        if (str == "CURRENT_MAX") return VitalDataModifierTarget::CURRENT_MAX;
        return VitalDataModifierTarget::UNKNOWN;
    }

    /**
     * A struct to hold a vital's dynamic values.
     */
    struct VitalData
    {
        int current{0};      ///< Current value.
        int currentMin{0};   ///< Minimum value (usually 0).
        int currentMax{100}; ///< Base maximum value.
        std::vector<rpg::Modifier<int>> maxModifiers; ///< Modifiers affecting max value.
        std::vector<rpg::Modifier<int>> minModifiers; ///< Modifiers affecting min value.

        VitalData(int c = 0, int cMin = 0, int cMax = 100)
            : current(c), currentMin(cMin), currentMax(cMax) {}
    };
    
    class Vitals
    {
    public:
        /**
         * Default constructor.
         */
        Vitals() = default;

        /**
         * Gets the Vital associated with the given identifier.
         * Overloads allow querying by vital name, vital ID, or Vital object (by reference).
         *
         *
         * @param name[const std::string&] - The name of the vital (e.g., "health", "mana").
         *        id[uint32_t] - The ID of the vital.
         *        vital[const Vital&] - The Vital object (returns the matching stored Vital or default if not found).
         * @return The Vital associated with the identifier, or a default Vital if not found.
         */
        VitalData getVitalData(const std::string& name) const;
        VitalData getVitalData(uint32_t id) const;
        VitalData getVitalData(const Vital& vital) const;
        
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
        bool addVitalData(const std::string& name, int current, int min, int max);
        bool addVitalData(uint32_t id, int current, int min, int max);
        bool addVitalData(const Vital& vital, int current, int min, int max);
        
        /**
         * Updates the current value of a Vital, clamped between min and effective max.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @param current[int] The new current value.
         * @return True if the vital was updated; false if it does not exist.
         */
        bool updateVitalCurrent(const std::string& name, int current);
        bool updateVitalCurrent(uint32_t id, int current);
        bool updateVitalCurrent(const Vital& vital, int current);

        /**
         * Updates the base minimum value of a Vital.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @param min[int] The new minimum value.
         * @return True if the vital was updated; false if it does not exist.
         */
        bool updateVitalMin(const std::string& name, int min);
        bool updateVitalMin(uint32_t id, int min);
        bool updateVitalMin(const Vital& vital, int min);

        /**
         * Updates the base maximum value of a Vital.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @param max[int] The new maximum value.
         * @return True if the vital was updated; false if it does not exist.
         */
        bool updateVitalMax(const std::string& name, int max);
        bool updateVitalMax(uint32_t id, int max);
        bool updateVitalMax(const Vital& vital, int max);
        
        /**
         * Adds a modifier to a vital's max value.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @param sourceId[uint32_t] ID of the source (e.g., attribute or item ID).
         * @param sourceType[ModifierSourceType] Type of source (e.g., ATTRIBUTE).
         * @param value[int32_t] The modifier value.
         * @param target[pVitalDataModifierTarget] - The target modifier type to modify.
         * @return True if the modifier was added; false if the vital does not exist.
         */
        bool addVitalModifier(const std::string& name, uint32_t sourceId, 
                              rpg::ModifierSourceType sourceType, int32_t value,
                              VitalDataModifierTarget target);
        bool addVitalModifier(uint32_t id, uint32_t sourceId, 
                              rpg::ModifierSourceType sourceType, int32_t value,
                              VitalDataModifierTarget target);
        bool addVitalModifier(const Vital& vital, uint32_t sourceId, 
                              rpg::ModifierSourceType sourceType, int32_t value,
                              VitalDataModifierTarget target);

        /**
         * Removes a max modifier by source ID and type.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @param sourceId[uint32_t] ID of the source.
         * @param sourceType[ModifierSourceType] Type of source.
         * @param target[pVitalDataModifierTarget] - The target modifier type to modify.
         * @return True if a modifier was removed; false if not found.
         */
        bool removeVitalModifier(const std::string& name, uint32_t sourceId, 
                                 rpg::ModifierSourceType sourceType,
                                 VitalDataModifierTarget target);
        bool removeVitalModifier(uint32_t id, uint32_t sourceId, 
                                 rpg::ModifierSourceType sourceType,
                                 VitalDataModifierTarget target);
        bool removeVitalModifier(const Vital& vital, uint32_t sourceId, 
                                 rpg::ModifierSourceType sourceType,
                                 VitalDataModifierTarget target);

        /**
         * Removes a vital by identifier or Vital object.
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @return True if the vital was removed; false if it does not exist.
         */
        bool removeVital(const std::string& name);
        bool removeVital(uint32_t id);
        bool removeVital(const Vital& vital);

        /**
         * Gets the effective max value (base max + modifiers).
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @return The effective max value, or 0 if the vital does not exist.
         */
        int getVitalEffectiveMax(const std::string& name) const;
        int getVitalEffectiveMax(uint32_t id) const;
        int getVitalEffectiveMax(const Vital& vital) const;

        /**
         * Gets the effective min value (base min + modifiers).
         *
         * @param name[const std::string&] The name of the vital.
         *        id[uint32_t] The ID of the vital.
         *        vital[const Vital&] The Vital object.
         * @return The effective min value, or 0 if the vital does not exist.
         */
        int getVitalEffectiveMin(const std::string& name) const;
        int getVitalEffectiveMin(uint32_t id) const;
        int getVitalEffectiveMin(const Vital& vital) const;

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
         * @param data A string containing the serialized Vitals.
         * @return True if deserialization succeeded; false otherwise.
         */
        bool deserialize(const std::string& data);

    private:
        std::unordered_map<uint32_t, VitalData> vitals; ///< Map of vital ID to their Vital instance.
    }; // class Vitals
} // namespace stats
