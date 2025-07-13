/**
 * @file Registry.hpp
 * @author Antonius Torode
 * @date 07/08/2025
 * Description: A templated base class for managing registries of game objects in the MIA RPG system.
 */
#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

namespace rpg
{
    /**
     * Templated base class for registries managing game objects (e.g., Currency, Vital).
     * Uses the Curiously Recurring Template Pattern (CRTP) where:
     * - Derived is the concrete registry subclass inheriting from this base.
     * - T is the type of object stored in the registry.
     *
     * This pattern enables the base class to provide functionality (like a singleton instance)
     * for the derived class, while allowing derived classes to implement type-specific behavior.
     *
     * @tparam Derived The concrete registry class inheriting from this base.
     * @tparam T The type of object stored in the registry.
     */
    template<typename Derived, typename T>
    class Registry
    {
    public:
        // Delete copy constructor and assignment operator for singleton
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        /**
         * Gets the singleton instance of the derived registry class.
         * This method ensures only one instance of the registry exists during runtime.
         * It uses the Curiously Recurring Template Pattern (CRTP) to instantiate the concrete derived class.
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
         * @throws std::runtime_error if file cannot be read or parsed.
         */
        virtual void loadFromFile(const std::string& filename)
        {
            std::ifstream file(filename);
            if (!file.is_open())
                throw std::runtime_error("Failed to open file: " + filename);

            nlohmann::json data;
            file >> data;
            loadFromJson(data);
        }
        
        /**
         * Loads objects from a JSON string.
         * Useful for testing or dynamic configuration without file I/O.
         *
         * @param jsonStr A JSON-formatted string.
         * @throws std::runtime_error if parsing fails.
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
                throw std::runtime_error("Failed to parse JSON string: " + std::string(e.what()));
            }
            loadFromJson(data);
        }

        /**
         * Gets an object by its ID.
         * @param id The unique ID.
         * @return Pointer to the object, or nullptr if not found.
         */
        const T* getById(uint32_t id) const
        {
            auto it = objects.find(id);
            return it != objects.end() ? &it->second : nullptr;
        }

        /**
         * Gets an object by its name.
         * @param name The name of the object.
         * @return Pointer to the object, or nullptr if not found.
         */
        const T* getByName(const std::string& name) const
        {
            auto it = nameToId.find(name);
            return it != nameToId.end() ? getById(it->second) : nullptr;
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
        Registry() = default;
        
        /**
         * Returns the JSON key for the derived class's data array.
         * Must be implemented by derived classes.
         * @return The JSON key (e.g., "currency", "vitals").
         */
        virtual std::string getJsonKey() const = 0;

        /**
         * Parses a JSON object into type T. Must be implemented by derived classes.
         * @param json The JSON object.
         * @return The parsed object of type T.
         */
        virtual T parseJson(const nlohmann::json& json) = 0;

        /**
         * Converts an object to a string for dumping. Must be implemented by derived classes.
         * @param obj The object of type T.
         * @return String representation of the object.
         */
        virtual std::string toString(const T& obj) const = 0;
        
        /**
         * Loads objects from a JSON object by extracting the array for the derived class's key.
         * @param data The JSON object containing the array.
         * @throws std::runtime_error if the key is missing or not an array.
         */
        void loadFromJson(const nlohmann::json& data)
        {
            std::string key = getJsonKey();
            if (!data.contains(key))
                throw std::runtime_error("JSON does not contain key: " + key);
            if (!data[key].is_array())
                throw std::runtime_error("JSON key '" + key + "' is not an array");

            objects.clear();
            nameToId.clear();
            for (const auto& item : data[key])
            {
                T obj = parseJson(item);
                uint32_t id = item["id"].get<uint32_t>();
                objects[id] = std::move(obj);
                nameToId[item["name"].get<std::string>()] = id;
            }
        }

        std::unordered_map<uint32_t, T> objects; ///< Map of ID to object.
        std::unordered_map<std::string, uint32_t> nameToId; ///< Map of name to ID.
    };
} // namespace rpg
