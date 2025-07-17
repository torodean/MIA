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
#include "Error.hpp"
#include "MIAException.hpp"

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
     * @param sourceRegistry[SourceRegistry&] - Reference to the source registry singleton.
     * @param targetRegistry[TargetRegistry&] - Reference to the target registry singleton.
     * @param SourceStorageType[SourceStorageType&] - Reference to the source storage.
     * @param TargetStorageType[TargetStorageType&] - Reference to the target storage.
     * @throws error::MIAException if source or target object is not found.
     */
    template<typename SourceRegistry, typename TargetRegistry, 
             typename SourceStorageType, typename TargetStorageType>
    void applyModifiers(SourceRegistry& sourceRegistry, 
                        TargetRegistry& targetRegistry,
                        SourceStorageType& sourceStorage, 
                        TargetStorageType& targetStorage)
{
    for (auto& sourceData : sourceStorage.getMap())
    {
        std::cout << "[DEBUG] Processing source ID: " << sourceData.first << "\n";

        const auto& source = sourceRegistry.getInstance().getByID(sourceData.first);
        if (!source)
        {
            std::string err = "Source object not found.";
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value, err);
        }

        int sourceDataValue = sourceData.second.getCurrent();
        std::cout << "[DEBUG] Source current value: " << sourceDataValue << "\n";

        if (!source->getModifies().empty())
        {
            std::cout << "[DEBUG] Found " << source->getModifies().size() << " modifies entries.\n";
            for (const auto& modifies : source->getModifies())
            {
                ModifyType modifyType = modifies.modifyType;
                double modifiesValue = modifies.modifyValuePer;

                std::cout << "[DEBUG] Modifies target: " << modifies.targetName
                          << ", type: " << static_cast<int>(modifyType)
                          << ", value per: " << modifiesValue << "\n";

                if (modifyType == ModifyType::ADD_MAX)
                {
                    int modifyValue = static_cast<int>(modifiesValue * sourceDataValue);
                    std::cout << "[DEBUG] Calculated modify value: " << modifyValue << "\n";

                    Modifier mod(
                        source->getID(),
                        stringToModifierSourceType(sourceRegistry.getInstance().getJsonKey()),
                        modifyValue
                    );

                    std::cout << "[DEBUG] Adding modifier to target: " << modifies.targetName << "\n";
                    targetStorage.addModifier(modifies.targetName, mod);
                }
            }
        }
        else
        {
            std::cout << "[DEBUG] No modifies entries found for source ID: " << sourceData.first << "\n";
        }
    }

    std::cout << "[DEBUG] Exiting applyModifiers.\n";
}
} // namespace rpg::helper_methods
