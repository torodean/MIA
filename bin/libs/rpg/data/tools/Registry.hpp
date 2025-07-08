/**
 * File: Registry.hpp
 * Author: Antonius Torode
 * Created on: 07/08/2025
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
     * @tparam T The type of object stored in the registry.
     */
    template<typename T>
    class Registry
    {
    public:
        // Delete copy constructor and assignment operator for singleton
        Registry(const Registry&) = delete;
        Registry& operator=(const Registry&) = delete;

        /**
         * Gets the singleton instance of the registry.
         * @return Reference to the singleton instance.
         */
        static Registry<T>& getInstance()
        {
            static Registry<T> instance;
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
            objects.clear();
            nameToId.clear();
            for (const auto& item : data)
            {
                T obj = parseJson(item);
                uint32_t id = item["id"];
                objects[id] = std::move(obj);
                nameToId[item["name"]] = id;
            }
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

        std::unordered_map<uint32_t, T> objects; ///< Map of ID to object.
        std::unordered_map<std::string, uint32_t> nameToId; ///< Map of name to ID.
    };
} // namespace rpg
