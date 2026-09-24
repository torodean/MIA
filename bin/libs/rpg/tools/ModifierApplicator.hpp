/**
 * @file ModifierApplicator.hpp
 * @author Antonius Torode
 * @date 07/14/2025
 * @brief: Utility for applying modifiers from source objects to target objects.
 */

#pragma once

#include <string>

#include "Registry.hpp"
#include "Modifies.hpp"
// Used for error handling.
#include "MIAException.hpp"
#include "Error.hpp"

namespace rpg::helper_methods
{
    /**
     * Applies modifiers from a source object's modifies field to a target object in its registry.
     *
     * @tparam SourceRegistry The registry type for the source object (e.g., AttributeRegistry).
     *     This is needed to gather the 'modifies' values directly from the registry.
     * @tparam TargetRegistry The registry type for the target object (e.g., VitalRegistry).
     *     This is needed to set the 'modifiers' values by ID.
     * @tparam SourceStorageType The type of the source object (e.g., Attribute).
     *     This is the container which contains the values which hold modifies objects.
     * @tparam TargetStorageType The type of the target object (e.g., VitalData).
     *     This contains the modifiers objects which ultimately need modified.
     * 
     * @param sourceRegistry Reference to the source registry singleton.
     * @param targetRegistry Reference to the target registry singleton.
     * @param SourceStorageType Reference to the source storage.
     * @param TargetStorageType Reference to the target storage.
     * @throws error::MIAException if source or target object is not found.
     */
    template<typename SourceRegistry, 
             typename TargetRegistry, 
             typename SourceStorageType, 
             typename TargetStorageType>
    void applyModifiers(SourceRegistry& sourceRegistry, 
                        TargetRegistry& targetRegistry,
                        SourceStorageType& sourceStorage, 
                        TargetStorageType& targetStorage)
    {
        for (auto& sourceData : sourceStorage.getMap())
        {
            // First, access the singleton instance of the source.
            const auto& source = sourceRegistry.getInstance().getByID(sourceData.first);
            if (!source)
            { // Error case.
                std::string err = "Source object not found.";
                MIA_THROW(error::ErrorCode::Undefined_RPG_Value, err);
            }

            if (source->getModifies().empty())
            { // Check if any modifiers exist in the source.
                return;
            }

            int sourceDataValue = sourceData.second.getCurrent();
            
            for (const auto& modifies : source->getModifies())
            {                
                switch (modifies.modifyType)
                {
                    case rpg::ModifyType::ADD_MAX:
                    {
                        int modifyValue = static_cast<int>(modifies.modifyValuePer * sourceDataValue);
                        rpg::Modifier mod(source->getID(),
                                          stringToModifierSourceType(sourceRegistry.getInstance().getJsonKey()),
                                          modifyValue);

                        // Apply the modifier to the target.
                        targetStorage.addModifier(modifies.targetName, mod);
                        break;
                    }

                    case rpg::ModifyType::MULTIPLY:
                        // TODO - how should multiple work. Should there be multiple multiply options?
                        break;

                    case rpg::ModifyType::SET:
                    {
                        // TODO - think about how setting works. Modifier might need an additional field.
                        break;
                    }

                    case rpg::ModifyType::UNKNOWN:
                    default:
                        break;
                }
            }
        }
    }
} // namespace rpg::helper_methods
