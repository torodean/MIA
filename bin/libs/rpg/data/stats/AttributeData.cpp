/**
 * @file AttributeData.cpp
 * @author Antonius Torode
 * @date 07/13/2025
 * Description: A class representing configurable attribute data for storing an active attribute.
 */

#include <algorithm>
#include <vector>

#include "Modifier.hpp"
#include "AttributeData.hpp"

namespace stats
{
    AttributeData::AttributeData(int curr) : current(curr) {}


    AttributeData::AttributeData(int curr, const std::vector<rpg::Modifier<int>>& mods)
        : current(curr), modifiers(mods) {}


    void AttributeData::addModifier(const rpg::Modifier<int>& mod)
    {
        auto it = std::find(modifiers.begin(), modifiers.end(), mod);
        if (it != modifiers.end())
            return; // Modifier already exists

        modifiers.push_back(mod);
        recalculateAdd(mod);
    }


    void AttributeData::removeModifier(const rpg::Modifier<int>& mod)
    {
        auto it = std::find_if(modifiers.begin(), modifiers.end(),
            [&mod](const rpg::Modifier<int>& m)
            {
                return m.sourceID == mod.sourceID && m.source == mod.source;
            });
        if (it != modifiers.end())
        {
            modifiers.erase(it);
            recalculateRemove(mod);
        }
    }
    

    int AttributeData::getCurrent()
    { 
        return current; 
    }


    void AttributeData::recalculateAdd(const rpg::Modifier<int>& mod)
    {
        current += mod.value;
    }
    
    
    const std::vector<rpg::Modifier<int>>& AttributeData::getModifiers() const
    { 
        return modifiers;
    }
    

    void AttributeData::recalculateRemove(const rpg::Modifier<int>& mod)
    {
        current -= mod.value;
    }
} // namespace stats
