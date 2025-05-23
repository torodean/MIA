/**
 * File: MIAConfig.cpp
 * Author: Antonius Torode
 * Created on: 05/22/2025
 * Description: Implementation of the MIAConfig class that manages loading and parsing
 *              of configuration files into flexible key-value storage.
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <limits> // Used for infinity()

// The associated header file.
#include "MIAConfig.hpp"
// Used for file path locations.
#include "Paths.hpp"
// Used foir some string manipulation
#include "BasicUtilities.hpp"
// Used for error codes and exceptions.
#include "Error.hpp"
#include "MIAException.hpp"


namespace MIA_System
{
    void MIAConfig::initialize(bool verboseMode)
    {
        rawConfigValsMap.clear();

        if (configFileName.empty()) 
        { 
            std::string err = "No config file set in MIAConfig::initialize(..).";
            throw error::MIAException(error::ErrorCode::Config_File_Not_Set, err);
        }

        std::string fullPath;
        if (configFileName.front() == '/') 
        {
            fullPath = configFileName; // Assume absolute path.
        } 
        else 
        {
            if (paths::isInstalled())
                fullPath = std::string(paths::SYSTEM_CONFIG_FILE_DIR) + "/" + configFileName;
            else                
                fullPath = std::string(paths::REPO_CONFIG_FILE_DIR) + "/" + configFileName;
                
            configFileFullPath = fullPath;
        }

        std::ifstream file(fullPath, std::ifstream::in);
        if (!file.is_open()) 
        {            
            std::string err = "Failed to open config file: " + fullPath;
            throw error::MIAException(error::ErrorCode::Failed_To_Open_File, err);
        }

        std::string line;
        std::vector<std::string> lines;
        
        while (std::getline(file, line)) 
        {            
            if (!line.empty() && line[0] != '#' && line.size()>2)
            {
                if(verboseMode) 
					std::cout << line << std::endl;
                lines.push_back(line);
            }
        }
        if(verboseMode) 
			std::cout << std::endl;

        file.close();
        
        int size = lines.size();
        int equalSignLocation;        
        std::string variable, value;
        for (int i=0; i<size;i++)
        {
            equalSignLocation = BasicUtilities::findCharInString(lines[i], '=');
            if (equalSignLocation <= 0 || equalSignLocation >= lines[i].size() - 1)
                continue; // or log malformed line
                
            variable = lines[i].substr(0, equalSignLocation);
            value = lines[i].substr(equalSignLocation+1,lines[i].size()-1);

            //removes end of line characters from variable name and value. Fixes a bug.
            variable.erase(remove(variable.begin(), variable.end(), '\r'), variable.end());
            value.erase(remove(value.begin(), value.end(), '\r'), value.end());

            rawConfigValsMap[variable] = value;
        }
    }
    

    int MIAConfig::getInt(const std::string& key) const
    {
        auto it = rawConfigValsMap.find(key);
        if (it == rawConfigValsMap.end())
        {
            std::string err = "Can't find key in configuration map: " + key;
            throw error::MIAException(error::ErrorCode::Cannot_Find_Mapped_Value, err);
        }
        try 
        {
            return std::stoi(it->second);
        } 
        catch (...) 
        {
            // TODO - Add MIAException here.
            return 0;
        }
    }


    double MIAConfig::getDouble(const std::string& key) const
    {
        auto it = rawConfigValsMap.find(key);
        if (it == rawConfigValsMap.end())
        {
            std::string err = "Can't find key in configuration map: " + key;
            throw error::MIAException(error::ErrorCode::Cannot_Find_Mapped_Value, err);
        }
        try 
        {
            if (it->second == "inf" || it->second == "infinity")
                return std::numeric_limits<double>::infinity();
            else
                return std::stod(it->second);
        } 
        catch (...) 
        {
            // TODO - Add MIAException here.
            return 0.0;
        }
    }


    std::string MIAConfig::getString(const std::string& key) const
    {
        auto it = rawConfigValsMap.find(key);
        if (it == rawConfigValsMap.end())
        {
            std::string err = "Can't find key in configuration map: " + key;
            throw error::MIAException(error::ErrorCode::Cannot_Find_Mapped_Value, err);
        }
            
        return it->second;
    }


    std::vector<std::string> MIAConfig::getVector(const std::string& key, char delimiter) const
    {
        std::vector<std::string> result;
        auto it = rawConfigValsMap.find(key);
        if (it == rawConfigValsMap.end())
        {
            std::string err = "Can't find key in configuration map: " + key;
            throw error::MIAException(error::ErrorCode::Cannot_Find_Mapped_Value, err);
        }

        std::string val = it->second;
        std::stringstream ss(val);
        std::string item;
        while (std::getline(ss, item, delimiter)) 
        {
            result.push_back(item);
        }
        
        return result;
    }


    bool MIAConfig::getBool(const std::string& key) const
    {
        auto it = rawConfigValsMap.find(key);
        if (it == rawConfigValsMap.end())
        {
            std::string err = "Can't find key in configuration map: " + key;
            throw error::MIAException(error::ErrorCode::Cannot_Find_Mapped_Value, err);
        }
            
        std::string val = it->second;
        std::transform(val.begin(), val.end(), val.begin(), ::tolower);
        
        // Check for different forms of true.
        return (val == "true" || val == "1" || val == "yes" || val == "on");
    }


    std::vector<int> MIAConfig::getIntVector(const std::string& key, char delimiter) const
    {
        std::vector<int> result;
        auto it = rawConfigValsMap.find(key);
        if (it == rawConfigValsMap.end())
        {
            std::string err = "Can't find key in configuration map: " + key;
            throw error::MIAException(error::ErrorCode::Cannot_Find_Mapped_Value, err);
        }

        std::string val = it->second;
        std::stringstream ss(val);
        std::string item;
        while (std::getline(ss, item, delimiter)) 
        {
            try 
            {
                result.push_back(std::stoi(item));
            } 
            catch (...) 
            {
                // TODO - Add MIAException here.
                // Skip invalid integers
            }
        }
        return result;
    }
} // namespace MIA_System

