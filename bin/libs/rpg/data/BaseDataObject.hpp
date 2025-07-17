/**
 * @file BaseDataObject.hpp
 * @author Antonius Torode
 * @date 07/17/2025
 * Description: Base data object class for rpg data classes.
 */
#pragma once

#include <string>

namespace rpg
{
    /**
     * Base data object. 
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
              const std::string& description) :
            id(id),
            name(name),
            description(description)
        { }

        /// Getters for the various data mambers.
        uint32_t getID() const { return id; }
        std::string getName() const { return name; }
        std::string getDescription() const { return description; }

    protected:
    
        /**
         * Serializes the BaseDataObject object to JSON.
         *
         * @return A JSON object containing the BaseDataObject's properties.
         */
        virtual nlohmann::json toJson() const
        {
            return
            {
                {"id", id},
                {"name", name},
                {"description", description}
            };
        }
        
        /**
         * Deserializes a BaseDataObject object from JSON.
         *
         * @param json The JSON object containing BaseDataObject properties.
         * @return The constructed BaseDataObject object.
         */
        static BaseDataObject fromJson(const nlohmann::json& json)
        {
            auto id = json.at("id").get<uint32_t>(); // use at() to require id
            auto name = json.at("name").get<std::string>();
            auto description = json.value("description", "");

            BaseDataObject baseDataObj(id, name, description);
            return baseDataObj;
        }

    private:
    
        uint32_t id{};            ///< Unique identifier for this BaseDataObject.
        std::string name;         ///< Name of the BaseDataObject.
        std::string description;  ///< Description of the BaseDataObject.
    };
} // namespace rpg

