/**
 * File: Vitals.hpp
 * Author: Antonius Torode
 * Created on: 07/07/2025
 * Description: A class representing configurable vitals for a character.
 */
#pragma once

#include <algorithm>
#include <unordered_map>
#include <string>
#include <sstream>

#include "Vital.hpp"

namespace rpg
{
    class Vitals
    {
    public:
        /**
         * Default constructor.
         */
        Vitals() = default;

        /**
         * Gets the Vital associated with the given identifier.
         * Overloads allow querying by vital name, vital ID, or Vital object (by reference).
         *
         * @param name[const std::string&] - The name of the vital (e.g., "health", "mana").
         *        id[uint32_t] - The ID of the vital.
         *        vital[const Vital&] - The Vital object (returns the matching stored Vital or default if not found).
         * @return The Vital associated with the identifier, or a default Vital if not found.
         */
        Vital getVital(const std::string& name) const;
        Vital getVital(uint32_t id) const;
        Vital getVital(const Vital& vital) const;
        
        /**
         * Updates the current value of a Vital by name or ID, clamped between min and max.
         *
         * @param name[const std::string&] - The name of the vital to update.
         *        id[uint32_t] - The ID of the vital to update.
         * @param current[int] - The new current value.
         * @return True if the vital was updated; false if it does not exist.
         */
        bool updateVitalCurrent(const std::string& name, int current);
        bool updateVitalCurrent(uint32_t id, int current);

        /**
         * Updates the minimum value of a Vital by name or ID.
         *
         * @param name[const std::string&] - The name of the vital to update.
         *        id[uint32_t] - The ID of the vital to update.
         * @param min[int] - The new minimum value.
         * @return True if the vital was updated; false if it does not exist.
         */
        bool updateVitalMin(const std::string& name, int min);
        bool updateVitalMin(uint32_t id, int min);

        /**
         * Updates the maximum value of a Vital by name or ID.
         *
         * @param name[const std::string&] - The name of the vital to update.
         *        id[uint32_t] - The ID of the vital to update.
         * @param max[int] - The new maximum value.
         * @return True if the vital was updated; false if it does not exist.
         */
        bool updateVitalMax(const std::string& name, int max);
        bool updateVitalMax(uint32_t id, int max);

        /**
         * Adds a new vital with default values or from a Vital object.
         * Overloads allow adding by vital name, vital ID, or Vital object.
         *
         * @param name[const std::string&] - The name of the new vital.
         *        id[uint32_t] - The ID of the new vital.
         *        vital[const Vital&] - The Vital object to add.
         * @return True if the vital was added; false if it already exists.
         */
        bool addVital(const std::string& name);
        bool addVital(uint32_t id);
        bool addVital(const Vital& vital);

        /**
         * Removes a vital by identifier or Vital object.
         * Overloads allow removing by vital name, vital ID, or Vital object.
         *
         * @param name[const std::string&] - The name of the vital to remove.
         *        id[uint32_t] - The ID of the vital to remove.
         *        vital[const Vital&] - The Vital object to remove.
         * @return True if the vital was removed; false if it didn't exist.
         */
        bool removeVital(const std::string& name);
        bool removeVital(uint32_t id);
        bool removeVital(const Vital& vital);

        /**
         * Serializes the Vitals to a compact string enclosed by unique markers
         * for reliable extraction within a larger data stream.
         *
         * Format: [CV_BEGIN]name:current,max,min;...[CV_END]
         *
         * @return A string representing the serialized state of the Vitals.
         */
        std::string serialize() const;

        /**
         * Deserializes a Vitals instance from a string containing serialized data.
         * Searches for the block enclosed by [CV_BEGIN] and [CV_END] markers,
         * then parses and reconstructs the vitals.
         *
         * @param data A string containing the serialized Vitals, possibly among other data.
         * @return True if deserialization succeeded; false otherwise.
         */
        bool deserialize(const std::string& data);

    private:
        std::unordered_map<uint32_t, Vital> vitals; ///< Map of vital ID to their Vital instance.
        std::unordered_map<std::string, uint32_t> nameToId; ///< Map of name to ID.
    }; // class Vitals
} // namespace rpg
