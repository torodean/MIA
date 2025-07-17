/**
 * @file VitalData.hpp
 * @author Antonius Torode
 * @date 07/12/2025
 * Description: A class representing configurable vital Data for storing an active vital.
 */
#pragma once

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

#include "Vital.hpp"
#include "Modifier.hpp"

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
    std::string VitalDataTargetToString(const VitalDataTarget& type);

    /**
     * Converts a string to a VitalDataTarget enum.
     *
     * Transforms the input string to uppercase and matches it against known VitalDataTarget values.
     * Returns VitalDataTarget::UNKNOWN if the string does not correspond to any valid type.
     *
     * @param typeStr The string representation of the VitalDataTarget.
     * @return The corresponding VitalDataTarget enum value.
     */
    VitalDataTarget stringToVitalDataTarget(const std::string& typeStr);

    /**
     * A struct to hold a vital's dynamic values.
     */
    struct VitalData
    {
        /**
         * Constructs a VitalData object with explicit values.
         * 
         * @param curr Initial current value.
         * @param cMin Initial minimum value.
         * @param cMax Initial maximum value.
         */
        VitalData(int curr, int cMin, int cMax);
    
        /**
         * Constructs a VitalData object based on the vital type.
         * 
         * @param type The VitalType indicating depletion or accumulation behavior.
         * @param cMin Initial minimum value.
         * @param cMax Initial maximum value.
         */
        VitalData(VitalType type, int cMin, int cMax);
        
        /**
         * Constructs a VitalData object with all values explicitly provided.
         * 
         * @param curr Initial current value.
         * @param cMin Initial minimum value.
         * @param cMax Initial maximum value.
         * @param maxMods Modifiers affecting the currentMax value.
         * @param minMods Modifiers affecting the currentMin value.
         */
        VitalData(int curr,
                  int cMin,
                  int cMax,
                  const std::vector<rpg::Modifier<int>>& maxMods,
                  const std::vector<rpg::Modifier<int>>& minMods);
           
           /**
         * Adds a modifier to either the maximum value and updates the corresponding value.
         *
         * @param mod[const rpg::Modifier<int>&] - The modifier to add.
         * @param recalc[bool] - true if a recalculation is requested. Should typically only be false
         *     for deserialization.
         */
        void addMaxModifier(const rpg::Modifier<int>& mod, bool recalc = true);
         
        /**
         * Adds a modifier to either the minimum or maximum value and updates the corresponding value.
         *
         * @param mod[const rpg::Modifier<int>&] - The modifier to add.
         * @param target[VitalDataTarget] - Enum indicating whether to modify currentMin or currentMax.
         * @param recalc[bool] - true if a recalculation is requested. Should typically only be false
         *     for deserialization.
         */
        void addModifier(const rpg::Modifier<int>& mod, VitalDataTarget target, bool recalc = true);

        /**
         * Removes a modifier from either the minimum or maximum value and updates the corresponding value.
         *
         * @param mod The modifier to remove (matched by sourceId and source only).
         * @param target Enum indicating whether to modify currentMin or currentMax.
         */
        void removeModifier(const rpg::Modifier<int>& mod, VitalDataTarget target);
        
        /**
         * Returns the current value of the vital.
         *
         * @return[int] - The current value of the vital.
         */
        int getCurrent() const { return current; }

        /**
         * Sets the current value of the vital.
         *
         * @param curr[int] - The new current value.
         */
        void setCurrent(int curr) { current = std::clamp(curr, currentMin, currentMax); }

        /**
         * Returns the current minimum value of the vital.
         *
         * @return[int] - The current minimum value of the vital.
         */
        int getCurrentMin() const { return currentMin; }

        /**
         * Sets the current minimum value of the vital.
         *
         * @param cMin[int] - The new minimum value.
         */
        void setCurrentMin(int cMin) { currentMin = cMin; }

        /**
         * Returns the current maximum value of the vital.
         *
         * @return[int] - The current maximum value of the vital.
         */
        int getCurrentMax() const { return currentMax; }

        /**
         * Sets the current maximum value of the vital.
         *
         * @param cMax[int] - The new maximum value.
         */
        void setCurrentMax(int cMax) { currentMax = cMax; }
        
        /**
         * Returns the modifiers for the specified target.
         *
         * @param target[VitalDataTarget] - Enum indicating which modifier vector to return (CURRENT_MIN or CURRENT_MAX).
         * @return [const std::vector<rpg::Modifier<int>>&] - The modifier vector for the specified target.
         * @throws [std::invalid_argument] If the target is UNKNOWN or CURRENT.
         */
        const std::vector<rpg::Modifier<int>>& getModifiers(VitalDataTarget target) const;
        
    private:
    
        int current{0};   ///< Current value.
        int currentMin;   ///< Current minimum value (usually 0).
        int currentMax;   ///< Current maximum value.
        std::vector<rpg::Modifier<int>> maxModifiers; ///< Modifiers affecting max value.
        std::vector<rpg::Modifier<int>> minModifiers; ///< Modifiers affecting min value.
    
        /**
         * Recalculates either the currentMin or currentMax value after a modifier is added or removed.
         *
         * @param mod The modifier used for the update.
         * @param target Enum indicating which value to update.
         */
        void recalculate(const rpg::Modifier<int>& mod, VitalDataTarget target);
    };
} // namespace stats
