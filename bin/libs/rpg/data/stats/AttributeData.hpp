/**
 * @file AttributeData.hpp
 * @author Antonius Torode
 * @date 07/13/2025
 * Description: A class representing configurable attribute data for storing an active attribute.
 */
#pragma once

#include <vector>
#include "Modifier.hpp"

namespace stats
{
    /**
     * A struct to hold an attribute's dynamic values.
     */
    struct AttributeData
    {
        /**
         * Constructs an AttributeData object with an initial current value.
         *
         * @param curr[int] - Initial current value.
         */
        AttributeData(int curr);

        /**
         * Constructs an AttributeData object with a current value and modifiers.
         *
         * @param curr[int] - Initial current value.
         * @param mods[const std::vector<rpg::Modifier<int>>&] - Modifiers affecting the current value.
         */
        AttributeData(int curr, const std::vector<rpg::Modifier<int>>& mods);

        /**
         * Adds a modifier to the current value and updates the current value.
         *
         * @param mod[const rpg::Modifier<int>&] - The modifier to add.
         */
        void addModifier(const rpg::Modifier<int>& mod);

        /**
         * Removes a modifier from the current value and updates the current value.
         *
         * @param mod The modifier to remove (matched by sourceId and source only).
         */
        void removeModifier(const rpg::Modifier<int>& mod);
        
        /**
         * Returns the current value of the attribute.
         *
         * @return [int] - The current value of the attribute, including modifier effects.
         */
        int getCurrent();

    private:
    
        int current{0};   ///< Current value of the attribute.
        std::vector<rpg::Modifier<int>> modifiers; ///< Modifiers affecting the current value.
        
        /**
         * Recalculates the current value after a modifier is added by adding the mod value.
         *
         * @param mod[const rpg::Modifier<int>&] - The modifier to add.
         */
        void recalculateAdd(const rpg::Modifier<int>& mod);
        
        /**
         * Recalculates the current value after a modifier is removed by removing the mod value.
         *
         * @param mod[const rpg::Modifier<int>&] - The modifier to add.
         */
        void recalculateRemove(const rpg::Modifier<int>& mod);
    };
} // namespace stats
