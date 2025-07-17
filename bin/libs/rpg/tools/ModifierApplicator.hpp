/**
 * @file ModifierApplicator.hpp
 * @author Antonius Torode
 * @date 07/14/2025
 * Description: Templated utility for applying modifiers from source objects to target objects.
 */

#pragma once

#include <string>
#include "Registry.hpp"
#include "Modifies.hpp"
#include "MIAException.hpp"

namespace rpg::helper_methods
{
    /**
     * Applies modifiers from a source object's modifies field to a target object in its registry.
     *
     * @tparam SourceRegistry The registry type for the source object (e.g., AttributeRegistry).
     *     This is needed to gather the 'modifies' values directly from the registry.
     * @tparam SourceStorageType The type of the source object (e.g., Attribute).
     *     This is the container which contains the values which hold modifies objects.
     * @tparam TargetStorageType The type of the target object (e.g., VitalData).
     *     This contains the modifiers objects which ultimately need modified.
     * 
     * @param sourceRegistry[SourceRegistry&] - Reference to the source registry singleton.
     * @param SourceStorageType[SourceStorageType&] - Reference to the source storage.
     * @param TargetStorageType[TargetStorageType&] - Reference to the target storage.
     * @throws error::MIAException if source or target object is not found.
     */
    template<typename SourceRegistry, typename SourceStorageType, typename TargetStorageType>
    void applyModifiers(SourceRegistry& sourceRegistry, 
                        SourceStorageType& sourceStorage, 
                        TargetStorageType& targetStorage)
    {
        // Loop over all source data from the source storage.
        for (auto& sourceData : sourceStorage)
        {
            // Get some basic information about the data types and base values.
            auto& source = sourceRegistry.getInstance().getByID(sourceData.second.getID());
            if (!source)
                throw error::MIAException("Source object not found");
            
            int sourceDataValue = sourceData.second.getCurrent();
            
            // Loop over the configured sources and get their modifies values.
            if (!source.getModifies().empty())
            {
                for (const auto& modifies : source.getModifies())
                {
                    ModifyType modifyType = modifies.modifyType;
                    double modifiesValue = modifies.modifyValuePer;
                    if (modifyType == ModifyType::ADD_MAX)
                    {
                        int modifyValue = static_cast<int>(modifiesValue * sourceDataValue);
                        Modifier mod(source.getID(), 
                                     stringToModifierSourceType(sourceRegistry.getInstance().getJsonKey()), 
                                     modifyValue);
                        targetStorage.addMaxModifier(mod);
                    }                    
                }
            }
        }
    }
} // namespace rpg::helper_methods
