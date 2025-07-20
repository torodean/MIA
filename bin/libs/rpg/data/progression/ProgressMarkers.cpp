/**
 * @file ProgressMarkers.cpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A container class for managing multiple progress markers and their values.
 */

#include <string>
#include <iostream>

#include "ProgressMarkers.hpp"
#include "RegistryHelper.hpp"
#include "ProgressRegistry.hpp"

namespace progress
{
    namespace helper_methods 
    {
        /**
         * Resolves and retrieves a ProgressMarker object from the ProgressRegistry based on a given identifier.
         *
         * This is a wrapper around the generic rpg::helper_methods::getFromRegistry, specialized
         * for the ProgressRegistry and ProgressMarker types used within the stats namespace. It accepts an identifier
         * in the form of a name (std::string), ID (uint32_t), or a ProgressMarker object, and attempts to fetch
         * the corresponding ProgressMarker instance from the registry.
         *
         * @tparam T The type of identifier: std::string, uint32_t, or ProgressMarker.
         * @param identifier The identifier used to locate the ProgressMarker in the registry.
         * @return Pointer to the corresponding ProgressMarker object.
         * @throws MIA_THROW with Undefined_RPG_Value if the object is not found.
         */
        template<typename T>
        const ProgressMarker* getFromRegistry(const T& identifier)
        {
            return rpg::helper_methods::getFromRegistry<ProgressRegistry, ProgressMarker, T>(identifier);
        }
    } // namespace helper_methods
    
    ProgressValue& ProgressMarkers::get(const std::string& name)
    {
        const ProgressMarker* progressMarker = helper_methods::getFromRegistry(name);
        return get(*progressMarker);
    }
    ProgressValue& ProgressMarkers::get(uint32_t id)
    {
        const ProgressMarker* progressMarker = helper_methods::getFromRegistry(id);
        return get(*progressMarker);
    }
    ProgressValue& ProgressMarkers::get(const ProgressMarker& progressMarker)
    {
        return dataStore[0];
    }
    

    void ProgressMarkers::add(const std::string& name, uint32_t value)
    {
    
    }
    void ProgressMarkers::add(uint32_t id, uint32_t value)
    {
    
    }
    void ProgressMarkers::add(const ProgressMarker& progressMarker, uint32_t value)
    {
    
    }


    void ProgressMarkers::remove(const std::string& name)
    {
    
    }
    void ProgressMarkers::remove(uint32_t id)
    {
    
    }
    void ProgressMarkers::remove(const ProgressMarker& progressMarker)
    {
    
    }
    

    void ProgressMarkers::update(const std::string& name, unsigned int value)
    {
    
    }
    void ProgressMarkers::update(uint32_t id, unsigned int value)
    {
    
    }
    void ProgressMarkers::update(const ProgressMarker& progressMarker, unsigned int value)
    {
    
    }


    bool ProgressMarkers::has(const std::string& name, unsigned int value) const
    {
        return true;
    }
    bool ProgressMarkers::has(uint32_t id, unsigned int value) const
    {
        return true;
    }
    bool ProgressMarkers::has(const ProgressMarker& progressMarker, unsigned int value) const
    {
        return true;
    }


    void ProgressMarkers::dump(std::ostream& os) const
    {
    
    }
    

    std::string ProgressMarkers::serialize() const
    {
        return "";
    }
    

    ProgressMarkers ProgressMarkers::deserialize(const std::string& data)
    {
        return ProgressMarkers();
    }
    
} // namespace progress
