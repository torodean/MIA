/**
 * @file VitalData.hpp
 * @author Antonius Torode
 * @date 07/12/2025
 * @brief A class representing configurable vital Data for storing an active vital.
 */
#pragma once

#include <vector>

#include "Vital.hpp"
#include "Modifier.hpp"

namespace stats
{
    /**
     * An enum to track which value of a vital is being read or modified.
     */
    enum class VitalDataTarget
    {
        UNKNOWN,      ///< Unknown or unspecified modifier target.
        CURRENT,      ///< Targets the current value of the vital.
        CURRENT_MIN,  ///< Targets the minimum value of a vital (val or modifier).
        CURRENT_MAX,  ///< Targets the maximum value of a vital (val or modifier).
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
     *
     * The vital stores its base minimum and maximum along with the modifiers attached to it.
     * The effective minimum and maximum are computed from the base values and the modifiers
     * whenever they are read, so adding, removing, or replacing a modifier needs no
     * recalculation and the values always derive from the stored base.
     */
    struct VitalData
    {
        /**
         * Constructs a VitalData object with explicit values.
         *
         * @param curr Initial current value.
         * @param baseMin Initial base minimum value.
         * @param baseMax Initial base maximum value.
         */
        VitalData(int curr, int baseMin, int baseMax);

        /**
         * Constructs a VitalData object based on the vital type.
         *
         * @param type The VitalType indicating depletion or accumulation behavior.
         * @param baseMin Initial base minimum value.
         * @param baseMax Initial base maximum value.
         */
        VitalData(VitalType type, int baseMin, int baseMax);

        /**
         * Constructs a VitalData object with all values explicitly provided.
         *
         * @param curr Initial current value.
         * @param baseMin Initial base minimum value.
         * @param baseMax Initial base maximum value.
         * @param maxMods Modifiers affecting the maximum value.
         * @param minMods Modifiers affecting the minimum value.
         */
        VitalData(int curr,
                  int baseMin,
                  int baseMax,
                  const std::vector<rpg::Modifier>& maxMods,
                  const std::vector<rpg::Modifier>& minMods);

        /**
         * Adds a modifier to the maximum value.
         * A modifier with the same source, source type, and modify type replaces the
         * existing one rather than stacking.
         *
         * @param mod The modifier to add.
         */
        void addMaxModifier(const rpg::Modifier& mod);

        /**
         * Adds a modifier to the minimum or maximum value.
         *
         * @param mod The modifier to add.
         * @param target Enum indicating whether to modify currentMin or currentMax.
         */
        void addModifier(const rpg::Modifier& mod, VitalDataTarget target);

        /**
         * Removes every modifier from the minimum or maximum value which matches the
         * given modifier's source ID and source type, regardless of modify type.
         *
         * @param mod The modifier whose source identifies what to remove.
         * @param target Enum indicating whether to modify currentMin or currentMax.
         */
        void removeModifier(const rpg::Modifier& mod, VitalDataTarget target);

        /**
         * Returns the current value of the vital, clamped into the effective range.
         *
         * @return The current value of the vital.
         */
        int getCurrent() const;

        /**
         * Sets the current value of the vital. The value is clamped into the
         * effective minimum and maximum.
         *
         * @param curr The new current value.
         */
        void setCurrent(int curr);

        /**
         * Returns the effective minimum value: the base minimum with its modifiers applied.
         *
         * @return The effective minimum value of the vital.
         */
        int getCurrentMin() const;

        /**
         * Returns the effective maximum value: the base maximum with its modifiers applied.
         *
         * @return The effective maximum value of the vital.
         */
        int getCurrentMax() const;

        /**
         * Returns the base minimum value before modifiers.
         *
         * @return The base minimum value of the vital.
         */
        int getBaseMin() const { return baseMin; }

        /**
         * Returns the base maximum value before modifiers.
         *
         * @return The base maximum value of the vital.
         */
        int getBaseMax() const { return baseMax; }

        /**
         * Sets the base minimum value before modifiers.
         *
         * @param baseMin The new base minimum value.
         */
        void setBaseMin(int baseMin) { this->baseMin = baseMin; }

        /**
         * Sets the base maximum value before modifiers.
         *
         * @param baseMax The new base maximum value.
         */
        void setBaseMax(int baseMax) { this->baseMax = baseMax; }

        /**
         * Returns the modifiers for the specified target.
         *
         * @param target Enum indicating which modifier vector to return (CURRENT_MIN or CURRENT_MAX).
         * @return The modifier vector for the specified target.
         * @throws error::MIAException if the target is UNKNOWN or CURRENT.
         */
        const std::vector<rpg::Modifier>& getModifiers(VitalDataTarget target) const;

    private:

        int current{0};   ///< Current value.
        int baseMin;      ///< Base minimum value before modifiers.
        int baseMax;      ///< Base maximum value before modifiers.
        std::vector<rpg::Modifier> maxModifiers; ///< Modifiers affecting the maximum value.
        std::vector<rpg::Modifier> minModifiers; ///< Modifiers affecting the minimum value.
    };
} // namespace stats
