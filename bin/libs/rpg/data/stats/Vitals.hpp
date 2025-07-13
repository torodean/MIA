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
    enum class VitalDataTarget
    {
        UNKNOWN,      ///< Unknown or unspecified modifier target.
        CURRENT,      ///< Targets the current value of the vital.
        CURRENT_MIN,  ///< Targets the minimum value of a vital (val or modifier).
        CURRENT_MAX,  ///< Targets the base maximum value of a vital (val or modifier).
    };
    
    /**
     * Converts a VitalDataTarget enum to its string representation.
     *
     * @param type The VitalDataTarget enum value.
     * @return A string corresponding to the VitalDataTarget.
     *         Returns "UNKNOWN" if the type is not recognized.
     */
    inline std::string VitalDataTargetToString(const VitalDataTarget& type)
    {
        switch (type)
        {
            case VitalDataTarget::UNKNOWN:     return "UNKNOWN";
            case VitalDataTarget::CURRENT_MIN: return "CURRENT_MIN";
            case VitalDataTarget::CURRENT_MAX: return "CURRENT_MAX";
            default:                                  return "UNKNOWN";
        }
    }

    /**
     * Converts a string to a VitalDataTarget enum.
     *
     * Transforms the input string to uppercase and matches it against known VitalDataTarget values.
     * Returns VitalDataTarget::UNKNOWN if the string does not correspond to any valid type.
     *
     * @param typeStr The string representation of the VitalDataTarget.
     * @return The corresponding VitalDataTarget enum value.
     */
    inline VitalDataTarget stringToVitalDataTarget(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);

        if (str == "UNKNOWN")     return VitalDataTarget::UNKNOWN;
        if (str == "CURRENT_MIN") return VitalDataTarget::CURRENT_MIN;
        if (str == "CURRENT_MAX") return VitalDataTarget::CURRENT_MAX;
        return VitalDataTarget::UNKNOWN;
    }

    /**
     * A struct to hold a vital's dynamic values.
     */
    struct VitalData
    {
        int current{0};   ///< Current value.
        int currentMin;   ///< Current minimum value (usually 0).
        int currentMax;   ///< Current maximum value.
        std::vector<rpg::Modifier<int>> maxModifiers; ///< Modifiers affecting max value.
        std::vector<rpg::Modifier<int>> minModifiers; ///< Modifiers affecting min value.

        /**
         * Constructs a VitalData object with explicit values.
         * 
         * @param c Initial current value.
         * @param cMin Initial minimum value.
         * @param cMax Initial maximum value.
         */
        VitalData(int c, int cMin, int cMax)
            : current(c), currentMin(cMin), currentMax(cMax) {}
    
        /**
         * Constructs a VitalData object based on the vital type.
         * 
         * @param type The VitalType indicating depletion or accumulation behavior.
         * @param cMin Initial minimum value.
         * @param cMax Initial maximum value.
         */
        VitalData(VitalType type, int cMin, int cMax)
            : currentMin(cMin), currentMax(cMax)
        {
            if (type == VitalType::ACCUMULATIVE)
                current = currentMin;
            else if (type == VitalType::DEPLETIVE)
                current = currentMax;
            else if (type == VitalType::UNKNOWN)
                current = int( (currentMax - currentMin)/2 ); // Default to half-max.
        }
        
        /**
         * Constructs a VitalData object with all values explicitly provided.
         * 
         * @param c Initial current value.
         * @param cMin Initial minimum value.
         * @param cMax Initial maximum value.
         * @param maxMods Modifiers affecting the currentMax value.
         * @param minMods Modifiers affecting the currentMin value.
         */
        VitalData(int c,
                  int cMin,
                  int cMax,
                  const std::vector<rpg::Modifier<int>>& maxMods,
                  const std::vector<rpg::Modifier<int>>& minMods)
            : current(c),
              currentMin(cMin),
              currentMax(cMax),
              maxModifiers(maxMods),
              minModifiers(minMods) {}
            
        /**
         * Adds a modifier to either the minimum or maximum value and updates the corresponding value.
         *
         * @param mod The modifier to add.
         * @param target Enum indicating whether to modify currentMin or currentMax.
         */
        void addModifier(const rpg::Modifier<int>& mod, VitalDataTarget target)
        {
            if (target == VitalDataTarget::CURRENT_MAX)
            {
                maxModifiers.push_back(mod);
                recalculate(mod, target);
            }
            else if (target == VitalDataTarget::CURRENT_MIN)
            {
                minModifiers.push_back(mod);
                recalculate(mod, target);
            }
        }

        /**
         * Removes a modifier from either the minimum or maximum value and updates the corresponding value.
         *
         * @param mod The modifier to remove (matched by sourceId and source only).
         * @param target Enum indicating whether to modify currentMin or currentMax.
         */
        void removeModifier(const rpg::Modifier<int>& mod, VitalDataTarget target)
        {
            auto match = [&](const rpg::Modifier<int>& m) {
                return m.sourceId == mod.sourceId && m.source == mod.source;
            };

            if (target == VitalDataTarget::CURRENT_MAX)
            {
                for (auto it = maxModifiers.begin(); it != maxModifiers.end(); )
                {
                    if (match(*it))
                    {
                        rpg::Modifier<int> modOut = mod;
                        modOut.value = 0 - it->value; // Negative because removing it.
                        it = maxModifiers.erase(it);
                        recalculate(modOut, target);
                    }
                    else
                        ++it;
                }
                recalculate(mod, target);
            }
            else if (target == VitalDataTarget::CURRENT_MIN)
            {
                for (auto it = minModifiers.begin(); it != minModifiers.end(); )
                {
                    if (match(*it))
                    {
                        rpg::Modifier<int> modOut = mod;
                        modOut.value = 0 - it->value; // Negative because removing it.
                        it = minModifiers.erase(it);
                        recalculate(modOut, target);
                    }
                    else
                        ++it;
                }
            }
        }
        
    private:
    
        /**
         * Recalculates either the currentMin or currentMax value after a modifier is added or removed.
         *
         * @param mod The modifier used for the update.
         * @param target Enum indicating which value to update.
         */
        void recalculate(const rpg::Modifier<int>& mod, VitalDataTarget target)
        {

            if (target == VitalDataTarget::CURRENT_MAX)
                currentMax += mod.value;
            else if (target == VitalDataTarget::CURRENT_MIN)
                currentMin += mod.value;
            // TODO - add sanity checks for values here.
        }
    };
    
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
         * @param data A string containing the serialized Vitals.
         * @return A reconstructed Vitals instance.
         */
        static Vitals deserialize(const std::string& data);

    private:
        std::unordered_map<uint32_t, VitalData> vitals; ///< Map of vital ID to their Vital instance.
    }; // class Vitals
} // namespace stats
