/**
 * File: MIAConfig.hpp
 * Author: Antonius Torode
 * Created on: 05/22/2025
 * Description: This file controls finding and loading in the configuration file(s). This file replaced
 *     the Configurator from the old MIA project.
 */
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <memory>

// Included for various common-types.
#include "ConfigData.hpp"

namespace MIA_System
{   
    // Forward declaration of ConfigData class where the configuration data is stored.
    class ConfigData;
        
    /**
     * @class MIAConfig
     * @brief Manages loading configuration files into flexible key-value storage.
     *
     * This class provides a mechanism to load configuration data into a Data object,
     * which can store key-value pairs, raw lines, or other formats based on the ConfigType.
     * It uses the PIMPL idiom to hide implementation details and support extensibility.
     *
     * The MIAConfig acts as a generic container for configuration data, allowing future
     * expansion to support diverse formats while maintaining consistent access patterns.
     */
    class MIAConfig
    {
    public:
        
        /**
         * Main default constructor for the MIAConfig class.
         */
        MIAConfig();
        
        /**
         * Main constructor for the MIAConfig class which sets the config file and type.
         * After constructed, initialize() must be called to load the configuration file.
         * @note The file name can be the entire file path or just the name (in which case
         *       default file path locations will be used).
         * @param configFile[const std::string&] - The config file to use.
         * @param type[constants::ConfigType] - The type of configuration to load.
         * @param verboseMode[bool] - Enables verbose output.
         */
        MIAConfig(const std::string& configFile, 
                  constants::ConfigType type, 
                  bool verboseMode = false);

        /**
         * Main destructor for the MIAConfig class.
         */
        ~MIAConfig();
        
        /**
         * This will iniitalize this object by reading and loading the configuration file into the RawConfigVals map.
         * @param verboseMode[bool] - Enables verbose output.
         * @throws MIAException - Thrown if the config file name is not set or if the config file cannot be opened.
         */
        void initialize(bool verboseMode = false);
        
        /**
         * This will reloadthe configuration file by calling initialize. This is mainly just semantically different
         * so that the call makes sense in contexts where it is used.
         * @param verboseMode[bool] - Enables verbose output.
         * @throws MIAException If the underlying initialize() call fails.
         */
        void reload(bool verboseMode = false)
        { initialize(verboseMode); };
        
        /**
         * Sets the config file name and type for this object and reloads the configuration.
         * @note The file name can be the entire file path or just the name (in which case
         *       default file path locations will be used).
         * @param configFile[const std::string&] - The config file to use.
         * @param type[constants::ConfigType] - The type of configuration to load.
         * @param verboseMode[bool] - Enables verbose output.
         * @throws MIAException - If the underlying initialize() call fails.
         */
        void setConfigFileName(const std::string& configFile, 
                               constants::ConfigType type, 
                               bool verboseMode = false);
        
        /**
         * Gets the config file name for this object.
         * @return [std::string&] - Returns the configFileName value.
         */
         std::string getConfigFileName()
         { return configFileName; }
         
        /**
         * Gets the config file name for this object.
         * @return [std::string&] - Returns the configFileName value.
         */
         std::string getConfigFileFullPath()
         { return configFileFullPath; }       
         
        /**
         * Gets the current configuration type.
         * @return [constants::ConfigType] - The type of configuration data stored.
         */
        constants::ConfigType getConfigType() const;
         
        /**
         * Retrieves the configuration value associated with the given key as an int.
         * @note Only supported for constants::ConfigType::KEY_VALUE.
         * @param key[const std::string&] - The key name to look up in the configuration map.
         * @return [int] - The int value if the key exists and conversion succeeds; otherwise, returns 0.
         * @throws MIAException - Thrown if the key is not found in the configuration map.
         */
        int getInt(const std::string& key) const;

        /**
         * Retrieves the configuration value associated with the given key as a double.
         * @note Only supported for constants::ConfigType::KEY_VALUE.
         * @param key[const std::string&] - The key name to look up in the configuration map.
         * @return [double] - The double value if the key exists and conversion succeeds; otherwise, returns 0.0.
         * @throws MIAException - Thrown if the key is not found in the configuration map.
         */
        double getDouble(const std::string& key) const;

        /**
         * Retrieves the configuration value associated with the given key as a string.
         * @note Only supported for constants::ConfigType::KEY_VALUE.
         * @param key[const std::string&] - The key name to look up in the configuration map.
         * @return [std::string] - The string value if the key exists; otherwise, returns an empty string.
         * @throws MIAException - Thrown if the key is not found in the configuration map.
         */
        std::string getString(const std::string& key) const;

        /**
         * Retrieves the configuration value associated with the given key as a vector of strings,
         * splitting the stored string by the specified delimiter.
         * @note Only supported for constants::ConfigType::KEY_VALUE.
         * @param key[const std::string&] - The key name to look up in the configuration map.
         * @param delimiter[char delimiter] - The character used to split the string into vector elements.
         * @return [std::vector<std::string>] - A vector of strings parsed from the value; empty vector if key not found.
         * @throws MIAException - Thrown if the key is not found in the configuration map.
         */
        std::vector<std::string> getVector(const std::string& key, char delimiter) const;
        
        /**
         * Retrieves the configuration value associated with the given key as a bool.
         * @note Only supported for constants::ConfigType::KEY_VALUE.
         * @param key[const std::string&] - The key name to look up in the configuration map.
         * @return [bool] - The bool value if the key exists and conversion succeeds; otherwise, returns false.
         * @throws MIAException - Thrown if the key is not found in the configuration map.
         */
        bool getBool(const std::string& key) const;

        /**
         * Retrieves the configuration value associated with the given key as a vector of ints,
         * splitting the stored string by the specified delimiter.
         * @param key[const std::string&] - The key name to look up in the configuration map.
         * @param delimiter[char] - The character used to split the string into vector elements.
         * @return [std::vector<int>] - A vector of ints parsed from the value; empty vector if key not found.
         * @throws MIAException - Thrown if the key is not found in the configuration map.
         */
        std::vector<int> getIntVector(const std::string& key, char delimiter) const;
        
        /**
         * Dumps all key-value pairs stored in rawConfigValsMap to the specified output stream.
         * Useful for debugging to inspect the current configuration values.
         * @param os [std::ostream&] - The output stream to write the key-value pairs to (default: std::cout).
         */
        [[maybe_unused]]
        void dumpConfigMap(std::ostream& os = std::cout) const;
        
        /**
         * Returns all key-value pairs stored in rawConfigValsMap as a vector.
         * This can be used to iterate over all configuration entries.
         * @note Only supported for constants::ConfigType::KEY_VALUE.
         * @return std::vector<constants::KeyValuePair> - A vector containing all configuration key-value pairs.
         */
        std::vector<constants::KeyValuePair> getAllConfigPairs() const;        
        
        /**
         * Returns all raw lines stored in the Data object.
         * If the config type does not support raw lines, returns an empty vector.
         * @return std::vector<std::string> - A vector containing all raw lines.
         */
        std::vector<std::string> getRawLines() const;

    private:
    
        /// The name of the configuration file for this object.
        std::string configFileName;
        
        /// The full path of the configuration file for this object.
        std::string configFileFullPath;
        
        /// Stores the configuration data (PIMPL idiom).
        std::unique_ptr<ConfigData> configData;
    }; // class MIAConfig
} // namespace MIA_System
