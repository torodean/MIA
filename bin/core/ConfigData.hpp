/**
 * @file ConfigData.hpp
 * @author Antonius Torode
 * @date 05/24/2025
 * @brief Private header defining the abstract ConfigData class for MIAConfig.
 *        This file is included only by implementation files, not by clients.
 */
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <optional>

// Used for the ConfigType
#include "Constants.hpp"

namespace config
{
    /**
     * @class ConfigData
     * @brief Abstract base class for storing and managing configuration data in MIAConfig.
     *
     * This class defines the interface for handling configuration data, supporting various
     * formats such as key-value pairs or raw lines. It is used internally by MIAConfig
     * as part of the PIMPL idiom to hide implementation details. Subclasses implement
     * specific storage and retrieval mechanisms based on the configuration type.
     *
     * All of the base-type returned values are set as std::optional so that callers can 
     * decide how they want to handle values which are not present (i.e. strictly require
     * or to allow some values to be optional). The vector values are not optional because
     * the caller can just check if the vector is empty for the same result.
     */
    class ConfigData
    {
    public:
        /**
         * @brief Virtual destructor to ensure proper cleanup in derived classes.
         */
        virtual ~ConfigData() = default;

        /**
         * @brief Retrieves the configuration value for a key as a boolean.
         * @param key The key to look up.
         * @return The boolean value of the configuration entry, or std::nullopt if the
         *         key is not present.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::optional<bool> getBool(const std::string& key) const = 0;

        /**
         * @brief Retrieves the configuration value for a key as an integer.
         * @param key The key to look up.
         * @return The integer value of the configuration entry, or std::nullopt if the
         *         key is not present.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::optional<int> getInt(const std::string& key) const = 0;

        /**
         * @brief Retrieves the configuration value for a key as a double.
         * @param key The key to look up.
         * @return The double value of the configuration entry, or std::nullopt if the
         *         key is not present.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::optional<double> getDouble(const std::string& key) const = 0;

        /**
         * @brief Retrieves the configuration value for a key as a string.
         * @param key The key to look up.
         * @return The string value of the configuration entry, or std::nullopt if the
         *         key is not present.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::optional<std::string> getString(const std::string& key) const = 0;

        /**
         * @brief Retrieves the configuration value for a key as a vector of strings, 
         *        split by a delimiter.
         * @param key The key to look up.
         * @param delimiter The character used to split the string value.
         * @return A vector of strings parsed from the configuration entry.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::vector<std::string> getVector(const std::string& key, 
                                                   char delimiter) const = 0;

        /**
         * @brief Retrieves the configuration value for a key as a vector of 
         *        integers, split by a delimiter.
         * @param key The key to look up.
         * @param delimiter The character used to split the string value.
         * @return A vector of integers parsed from the configuration entry.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::vector<int> getIntVector(const std::string& key, 
                                              char delimiter) const = 0;

        /**
         * @brief Retrieves all key-value pairs stored in the configuration.
         * @return A vector of all key-value pairs.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::vector<constants::KeyValuePair> getAllConfigPairs() const = 0;

        /**
         * @brief Retrieves all raw lines stored in the configuration.
         * @return A vector of all raw lines.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::vector<std::string> getRawLines() const = 0;

        /**
         * @brief Outputs the configuration data to the specified stream.
         * @param os The output stream to write to.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual void dump(std::ostream& os) const = 0;

        /**
         * @brief Loads configuration data from a file.
         * @param filePath The path to the configuration file.
         * @param verboseMode If true, enables verbose output during loading.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual void load(const std::string& filePath, bool verboseMode) = 0;

        /**
         * @brief Retrieves the type of configuration data stored.
         * @return The configuration type (e.g., KEY_VALUE, RAW_LINES).
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual constants::ConfigType getType() const = 0;
    };
} // namespace config
