/**
 * @file ModifierApplicator.hpp
 * @author Antonius Torode
 * @date 07/14/2025
 * @brief Utility for applying modifiers from source objects to target objects.
 */
#pragma once

#include <string>

#include "Registry.hpp"
#include "Modifies.hpp"
#include "Modifier.hpp"
// Used for error handling.
#include "MIAException.hpp"
#include "Error.hpp"

namespace rpg::helper_methods
{
    /**
     * Applies modifiers from a source object's modifies field to a target object in its registry.
     *
     * Each Modifies declaration on the source is translated into a Modifier and attached to the
     * named target. The applicator only computes the modifier value; the target data type
     * combines the modifier with its own value when it computes its effective value.
     *
     * The stored modifier value per declaration type is:
     * - ADD_MAX:  modifyValuePer * source value (an amount to add).
     * - MULTIPLY: modifyValuePer * source value (a multiplier bonus, 0.1 = +10%).
     * - SET:      modifyValuePer (the value to set the target to).
     *
     * The function is idempotent: a source's new modifier replaces its previous modifier of the
     * same type on the target, so re-applying after the source's value changes never stacks.
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
     * @param sourceStorage Reference to the source storage.
     * @param targetStorage Reference to the target storage.
     * @throws error::MIAException if the source or target object is not found.
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
        // The source type is the same for every source in this registry.
        auto sourceType = stringToModifierSourceType(
            sourceRegistry.getInstance().getJsonKey());

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
            { // Nothing to apply for this source; keep processing the others.
                continue;
            }

            // Get the current value of the data being modified.
            int sourceDataValue = sourceData.second.getCurrent();

            for (const auto& modifies : source->getModifies())
            {
                switch (modifies.modifyType)
                {
                    case rpg::ModifyType::ADD_MAX:
                    {
                        int modifyValue = static_cast<int>(
                            modifies.modifyValuePer * sourceDataValue);
                        rpg::Modifier mod(source->getID(), sourceType, modifyValue,
                                          modifies.modifyType, modifies.stackPolicy);

                        // Attach the modifier to the target.
                        targetStorage.addModifier(modifies.targetName, mod);
                        break;
                    }

                    case rpg::ModifyType::MULTIPLY:
                    {
                        double modifyValue = modifies.modifyValuePer * sourceDataValue;
                        rpg::Modifier mod(source->getID(), sourceType, modifyValue,
                                          modifies.modifyType, modifies.stackPolicy);

                        // Attach the modifier to the target.
                        targetStorage.addModifier(modifies.targetName, mod);
                        break;
                    }

                    case rpg::ModifyType::SET:
                    {
                        int modifyValue = static_cast<int>(modifies.modifyValuePer);
                        rpg::Modifier mod(source->getID(), sourceType, modifyValue,
                                          modifies.modifyType, modifies.stackPolicy);

                        // Attach the modifier to the target.
                        targetStorage.addModifier(modifies.targetName, mod);
                        break;
                    }

                    case rpg::ModifyType::UNKNOWN:
                    default:
                        break;
                } // switch (modifies.modifyType)
            } // for (const auto& modifies : source->getModifies()) 
        } // for (auto& sourceData : sourceStorage.getMap())
    } // void applyModifiers()
} // namespace rpg::helper_methods
