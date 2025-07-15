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
     * @tparam SourceType The type of the source object (e.g., Attribute).
     * @tparam TargetRegistry The registry type for the target object (e.g., VitalRegistry).
     * @tparam TargetType The type of the target object (e.g., VitalData).
     * @param sourceId The ID of the source object in its registry.
     * @param sourceRegistry Reference to the source registry singleton.
     * @param targetRegistry Reference to the target registry singleton.
     * @throws error::MIAException if source or target object is not found.
     */
    template<typename SourceRegistry, typename SourceType, typename TargetRegistry, typename TargetType>
    void applyModifiers(uint32_t sourceId, SourceRegistry& sourceRegistry, TargetRegistry& targetRegistry)
    {
        const SourceType* source = sourceRegistry.getById(sourceId);
        if (!source)
            MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

        for (const auto& modify : source->getModifies())
        {
                stats::TargetType* target = const_cast<stats::TargetType*>(targetRegistry.getById(modify.targetId));
                if (!target)
                    MIA_THROW(error::ErrorCode::Undefined_RPG_Value);

                target->addModifier(modify.modifier, modify.targetField, true);
        }
    }
} // namespace rpg::helper_methods
