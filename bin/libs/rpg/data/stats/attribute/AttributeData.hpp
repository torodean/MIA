/**
 * @file AttributeData.hpp
 * @author Antonius Torode
 * @date 07/13/2025
 * @brief A class representing configurable attribute data for storing an active attribute.
 */
#pragma once

#include <vector>
#include "Modifier.hpp"

namespace stats
{
    /**
     * A struct to hold an attribute's dynamic values.
     *
     * The attribute stores its base value along with the modifiers attached to it.
     * The effective current value is computed from the base and the modifiers whenever
     * it is read, so adding, removing, or replacing a modifier needs no recalculation
     * and the value always derives from the stored base.
     */
    struct AttributeData
    {
        /**
         * Constructs an AttributeData object with an initial base value.
         *
         * @param baseValue Initial base value.
         */
        AttributeData(int baseValue);

        /**
         * Constructs an AttributeData object with a base value and modifiers.
         *
         * @param baseValue Initial base value.
         * @param mods Modifiers affecting the current value.
         */
        AttributeData(int baseValue, const std::vector<rpg::Modifier>& mods);

        /**
         * Adds a modifier to the attribute. A modifier with the same source, source
         * type, and modify type replaces the existing one rather than stacking.
         *
         * @param mod The modifier to add.
         */
        void addModifier(const rpg::Modifier& mod);

        /**
         * Removes every modifier which matches the given modifier's source ID and source
         * type, regardless of modify type.
         *
         * @param mod The modifier whose source identifies what to remove.
         */
        void removeModifier(const rpg::Modifier& mod);

        /**
         * Returns the effective current value: the base value with its modifiers applied.
         *
         * @return The current value of the attribute, including modifier effects.
         */
        int getCurrent() const;

        /**
         * Sets the base value of the attribute.
         *
         * @param value The value to set the base to.
         */
        void setBaseValue(int value);

        /**
         * Returns the base value before modifiers.
         *
         * @return The base value of the attribute.
         */
        int getBaseValue() const { return baseValue; }

        /**
         * Returns the modifiers affecting this attribute.
         *
         * @return The modifier vector.
         */
        const std::vector<rpg::Modifier>& getModifiers() const;

    private:

        int baseValue{0};   ///< Base value of the attribute before modifiers.
        std::vector<rpg::Modifier> modifiers; ///< Modifiers affecting the current value.
    };
} // namespace stats
