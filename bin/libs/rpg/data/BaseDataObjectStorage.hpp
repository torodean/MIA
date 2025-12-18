/**
 * @file BaseDataObjectStorage.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A container class for managing multiple Base Data objects and 
 * the interactions with the container.
 */
#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

namespace data
{
    /**
     * A generic interface for containers that store and manage keyed data objects.
     * Provides lookup, serialization, and dumping mechanisms. Implementations must define
     * how objects are retrieved and serialized.
     *
     * Designed to be subclassed—base deserialization is intentionally deleted to enforce
     * concrete behavior in derived classes.
     */
    template<typename dataType, typename dataStorageType>
    class BaseDataObjectStorage
    {
    public:
        /// Define a storage map for data ID to data storage.
        using DataStorageMap = std::unordered_map<uint32_t, dataStorageType>;

        virtual ~BaseDataObjectStorage() = default;

        /**
         * Gets the object associated with the given identifier. If the object is not found,
         * a reference to a default-constructed value (or error) may be returned, depending on implementation.
         * Overloads support lookup by name, ID, or a reference key object.
         *
         * @param name[const std::string&] - The string identifier of the object.
         *        id[uint32_t] - The numeric identifier of the object.
         *        keyObject[const dataType&] - A key-type object (holding ID and name).
         * @return A reference to the stored data object.
         */
        virtual dataStorageType& get(const std::string& name) = 0;
        virtual dataStorageType& get(uint32_t id) = 0;
        virtual dataStorageType& get(const dataType& keyObject) = 0;

        /**
         * Serializes the container contents to a string.
         * Format and encoding must be defined by the derived class.
         *
         * @return A serialized representation of the object map.
         */
        virtual std::string serialize() const = 0;

        /**
         * Dumps the container contents to a stream, primarily for debugging.
         * @param os[std::ostream&] - The output stream to write to (defaults to std::cout).
         */
        virtual void dump(std::ostream& os = std::cout) const = 0;

        /**
         * Returns a mutable reference to the internal data storage map.
         * Useful for low-level manipulation or iteration.
         *
         * @return A reference to the internal data map.
         */
        DataStorageMap& getMap()
        { return dataStore; }
        
    protected:
        /// Map of the data ID to the associated data storage instance.
        DataStorageMap dataStore;
    };

} // namespace data

