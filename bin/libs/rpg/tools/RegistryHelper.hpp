/**
 * @file RegistryHelper.hpp
 * @author Antonius Torode
 * @date 07/07/2025
 * @brief: Helper methods for accessing registry objects.
 */
#pragma once

// Used for error handling.
#include "MIAException.hpp"
#include "Error.hpp"

namespace rpg
{
    /**
     * This namespace defines some helper methods for code which is re-used
     * through this file.
     */
    namespace helper_methods
    {
        /**
         * Helper template method to validate and retrieve an object from RegistryType.
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
            { // Searches by name if a string identifier was used.
                object = RegistryType::getInstance().getByName(identifier);
            }
            else if constexpr (std::is_same_v<SearchType, uint32_t>)
            { // Searches by ID if a uint32_t identifier was used.
                object = RegistryType::getInstance().getByID(identifier);
            }
            else if constexpr (std::is_same_v<SearchType, ObjType>)
            { // If the identifier is of a derived type, search for the instance of that object.
                object = RegistryType::getInstance().getByID(identifier.getID());
            }
            if (!object)
            { // Object was never defined.
                MIA_THROW(error::ErrorCode::Undefined_RPG_Value);
            }
            else
            {
                return object;
            }
        }
    } // namespace helper_methods
} // namespace rpg
