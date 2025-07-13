/**
 * File: VitalData.hpp
 * Author: Antonius Torode
 * Created on: 07/12/2025
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
        VitalData(int c, int cMin, int cMax);
    
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
                  const std::vector<rpg::Modifier<int>>& minMods);
            
        /**
         * Adds a modifier to either the minimum or maximum value and updates the corresponding value.
         *
         * @param mod The modifier to add.
         * @param target Enum indicating whether to modify currentMin or currentMax.
         */
        void addModifier(const rpg::Modifier<int>& mod, VitalDataTarget target);

        /**
         * Removes a modifier from either the minimum or maximum value and updates the corresponding value.
         *
         * @param mod The modifier to remove (matched by sourceId and source only).
         * @param target Enum indicating whether to modify currentMin or currentMax.
         */
        void removeModifier(const rpg::Modifier<int>& mod, VitalDataTarget target);
        
    private:
    
        /**
         * Recalculates either the currentMin or currentMax value after a modifier is added or removed.
         *
         * @param mod The modifier used for the update.
         * @param target Enum indicating which value to update.
         */
        void recalculate(const rpg::Modifier<int>& mod, VitalDataTarget target);
    };
} // namespace stats
