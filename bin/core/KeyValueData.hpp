/**
 * File: KeyValueData.hpp
 * Author: Antonius Torode
 * Created on: 05/24/2025
 * Description: Private header defining the KeyValueData class for MIAConfig.
 *              This class implements the ConfigData interface to store configuration
 *              data as key-value pairs, supporting key-based access and conversion.
 */
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

// Included for the abstract base class.
#include "ConfigData.hpp"

namespace config
{
    /**
     * @class KeyValueData
     * @brief Concrete implementation of ConfigData for storing key-value pair configurations.
     *
     * This class implements the ConfigData interface to manage configuration data as
     * key-value pairs parsed from a file, with keys and values separated by '='. It
     * supports key-based access, type conversion, and constructing raw lines from pairs.
     * See ConfigData for the general interface contract.
     */
    class KeyValueData : public ConfigData
    {
    public:
        /**
         * @brief Type alias for the map storing raw key-value pairs.
         *
         * Key-value pairs in the configuration file are expected to be separated by '='.
         * This delimiter is used when parsing the file into the map.
         */
        using BasicConfigMap = std::unordered_map<std::string, std::string>;

        /**
         * @brief Loads configuration data from a file as key-value pairs.
         * @param filePath[const std::string&] - The path to the configuration file.
         * @param verboseMode[bool] - If true, enables verbose output during loading.
         * @throws MIAException If the file cannot be opened or parsed.
         * @see ConfigData::load()
         */
        void load(const std::string& filePath, bool verboseMode) override;

        /**
         * @brief Retrieves the configuration value for a key as an integer.
         * @param key[const std::string&] - The key to look up.
         * @return [int] - The integer value if the key exists and conversion succeeds; otherwise, 0.
         * @throws MIAException If the key is not found or conversion fails.
         * @see ConfigData::getInt()
         */
        int getInt(const std::string& key) const override;

        /**
         * @brief Retrieves the configuration value for a key as a double.
         * @param key[const std::string&] - The key to look up.
         * @return [double] - The double value if the key exists and conversion succeeds; otherwise, 0.0.
         * @throws MIAException If the key is not found or conversion fails.
         * @see ConfigData::getDouble()
         */
        double getDouble(const std::string& key) const override;

        /**
         * @brief Retrieves the configuration value for a key as a string.
         * @param key[const std::string&] - The key to look up.
         * @return [std::string] - The string value if the key exists; otherwise, an empty string.
         * @throws MIAException If the key is not found.
         * @see ConfigData::getString()
         */
        std::string getString(const std::string& key) const override;

        /**
         * @brief Retrieves the configuration value for a key as a vector of strings.
         * @param key[const std::string&] - The key to look up.
         * @param delimiter[char] - The character used to split the string value.
         * @return [std::vector<std::string>] - A vector of strings parsed from the value; empty if key not found or conversion fails.
         * @throws MIAException If the key is not found.
         * @see ConfigData::getVector()
         */
        std::vector<std::string> getVector(const std::string& key, char delimiter) const override;

        /**
         * @brief Retrieves the configuration value for a key as a boolean.
         * @param key[const std::string&] - The key to look up.
         * @return [bool] - The boolean value if the key exists and conversion succeeds; otherwise, false.
         * @throws MIAException If the key is not found or conversion fails.
         * @see ConfigData::getBool()
         */
        bool getBool(const std::string& key) const override;

        /**
         * @brief Retrieves the configuration value for a key as a vector of integers.
         * @param key[const std::string&] - The key to look up.
         * @param delimiter[char] - The character used to split the string value.
         * @return [std::vector<int>] - A vector of integers parsed from the value; empty if key not found or conversion fails.
         * @throws MIAException If the key is not found or conversion fails.
         * @see ConfigData::getIntVector()
         */
        std::vector<int> getIntVector(const std::string& key, char delimiter) const override;

        /**
         * @brief Retrieves all key-value pairs stored in the configuration.
         * @return [std::vector<constants::KeyValuePair>] - A vector containing all key-value pairs.
         * @see ConfigData::getAllConfigPairs()
         */
        std::vector<constants::KeyValuePair> getAllConfigPairs() const override;

        /**
         * @brief Retrieves raw lines constructed from key-value pairs.
         * @return [std::vector<std::string>] - A vector of strings, each representing a key-value pair as a line.
         * @note Constructs lines in the format "key=value" from stored pairs.
         * @see ConfigData::getRawLines()
         */
        std::vector<std::string> getRawLines() const override;

        /**
         * @brief Outputs the configuration data to the specified stream.
         * @param os[std::ostream&] - The output stream to write to.
         * @return [void] - No return value; outputs all key-value pairs.
         * @note Outputs each key-value pair in the format "key=value".
         * @see ConfigData::dump()
         */
        void dump(std::ostream& os) const override;

        /**
         * @brief Retrieves the type of configuration data stored.
         * @return [constants::ConfigType] - Always returns constants::ConfigType::KEY_VALUE.
         * @see ConfigData::getType()
         */
        constants::ConfigType getType() const override;

    private:
        /**
         * @brief Stores the key-value pairs parsed from the configuration file.
         */
        BasicConfigMap rawConfigValsMap;
    };
} // namespace config
