/**
 * File: CharacterVitals.hpp
 * Author: Antonius Torode
 * Created on: 07/07/2025
 * Description: A class representing configurable vitals for a character.
 */
#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <sstream>

namespace rpg
{
    /**
     * Represents a character's vital resource with a current and maximum value.
     * Both current and max are integers to allow positive and negative states.
     * 
     * Ensures you can clamp current between min and max, and handle depletion or overflow.
     */
    struct Vital
    {
        int current{100}; ///< Current amount of the vital.
        int max{100};     ///< Maximum amount of the vital.
        int min{0};       ///< Minimum amount of the vital.
    };

    class CharacterVitals
    {
    public:
        /**
         * Default constructor.
         */
        CharacterVitals() = default;

        /**
         * Gets the Vital associated with the given name.
         * If the vital does not exist, returns a default Vital.
         *
         * @param name The name of the vital (e.g., "health", "mana").
         * @return The Vital associated with the name.
         */
        Vital getVital(const std::string& name) const;

        /**
         * Sets a Vital by name, clamping its current value within the specified min and max bounds.
         *
         * @param name The name of the vital to set (e.g., "health", "mana").
         * @param vital The new Vital values to apply.
         */
        void setVital(const std::string& name, const Vital& vital);

        /**
         * Adds a new vital with the given name and default values.
         *
         * @param name The name of the new vital.
         * @return True if the vital was added; false if it already exists.
         */
        bool addVital(const std::string& name);

        /**
         * Removes a vital by name.
         *
         * @param name The name of the vital to remove.
         * @return True if the vital was removed; false if it didn't exist.
         */
        bool removeVital(const std::string& name);

        /**
         * Serializes the CharacterVitals to a compact string enclosed by unique markers
         * for reliable extraction within a larger data stream.
         *
         * Format: [CV_BEGIN]name:current,max,min;...[CV_END]
         *
         * @return A string representing the serialized state of the CharacterVitals.
         */
        std::string serialize() const;

        /**
         * Deserializes a CharacterVitals instance from a string containing serialized data.
         * Searches for the block enclosed by [CV_BEGIN] and [CV_END] markers,
         * then parses and reconstructs the vitals.
         *
         * @param data A string containing the serialized CharacterVitals, possibly among other data.
         * @return True if deserialization succeeded; false otherwise.
         */
        bool deserialize(const std::string& data);

    private:
        std::map<std::string, Vital> vitals; ///< Map of vital names to their Vital structures.
    }; // class CharacterVitals
} // namespace rpg
