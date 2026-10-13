/**
 * @file BaseDataObject.hpp
 * @author Antonius Torode
 * @date 07/17/2025
 * @brief Base data object class for rpg data classes.
 */
#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace rpg
{
    /**
     * Base data object. This data objects represents a piece of data from
     * the game-defined configuration. This does not represent a dynamic
     * object which changes during gameplay - although closely related.
     */
    class BaseDataObject
    {
    public:
        /// Default constructor.
        BaseDataObject() = default;

        /**
         * Constructs a BaseDataObject with specified properties.
         *
         * @param id Unique identifier for the BaseDataObject.
         * @param name Human-readable name of the BaseDataObject.
         * @param description A brief description of the BaseDataObject.
         */
        BaseDataObject(uint32_t id,
                       const std::string& name,
                       const std::string& description);

        /// Getters for the various data members.
        uint32_t getID() const;
        std::string getName() const;
        std::string getDescription() const;

    protected:
    
        /**
         * Serializes the BaseDataObject object to JSON.
         *
         * @return A JSON object containing the BaseDataObject's properties.
         */
        virtual nlohmann::json toJson() const;
        
        /**
         * Deserializes a BaseDataObject object from JSON.
         *
         * @param json The JSON object containing BaseDataObject properties.
         * @return The constructed BaseDataObject object.
         */
        static BaseDataObject fromJson(const nlohmann::json& json);

    private:
    
        uint32_t id{};             ///< Unique identifier for this BaseDataObject.
        std::string name{};        ///< Name of the BaseDataObject.
        std::string description{}; ///< Description of the BaseDataObject.
    };
} // namespace rpg

