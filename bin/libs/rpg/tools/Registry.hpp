/**
 * @file Registry.hpp
 * @author Antonius Torode
 * @date 07/08/2025
 * Description: A base class for managing registries of game objects in the MIA RPG system.
 */
#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

// Used for exception and error handling.
#include "MIAException.hpp"
#include "Error.hpp"

namespace rpg
{
    /**
     * Base class for registries managing game objects (e.g., Currency, Vital).
     * This uses the Curiously Recurring Template Pattern (CRTP) where:
     * - Derived is the registry subclass inheriting from this base.
     * - Type is the type of object stored in the registry.
     *
     * This pattern allows the base class to provide functionality (like a singleton instance)
     * for the derived class, while allowing derived classes to have type-specific behavior.
     *
     * @tparam Derived The registry class inheriting from this base.
     * @tparam Type The type of object stored in the registry.
     */
    template<typename Derived, typename Type>
    class Registry
    {
    public:
        // Delete copy constructor and assignment operator for singleton
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        /**
         * Gets the singleton instance of the derived registry class.
         * This ensures exactly one instance of the registry exists during runtime.
         * It uses the Curiously Recurring Template Pattern (CRTP) to instantiate the derived class.
         * 
         * @return Reference to the unique singleton instance of the derived registry.
         */
        static Derived& getInstance()
        {
            static Derived instance;
            return instance;
        }

        /**
         * Loads objects from a JSON configuration file.
         * @param filename Path to the JSON file.
         * @throws MIAException if file cannot be read or parsed.
         */
        virtual void loadFromFile(const std::string& filename)
        {
            std::ifstream file(filename);
            if (!file.is_open())
                MIA_THROW(error::Failed_To_Open_File, filename);

            nlohmann::json data;
            file >> data;
            loadFromJson(data);
        }
        
        /**
         * Loads objects from a JSON string.
         * Useful for testing or dynamic configuration without file I/O.
         *
         * @param jsonStr A JSON-formatted string.
         * @throws MIAException if parsing fails.
         */
        virtual void loadFromString(const std::string& jsonStr)
        {
            nlohmann::json data;
            try
            {
                data = nlohmann::json::parse(jsonStr);
            }
            catch (const nlohmann::json::parse_error& e)
            {
                MIA_THROW(error::JSON_Failed_To_Parse_String, std::string(e.what()));
            }
            loadFromJson(data);
        }

        /**
         * Gets an object by its ID.
         * @param id The unique ID.
         * @return Pointer to the object, or nullptr if not found.
         */
        const Type* getByID(uint32_t id) const
        {
            auto it = objects.find(id);
            return it != objects.end() ? &it->second : nullptr;
        }

        /**
         * Gets an object by its name.
         * @param name The name of the object.
         * @return Pointer to the object, or nullptr if not found.
         */
        const Type* getByName(const std::string& name) const
        {
            auto it = nameToId.find(name);
            return it != nameToId.end() ? getByID(it->second) : nullptr;
        }

        /**
         * Dumps all objects to the provided output stream.
         * @param os The output stream (defaults to std::cout).
         */
        virtual void dump(std::ostream& os = std::cout) const
        {
            for (const auto& [id, obj] : objects)
            {
                os << "ID: " << id << ", " << toString(obj) << "\n";
            }
        }        

    protected:
    
        /**
         * Default constructor. TODO - I forgot why this was protected.
         */
        Registry() = default;
        
        /**
         * Returns the JSON key for the derived class's data array.
         * Must be implemented by derived classes.
         * @return The JSON key (e.g., "currency", "vitals").
         */
        virtual std::string getJsonKey() const = 0;

        /**
         * Parses a JSON object into type Type. Must be implemented by derived classes.
         * @param json The JSON object.
         * @return The parsed object of type Type.
         */
        virtual Type parseJson(const nlohmann::json& json) = 0;

        /**
         * Converts an object to a string for dumping. Must be implemented by derived classes.
         * @param obj The object of type Type.
         * @return String representation of the object.
         */
        virtual std::string toString(const Type& obj) const = 0;
        
        /**
         * Loads objects from a JSON object by extracting the array for the derived class's key.
         * @param data The JSON object containing the array.
         * @throws MIAException if the key is missing or not an array.
         */
        void loadFromJson(const nlohmann::json& data)
        {
            std::string key = getJsonKey();
            if (!data.contains(key))
                MIA_THROW(error::JSON_Key_Not_Found, key);
            if (!data[key].is_array())
                MIA_THROW(error::JSON_Key_Not_Array_Type, key);

            objects.clear();
            nameToId.clear();
            for (const auto& item : data[key])
            {
                Type obj = parseJson(item);
                uint32_t id = item["id"].get<uint32_t>();
                objects[id] = std::move(obj);
                nameToId[item["name"].get<std::string>()] = id;
            }
        }

        std::unordered_map<uint32_t, Type> objects; ///< Map of ID to object.
        std::unordered_map<std::string, uint32_t> nameToId; ///< Map of name to ID.
    };
} // namespace rpg
