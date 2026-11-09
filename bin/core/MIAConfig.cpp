/**
 * @file MIAConfig.cpp
 * @author Antonius Torode
 * @date 05/22/2025
 * Description: Implementation of the MIAConfig class that manages loading and parsing
 *              of configuration files into flexible key-value storage.
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <optional>
#include <limits> // Used for infinity()

// The associated header file(s).
#include "MIAConfig.hpp"
#include "KeyValueData.hpp"
#include "RawLinesData.hpp"
// Used for file path locations.
#include "Paths.hpp"
// Used for some string manipulation
#include "BasicUtilities.hpp"
// Used for error codes and exceptions.
#include "Error.hpp"
#include "MIAException.hpp"


namespace config
{
    namespace
    {
        /**
         * @brief Resolves an optional configuration value by returning the found value or the
         *        default value, or by throwing if the missing value is required.
         * @tparam Type The value type returned by the configuration lookup.
         * @param key The key that was looked up; used for the exception message when required.
         * @param val The optional value returned by the underlying ConfigData lookup.
         * @param optional Whether or not this config value is optional.
         * @param defaultVal The value to return if the value was not found and is optional.
         * @return The found value; otherwise defaultVal if optional is true.
         * @throws MIAException if the value was not found and is required.
         */
        template <typename Type>
        Type resolveOrDefault(const std::string& key,
                              const std::optional<Type>& val,
                              bool optional,
                              const Type& defaultVal)
        {
            if (val.has_value())
                return val.value();

            if (!optional)
            { // This indicates the value was not found in the config but is required.
                MIA_THROW(error::ErrorCode::Config_Missing_Key, key);
            }

            return defaultVal;
        }
    } // namespace


    // default to RawLinesData since it is perfectly valid for any type of file.
    MIAConfig::MIAConfig() : configData(std::make_unique<RawLinesData>()) {}


    MIAConfig::MIAConfig(const std::string& configFile, constants::ConfigType type, bool verboseMode)
        : configFileName(configFile)
    {
        switch (type) 
        {
            case constants::ConfigType::KEY_VALUE:
                configData = std::make_unique<KeyValueData>();
                break;
            case constants::ConfigType::RAW_LINES:
                configData = std::make_unique<RawLinesData>();
                break;
        }
    }


    MIAConfig::~MIAConfig() = default;


    void MIAConfig::setConfigFileName(const std::string& configFile, constants::ConfigType type, bool verboseMode)
    {
        configFileName = configFile;
        switch (type) 
        {
            case constants::ConfigType::KEY_VALUE:
                configData = std::make_unique<KeyValueData>();
                break;
            case constants::ConfigType::RAW_LINES:
                configData = std::make_unique<RawLinesData>();
                break;
        }
        reload(verboseMode);
    }


    void MIAConfig::initialize(bool verboseMode)
    {
        // Check if the configuration file has been set.
        if (configFileName.empty()) 
        { 
            std::string err = "No config file set in MIAConfig::initialize(..).";
            throw error::MIAException(error::ErrorCode::Config_File_Not_Set, err);
        }

        // Get the full path of the configuration file.
        if (configFileName.front() == '/') 
        {
            configFileFullPath = configFileName; // Assume absolute path.
        } 
        else 
        {
            configFileFullPath = paths::getDefaultConfigDirToUse() + "/" + configFileName;
        }

        configData->load(configFileFullPath, verboseMode);
    }


    constants::ConfigType MIAConfig::getConfigType() const
    {
        return configData->getType();
    }


    bool MIAConfig::getBool(const std::string& key, bool optional, bool defaultVal) const
    {
        return resolveOrDefault(key, configData->getBool(key), optional, defaultVal);
    }


    int MIAConfig::getInt(const std::string& key, bool optional, int defaultVal) const
    {
        return resolveOrDefault(key, configData->getInt(key), optional, defaultVal);
    }


    double MIAConfig::getDouble(const std::string& key, bool optional, double defaultVal) const
    {
        return resolveOrDefault(key, configData->getDouble(key), optional, defaultVal);
    }


    std::string MIAConfig::getString(const std::string& key, 
                                     bool optional,
                                     const std::string& defaultVal) const
    {
        return resolveOrDefault(key, configData->getString(key), optional, defaultVal);
    }


    std::vector<std::string> MIAConfig::getVector(const std::string& key, char delimiter) const
    {
        return configData->getVector(key, delimiter);
    }


    std::vector<int> MIAConfig::getIntVector(const std::string& key, char delimiter) const
    {
        return configData->getIntVector(key, delimiter);
    }


    std::vector<constants::KeyValuePair> MIAConfig::getAllConfigPairs() const
    {
        return configData->getAllConfigPairs();
    }


    std::vector<std::string> MIAConfig::getRawLines() const
    {
        return configData->getRawLines();
    }


    void MIAConfig::dumpConfigMap(std::ostream& os) const
    {
        configData->dump(os);
    }    
} // namespace config
