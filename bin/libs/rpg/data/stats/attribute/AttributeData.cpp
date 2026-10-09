/**
 * @file AttributeData.cpp
 * @author Antonius Torode
 * @date 07/13/2025
 * @brief A class representing configurable attribute data for storing an active attribute.
 */

// Include associated header file.
#include "AttributeData.hpp"

#include <algorithm>
#include <vector>


namespace stats
{
    AttributeData::AttributeData(int baseValue) : baseValue(baseValue) {}


    AttributeData::AttributeData(int baseValue, const std::vector<rpg::Modifier>& mods)
        : baseValue(baseValue), modifiers(mods) {}


    void AttributeData::addModifier(const rpg::Modifier& mod)
    {
        rpg::attachModifier(modifiers, mod);
    }


    void AttributeData::removeModifier(const rpg::Modifier& mod)
    {
        auto it = std::find_if(modifiers.begin(), modifiers.end(),
            [&mod](const rpg::Modifier& m)
            {
                return m.sourceID == mod.sourceID && m.source == mod.source;
            });
        if (it != modifiers.end())
            modifiers.erase(it);
    }


    int AttributeData::getCurrent() const
    {
        return rpg::computeModifiedValue(baseValue, modifiers);
    }


    void AttributeData::setBaseValue(int value)
    {
        baseValue = value;
    }


    const std::vector<rpg::Modifier>& AttributeData::getModifiers() const
    {
        return modifiers;
    }
} // namespace stats
