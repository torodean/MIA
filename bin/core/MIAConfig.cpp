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
#include <limits> // Used for infinity()

// The associated header file(s).
#include "MIAConfig.hpp"
#include "KeyValueData.hpp"
#include "RawLinesData.hpp"
// Used for file path locations.
#include "Paths.hpp"
// Used foir some string manipulation
#include "BasicUtilities.hpp"
// Used for error codes and exceptions.
#include "Error.hpp"
#include "MIAException.hpp"


namespace config
{
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
        std::optional<bool> val = configData->getBool(key);;
        if (val.has_value())
            return val.value();

        if (!val.has_value() && !optional)
        { // This indicates the value was not found in the config but is required.
            MIA_THROW(error::ErrorCode::Config_Missing_Key, key);
        }

        return defaultVal;
    }


    int MIAConfig::getInt(const std::string& key, bool optional, int defaultVal) const
    {
        std::optional<int> val = configData->getInt(key);
        if (val.has_value())
            return val.value();

        if (!val.has_value() && !optional)
        { // This indicates the value was not found in the config but is required.
            MIA_THROW(error::ErrorCode::Config_Missing_Key, key);
        }

        return defaultVal;
    }


    double MIAConfig::getDouble(const std::string& key, bool optional, double defaultVal) const
    {
        std::optional<double> val = configData->getDouble(key);
        if (val.has_value())
            return val.value();

        if (!val.has_value() && !optional)
        { // This indicates the value was not found in the config but is required.
            MIA_THROW(error::ErrorCode::Config_Missing_Key, key);
        }

        return defaultVal;
    }


    std::string MIAConfig::getString(const std::string& key, 
                                     bool optional,
                                     const std::string& defaultVal) const
    {
        std::optional<std::string> val = configData->getString(key);
        if (val.has_value())
            return val.value();

        if (!val.has_value() && !optional)
        { // This indicates the value was not found in the config but is required.
            MIA_THROW(error::ErrorCode::Config_Missing_Key, key);
        }

        return defaultVal;
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

