/**
 * File: CharacterVitals.hpp
 * Author: Antonius Torode
 * Created on: 07/07/2025
 * Description: A class representing the vitals for a character.
 */
#pragma once

#include <algorithm>
#include <string>

namespace rpg
{
    /**
     * Represents a character's vital resource with a current and maximum value.
     * Both current and max are integers to allow positive and negative states.
     * 
     * Usage:
     * - current: the current amount of the resource (e.g., health, hunger).
     * - max: the maximum possible amount of the resource.
     * 
     * Ensures you can clamp current between 0 and max, and handle depletion or overflow.
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
    
        /// Getters for the various data mambers.
        Vital getHealth() const { return health; }
        Vital getMana() const { return mana; }
        Vital getEnergy() const { return energy; }
        Vital getRage() const { return rage; }
        Vital getBreath() const { return breath; }
        Vital getFocus() const { return focus; }
        Vital getRadiation() const { return radiation; }
        Vital getTemperature() const { return temperature; }
        Vital getHunger() const { return hunger; }
        Vital getThirst() const { return thirst; }
        Vital getToxicity() const { return toxicity; }
        Vital getSanity() const { return sanity; }
        
        /**
         * Sets a Vital member by clamping its current value within the specified min and max bounds,
         * then updates the min and max values accordingly.
         * 
         * @tparam T The type of the Vital member (typically 'Vital').
         * @param member Reference to the Vital member to update.
         * @param vital The new Vital values to apply.
         * 
         * This function ensures the current value remains within [min, max].
         */
        template<typename T>
        void setVital(T& member, const Vital& vital) 
        {
            member.current = std::clamp(vital.current, vital.min, vital.max);
            member.max = vital.max;
            member.min = vital.min;
        }
        
        /// Setters for the various data members.
        void setHealth(const Vital& vital) { setVital(health, vital); }
        void setMana(const Vital& vital) { setVital(mana, vital); }
        void setEnergy(const Vital& vital) { setVital(energy, vital); }
        void setRage(const Vital& vital) { setVital(rage, vital); }
        void setBreath(const Vital& vital) { setVital(breath, vital); }
        void setFocus(const Vital& vital) { setVital(focus, vital); }
        void setRadiation(const Vital& vital) { setVital(radiation, vital); }
        void setTemperature(const Vital& vital) { setVital(temperature, vital); }
        void setHunger(const Vital& vital) { setVital(hunger, vital); }
        void setThirst(const Vital& vital) { setVital(thirst, vital); }
        void setToxicity(const Vital& vital) { setVital(toxicity, vital); }
        void setSanity(const Vital& vital) { setVital(sanity, vital); }
        
        /**
         * Serializes the CharacterVitals to a compact string enclosed by unique markers
         * for reliable extraction within a larger data stream.
         *
         * Format: [CV_BEGIN]current,max,min,...(for each Vital)[CV_END]
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
    
        Vital health;       ///< The character's current health.
        Vital mana;         ///< The character's current mana used for spellcasting.
        Vital energy;       ///< The character's energy used for physical actions.
        Vital rage;         ///< The character's rage, typically builds up during combat.
        Vital breath;       ///< The character's remaining breath for underwater or choking scenarios.
        Vital focus;        ///< The character's mental focus or concentration level.
        Vital radiation;    ///< The character's radiation exposure level.
        Vital temperature;  ///< The character's current body temperature.
        Vital hunger;       ///< The character's hunger level.
        Vital thirst;       ///< The character's thirst level.
        Vital toxicity;     ///< The character's level of poison or toxicity.
        Vital sanity;       ///< The character's current mental stability or sanity.

    }; // class CharacterVitals
} // namespace rpg
