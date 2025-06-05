/**
 * File: RawLinesData.cpp
 * Author: Antonius Torode
 * Created on: 05/24/2025
 * Description: 
 */

#include <iostream>
#include <fstream>

// Include the associated header.
#include "RawLinesData.hpp"
// Used for exception handling and error codes.
#include "MIAException.hpp"
#include "Error.hpp"

namespace MIA_System
{
    void RawLinesData::load(const std::string& filePath, bool verboseMode)
    {
        std::ifstream file(filePath, std::ifstream::in);
        if (!file.is_open()) 
        {            
            std::string err = "Failed to open config file: " + filePath;
            throw error::MIAException(error::ErrorCode::Failed_To_Open_File, err);
        }            
        rawConfigLines.clear();
        
        std::string line;
        while (std::getline(file, line)) 
        {
            if (!line.empty() && line[0] != '#' && line.size()>2)
            {
                if(verboseMode)
				    std::cout << "Config line read: " << line << std::endl;
                rawConfigLines.push_back(line);
            }
        }
        
        if (verboseMode) 
        {
            std::cout << "Finished loading raw lines from: " << filePath << std::endl;
        }
    }


    int RawLinesData::getInt(const std::string& key) const
    {
        throw error::MIAException(error::ErrorCode::Invalid_Configuration_Call, 
            "getInt not supported for RAW_LINES configuration file.");
    }


    double RawLinesData::getDouble(const std::string& key) const
    {
        throw error::MIAException(error::ErrorCode::Invalid_Configuration_Call, 
            "getDouble not supported for RAW_LINES configuration file.");
    }


    std::string RawLinesData::getString(const std::string& key) const
    {
        throw error::MIAException(error::ErrorCode::Invalid_Configuration_Call, 
            "getString not supported for RAW_LINES configuration file.");
    }


    std::vector<std::string> RawLinesData::getVector(const std::string& key, char delimiter) const
    {
        throw error::MIAException(error::ErrorCode::Invalid_Configuration_Call, 
            "getVector not supported for RAW_LINES configuration file.");
    }


    bool RawLinesData::getBool(const std::string& key) const
    {
        throw error::MIAException(error::ErrorCode::Invalid_Configuration_Call, 
            "getBool not supported for RAW_LINES configuration file.");
    }


    std::vector<int> RawLinesData::getIntVector(const std::string& key, char delimiter) const
    {
        throw error::MIAException(error::ErrorCode::Invalid_Configuration_Call, 
            "getIntVector not supported for RAW_LINES configuration file.");
    }


    std::vector<constants::KeyValuePair> RawLinesData::getAllConfigPairs() const
    {        
        throw error::MIAException(error::ErrorCode::Invalid_Configuration_Call, 
            "getAllConfigPairs not supported for RAW_LINES configuration file.");
    }


    std::vector<std::string> RawLinesData::getRawLines() const
    {
        return rawConfigLines;
    }


    void RawLinesData::dump(std::ostream& os) const
    {
        os << "Dumping rawConfigLines:" << std::endl;
        for (const auto& line : rawConfigLines)
        {
            os << line << std::endl;
        }
        os << std::endl;
    }


    constants::ConfigType RawLinesData::getType() const
    {
        return constants::ConfigType::RAW_LINES;
    }
} // namespace MIA_System
