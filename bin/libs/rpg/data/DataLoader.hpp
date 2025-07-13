/**
 * @file DataLoader.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A singleton class to manage loading of all game data registries from config files.
 */
#pragma once

#include <string>
#include <iostream>

namespace rpg
{
    /**
     * A singleton class to handle loading of all game data registries.
     */
    class DataLoader
    {
    public:
        // Delete copy constructor and assignment operator to enforce singleton
        DataLoader(const DataLoader&) = delete;
        DataLoader& operator=(const DataLoader&) = delete;

        /**
         * Gets the singleton instance of the DataLoader.
         * @return Reference to the singleton instance.
         */
        static DataLoader& getInstance();

        /**
         * Initializes all registries from their configuration files.
         * @param configDir The directory containing config files (e.g., "/home/awtorode/git/MIA/bin/libs/rpg/data/").
         * @return True if all registries loaded successfully, false otherwise.
         */
        bool initialize(const std::string& configDir);

        /**
         * Dumps the contents of all registries to the provided output stream.
         * @param os The output stream to write to (defaults to std::cout).
         */
        void dump(std::ostream& os = std::cout) const;

    private:
        DataLoader() = default;
    };
} // namespace rpg
