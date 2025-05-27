/**
 * File: BasicUtilities.cpp
 * Author: Antonius Torode
 * Created on: 05/22/2025
 * Description: Implements basic utility functions declared in BasicUtilities.hpp.
 */

#include <string>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <iostream>

// Include the associated header file.
#include "BasicUtilities.hpp"

namespace BasicUtilities
{
    int findCharInString(const std::string& str, char ch)
    {
        auto pos = str.find(ch);
        return pos == std::string::npos ? -1 : static_cast<int>(pos);
    }
    
    
    std::string lstrip(const std::string& s) 
    {
        auto start = std::find_if_not(s.begin(), s.end(), [](unsigned char ch) 
        {
            return std::isspace(ch);
        });
        return std::string(start, s.end());
    }
    
    
    std::string rstrip(const std::string& s) 
    {
        auto end = std::find_if_not(s.rbegin(), s.rend(), [](unsigned char ch) 
        {
            return std::isspace(ch);
        }).base();
        return std::string(s.begin(), end);
    }
    
    
    std::string strip(const std::string& s) 
    {
        return lstrip(rstrip(s));
    }
    
    
    std::string getCurrentDateTime()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        std::tm tm_local{};
    #if defined(_WIN32)
        localtime_s(&tm_local, &now_c);
    #else
        localtime_r(&now_c, &tm_local);
    #endif

        std::ostringstream oss;
        oss << std::put_time(&tm_local, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
    
    
    bool ensureDirectoryExists(const std::string& path, bool createIfMissing)
    {
        try
        {
            namespace fs = std::filesystem;
            fs::path dirPath(path);

            if (fs::exists(dirPath)) 
            {
                return fs::is_directory(dirPath);
            }

            if (createIfMissing) 
            {
                return fs::create_directories(dirPath);
            }

            return false;
        }        
        catch (const std::filesystem::filesystem_error& e) 
        {
            std::cerr << "Filesystem error: " << e.what() << std::endl;
            return false;
        }
    }
}
