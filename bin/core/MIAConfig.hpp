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

namespace MIA_System
{
    /**
     * @class MIAConfig
     * @brief Manages loading configuration files into flexible key-value storage.
     *
     * This class provides a basic mechanism to load configuration data into a simple key-value map
     * (BasicConfigMap). It is designed with extensibility in mind: additional typedefs
     * like `SpecialConfigMap` or other data structures can be introduced later to support
     * more complex or alternative config formats without changing the core interface.
     *
     * The MIAConfig acts as a generic container for configuration data, allowing future
     * expansion to support diverse formats while maintaining consistent access patterns.
     */
    class MIAConfig
    {
    public:
        /**
         * The config map type for storing raw key/value pairs.
         * Key/value pairs in the configuration files are expected to be separated by '='.
         * This delimiter is used when parsing the configuration file into the BasicConfigMap.
         */
        using BasicConfigMap = std::unordered_map<std::string, std::string>;
        
        /**
         * Alias for a key-value pair representing a single configuration entry.
         * Represents a key-value pair from the configuration,
         * where both key and value are strings.
         */
        using KeyValuePair = std::pair<std::string, std::string>;
        
        /**
         * Main default constructor for the MIAConfig class.
         */
        MIAConfig() = default;
        
        /**
         * Main constructor for the MIAConfig class which sets the config file. After constructed, initialize()
         * will need called to load the actual configuration file.
         * @note The file name here can be the entire file path OR just the name of the file (in which case the
         *       default file path locations will be used).
         * @param configFile[const std::string&] - The config file to use for this object.
         * @param verboseMode[bool] - Enables verbose output.
         */
        MIAConfig(const std::string& configFile, bool verboseMode = false) :
            configFileName(configFile)
        {  };

        /**
         * Main destructor for the MIAConfig class.
         */
        ~MIAConfig() = default;
        
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
         * Sets the config file name for this object. After this is set, the initialize() method should be called.
         * This will also automatically call the reload() since it should be assumed that the config file set is
         * likely different than the initial one.
         * @note The file name here can be the entire file path OR just the name of the file (in which case the
         *       default file path locations will be used).
         * @param configFile[const std::string&] - The config file to use for this object.
         * @param verboseMode[bool] - Enables verbose output.
         * @throws MIAException If the underlying initialize() call fails.
         */
         void setConfigFileName(const std::string& configFile, bool verboseMode = false)
         { 
            configFileName = configFile; 
            reload(verboseMode); // may throw due to initialize()
        }
        
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
         * Retrieves the configuration value associated with the given key as an int.
         * @param key[const std::string&] - The key name to look up in the configuration map.
         * @return [int] - The int value if the key exists and conversion succeeds; otherwise, returns 0.
         * @throws MIAException - Thrown if the key is not found in the configuration map.
         */
        int getInt(const std::string& key) const;

        /**
         * Retrieves the configuration value associated with the given key as a double.
         * @param key[const std::string&] - The key name to look up in the configuration map.
         * @return [double] - The double value if the key exists and conversion succeeds; otherwise, returns 0.0.
         * @throws MIAException - Thrown if the key is not found in the configuration map.
         */
        double getDouble(const std::string& key) const;

        /**
         * Retrieves the configuration value associated with the given key as a string.
         * @param key[const std::string&] - The key name to look up in the configuration map.
         * @return [std::string] - The string value if the key exists; otherwise, returns an empty string.
         * @throws MIAException - Thrown if the key is not found in the configuration map.
         */
        std::string getString(const std::string& key) const;

        /**
         * Retrieves the configuration value associated with the given key as a vector of strings,
         * splitting the stored string by the specified delimiter.
         * @param key[const std::string&] - The key name to look up in the configuration map.
         * @param delimiter[char delimiter] - The character used to split the string into vector elements.
         * @return [std::vector<std::string>] - A vector of strings parsed from the value; empty vector if key not found.
         * @throws MIAException - Thrown if the key is not found in the configuration map.
         */
        std::vector<std::string> getVector(const std::string& key, char delimiter) const;
        
        /**
         * Retrieves the configuration value associated with the given key as a bool.
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
         * @return std::vector<KeyValuePair> - A vector containing all configuration key-value pairs.
         */
        std::vector<KeyValuePair> getAllConfigPairs() const;

    private:
    
        /// The name of the configuration file for this object.
        std::string configFileName;
        
        /// The full path of the configuration file for this object.
        std::string configFileFullPath;
        
        /// Stores the configuration values from a file which are key/value pairs.
        BasicConfigMap rawConfigValsMap;
    }; // class MIAConfig
} // namespace MIA_System
