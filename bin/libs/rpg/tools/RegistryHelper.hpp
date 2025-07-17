/**
 * @file RegistryHelper.hpp
 * @author Antonius Torode
 * @date 07/07/2025
 * Description: Helper methods for accessing registry objects..
 */
#pragma once

#include "Error.hpp"
#include "MIAException.hpp"

namespace rpg
{
    /**
     * This namespace defines some helper methods for code which is re-used
     * through this file.
     */
    namespace helper_methods
    {
        /**
         * Helper template method to validate and retrieve a Vital object from RegistryType.
         *
         * @tparam RegistryType The type of registry (e.g., CurrencyRegistry, VitalRegistry, etc).
         * @tparam ObjType The type of the object (e.g., Currency, Vital, etc).
         * @tparam SearchType The type of the identifier (std::string for name, uint32_t for ID, or ObjType for object).
         * @param identifier The identifier used to query the ObjType (name, ID, or ObjType object).
         * @return Pointer to the ObjType object if found; nullptr otherwise.
         */
        template<typename RegistryType, typename ObjType, typename SearchType>
        const ObjType* getFromRegistry(const SearchType& identifier)
        {
            const ObjType* object = nullptr;
            if constexpr (std::is_same_v<SearchType, std::string>)
            {
                object = RegistryType::getInstance().getByName(identifier);
            }
            else if constexpr (std::is_same_v<SearchType, uint32_t>)
            {
                object = RegistryType::getInstance().getByID(identifier);
            }
            else if constexpr (std::is_same_v<SearchType, ObjType>)
            {
                object = RegistryType::getInstance().getByID(identifier.getID());
            }
            if (!object)
            {
                MIA_THROW(error::ErrorCode::Undefined_RPG_Value);
            }
            else
            {
                return object;
            }
        }
    } // namespace helper_methods
} // namespace rpg
