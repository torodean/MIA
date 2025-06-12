/**
 * File: ConfigData.hpp
 * Author: Antonius Torode
 * Created on: 05/24/2025
 * Description: Private header defining the abstract ConfigData class for MIAConfig.
 *              This file is included only by implementation files, not by clients.
 */
#pragma once

#include <string>
#include <vector>
#include <iostream>

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
     */
    class ConfigData
    {
    public:
        /**
         * @brief Virtual destructor to ensure proper cleanup in derived classes.
         */
        virtual ~ConfigData() = default;
        
        /**
         * @brief Retrieves the configuration value for a key as an integer.
         * @param key[const std::string&] - The key to look up.
         * @return [int] - The integer value of the configuration entry.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual int getInt(const std::string& key) const = 0;

        /**
         * @brief Retrieves the configuration value for a key as a double.
         * @param key[const std::string&] - The key to look up.
         * @return [double] - The double value of the configuration entry.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual double getDouble(const std::string& key) const = 0;

        /**
         * @brief Retrieves the configuration value for a key as a string.
         * @param key[const std::string&] - The key to look up.
         * @return [std::string] - The string value of the configuration entry.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::string getString(const std::string& key) const = 0;

        /**
         * @brief Retrieves the configuration value for a key as a vector of strings, split by a delimiter.
         * @param key[const std::string&] - The key to look up.
         * @param delimiter[char] - The character used to split the string value.
         * @return [std::vector<std::string>] - A vector of strings parsed from the configuration entry.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::vector<std::string> getVector(const std::string& key, char delimiter) const = 0;

        /**
         * @brief Retrieves the configuration value for a key as a boolean.
         * @param key[const std::string&] - The key to look up.
         * @return [bool] - The boolean value of the configuration entry.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual bool getBool(const std::string& key) const = 0;

        /**
         * @brief Retrieves the configuration value for a key as a vector of integers, split by a delimiter.
         * @param key[const std::string&] - The key to look up.
         * @param delimiter[char] - The character used to split the string value.
         * @return [std::vector<int>] - A vector of integers parsed from the configuration entry.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::vector<int> getIntVector(const std::string& key, char delimiter) const = 0;

        /**
         * @brief Retrieves all key-value pairs stored in the configuration.
         * @return [std::vector<constants::KeyValuePair>] - A vector of all key-value pairs.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::vector<constants::KeyValuePair> getAllConfigPairs() const = 0;

        /**
         * @brief Retrieves all raw lines stored in the configuration.
         * @return [std::vector<std::string>] - A vector of all raw lines.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual std::vector<std::string> getRawLines() const = 0;

        /**
         * @brief Outputs the configuration data to the specified stream.
         * @param os[std::ostream&] - The output stream to write to.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual void dump(std::ostream& os) const = 0;

        /**
         * @brief Loads configuration data from a file.
         * @param filePath[const std::string&] - The path to the configuration file.
         * @param verboseMode[bool] - If true, enables verbose output during loading.
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual void load(const std::string& filePath, bool verboseMode) = 0;

        /**
         * @brief Retrieves the type of configuration data stored.
         * @return [constants::ConfigType] - The configuration type (e.g., KEY_VALUE, RAW_LINES).
         * @note Pure virtual; must be implemented by derived classes.
         */
        virtual constants::ConfigType getType() const = 0;
    };
} // namespace config
