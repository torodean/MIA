/**
 * @file RawLinesData.hpp
 * @author Antonius Torode
 * @date 05/24/2025
 * Description: Private header defining the RawLinesData class for MIAConfig.
 *              This class implements the ConfigData interface to store configuration
 *              data as a vector of raw file lines, supporting raw line access but not
 *              key-value operations.
 */
#pragma once

#include <iostream>
#include <vector>
#include <string>

// Included for the abstract base class.
#include "ConfigData.hpp"

namespace config
{
    /**
     * @class RawLinesData
     * @brief Concrete implementation of ConfigData for storing raw configuration file lines.
     *
     * This class implements the ConfigData interface to manage configuration data as a
     * vector of raw lines read from a file. It supports loading and accessing raw lines
     * but throws exceptions for key-value access methods, as they are unsupported for
     * the RAW_LINES configuration type. See ConfigData for the general interface contract.
     */
    class RawLinesData : public ConfigData
    {
    public:
        /**
         * @brief Loads configuration data from a file as raw lines.
         * @param filePath[const std::string&] - The path to the configuration file.
         * @param verboseMode[bool] - If true, enables verbose output during loading.
         * @throws MIAException If the file cannot be opened or read.
         * @see ConfigData::load()
         */
        void load(const std::string& filePath, bool verboseMode) override;

        /**
         * @name Unsupported Key-Value Access Methods
         * @brief Methods for key-value access that are unsupported in RawLinesData.
         *
         * The following methods are not implemented for the RAW_LINES configuration type,
         * as they require key-value pair storage. Each method throws an MIAException to
         * indicate that key-value access is not supported. See ConfigData for the interface
         * contract of each method.
         * @{
         */
        int getInt(const std::string& key) const override;
        double getDouble(const std::string& key) const override;
        std::string getString(const std::string& key) const override;
        std::vector<std::string> getVector(const std::string& key, char delimiter) const override;
        bool getBool(const std::string& key) const override;
        std::vector<int> getIntVector(const std::string& key, char delimiter) const override;
        std::vector<constants::KeyValuePair> getAllConfigPairs() const override;
        /** @} */

        /**
         * @brief Retrieves all raw lines stored in the configuration.
         * @return [std::vector<std::string>] - A vector containing all raw lines.
         * @see ConfigData::getRawLines()
         */
        std::vector<std::string> getRawLines() const override;

        /**
         * @brief Outputs the configuration data to the specified stream.
         * @param os[std::ostream&] - The output stream to write to.
         * @note Outputs all raw lines stored in the configuration.
         * @see ConfigData::dump()
         */
        void dump(std::ostream& os) const override;

        /**
         * @brief Retrieves the type of configuration data stored.
         * @return [constants::ConfigType] - Always returns constants::ConfigType::RAW_LINES.
         * @see ConfigData::getType()
         */
        constants::ConfigType getType() const override;

    private:
        /**
         * @brief Stores the raw lines read from the configuration file.
         */
        std::vector<std::string> rawConfigLines;
    };
} // namespace config
