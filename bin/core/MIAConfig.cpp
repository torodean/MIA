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

// The associated header file.
#include "MIAConfig.hpp"

namespace MIA_System
{
    void MIAConfig::initialize()
    {
        // TODO: Implement loading and parsing of the configuration file
    }
    

    int MIAConfig::getInt(const std::string& key) const
    {
        auto it = RawConfigVals.find(key);
        if (it == RawConfigVals.end()) 
            return 0; // TODO - Add MIAException here.
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
        auto it = RawConfigVals.find(key);
        if (it == RawConfigVals.end()) 
            return 0.0; // TODO - Add MIAException here.
        try 
        {
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
        auto it = RawConfigVals.find(key);
        if (it == RawConfigVals.end()) 
            return {}; // TODO - Add MIAException here.
            
        return it->second;
    }


    std::vector<std::string> MIAConfig::getVector(const std::string& key, char delimiter) const
    {
        std::vector<std::string> result;
        auto it = RawConfigVals.find(key);
        if (it == RawConfigVals.end()) 
            return result; // TODO - Add MIAException here.

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
        auto it = RawConfigVals.find(key);
        if (it == RawConfigVals.end()) 
            return false; // TODO - Add MIAException here.
            
        std::string val = it->second;
        std::transform(val.begin(), val.end(), val.begin(), ::tolower);
        
        // Check for different forms of true.
        return (val == "true" || val == "1" || val == "yes" || val == "on");
    }


    std::vector<int> MIAConfig::getIntVector(const std::string& key, char delimiter) const
    {
        std::vector<int> result;
        auto it = RawConfigVals.find(key);
        if (it == RawConfigVals.end()) 
            return result; // TODO - Add MIAException here.

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

