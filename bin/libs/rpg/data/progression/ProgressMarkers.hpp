/**
 * @file ProgressMarkers.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A container class for managing multiple progress markers and their values.
 */
#pragma once

#include <unordered_map>

#include "ProgressMarker.hpp"
#include "ProgressValue.hpp"
#include "BaseDataObjectStorage.hpp"

namespace progress
{
    /**
     * A container class to manage a set of progress markers and their values.
     */
    class ProgressMarkers : public data::BaseDataObjectStorage<ProgressMarker, ProgressValue>
    {
    public:
        /**
         * Default constructor.
         */
        ProgressMarkers() = default;
        
        /**
         * Gets the ProgressValue associated with the given identifier. If the data object is not found
         * in the objects map, a default constructed data object will be returned.
         * Overloads allow querying by progress marker name, progress marker ID, or ProgressMarker object (by reference).
         *
         * @param name[const std::string&] - The name of the progress marker (e.g., "experience", "defense skill").
         *        id[uint32_t] - The ID of the progress marker.
         *        progressMarker[const ProgressMarker&] - The ProgressMarker object.
         * @return The ProgressMarker associated with the identifier, or a default ProgressMarker if not found.
         */
        ProgressValue& get(const std::string& name) override;
        ProgressValue& get(uint32_t id) override;
        ProgressValue& get(const ProgressMarker& progressMarker) override;
        
        /**
         * Adds a specified progress marker to the container.
         * Overloads allow adding by ProgressMarker object, progress marker ID, or progress marker name.
         * If the progress marker is already added, this will increment the value.
         *
         * @param name[const std::string&] - The name of the progress marker (e.g., "gold", "silver").
         *        id[uint32_t] - The ID of the progress marker.
         *        progressMarker[const ProgressMarker&] - The ProgressMarker object.
         * @param value[uint32_t] - The value to add.
         */
        void add(const std::string& name, uint32_t value);
        void add(uint32_t id, uint32_t value);
        void add(const ProgressMarker& progressMarker, uint32_t value);

        /**
         * Removes a specified progress marker from the container.
         * Overloads allow removing by ProgressMarker object, progress marker ID, or progress marker name.
         *
         * @param name[const std::string&] - The name of the progress marker (e.g., "gold", "silver").
         *        id[uint32_t] - The ID of the progress marker.
         *        progressMarker[const ProgressMarker&] - The ProgressMarker object.
         */
        void remove(const std::string& name);
        void remove(uint32_t id);
        void remove(const ProgressMarker& progressMarker);
        
        /**
         * Updates a specified value of a progress marker.
         * Overloads allow updating by ProgressMarker object, progress marker ID, or progress marker name.
         *
         * @param name[const std::string&] - The name of the progress marker (e.g., "gold", "silver").
         *        id[uint32_t] - The ID of the progress marker.
         *        progressMarker[const ProgressMarker&] - The ProgressMarker object.
         * @param value[int] - The signed value to update .
         */
        void update(const std::string& name, unsigned int value);
        void update(uint32_t id, unsigned int value);
        void update(const ProgressMarker& progressMarker, unsigned int value);

        /**
         * Checks if the container has at least the specified value of a progress marker.
         * Overloads allow checking by ProgressMarker object, progress marker ID, or progress marker name.
         *
         * @param name[const std::string&] - The name of the progress marker (e.g., "gold", "silver").
         *        id[uint32_t] - The ID of the progress marker.
         *        progressMarker[const ProgressMarker&] - The ProgressMarker object.
         * @param value[uint32_t] - The required value.
         * @return True if the container has enough; false otherwise.
         */
        bool has(const std::string& name, unsigned int value) const;
        bool has(uint32_t id, unsigned int value) const;
        bool has(const ProgressMarker& progressMarker, unsigned int value) const;

        /**
         * Dumps the container's contents to the provided output stream.
         * @param os The output stream to write to (defaults to std::cout).
         */
        void dump(std::ostream& os = std::cout) const override;
        
        /**
         * Serializes the contents of the ProgressMarkers to a compact string.
         * The serialized format is enclosed between unique [WALLET_BEGIN] and [WALLET_END] markers
         * for easy identification within a larger data stream.
         *
         * Format: [WALLET_BEGIN]id1:qty1;id2:qty2;...[WALLET_END]
         *
         * @return A string representing the serialized state of the container.
         */
        std::string serialize() const override;
        
        /**
         * Deserializes a ProgressMarkers from a string containing serialized data.
         * The method searches for a block enclosed between [WALLET_BEGIN] and [WALLET_END],
         * then reconstructs the container from the progress marker ID and value pairs.
         *
         * @param data A string containing the serialized container, possibly among other data.
         * @return A reconstructed ProgressMarkers instance.
         * @throws std::invalid_argument if no valid serialized block is found.
         */
        static ProgressMarkers deserialize(const std::string& data);

    }; // class ProgressMarkers
    
} // namespace progress
