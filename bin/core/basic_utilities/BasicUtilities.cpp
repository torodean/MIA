/**
 * @file BasicUtilities.cpp
 * @author Antonius Torode
 * @date 05/22/2025
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
#include <fstream>

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
    
    
    /**
     * @brief Ensures that a file exists at the given path.
     * 
     * Checks if the file exists. If it does, returns true.
     * If it doesn't exist and createIfMissing is true, creates an empty file.
     * Returns false if the path exists but is not a regular file, or on error.
     * 
     * @param path The path to the file.
     * @param createIfMissing Whether to create an empty file if missing.
     * @return true if file exists or was successfully created, false otherwise.
     */
    bool ensureFileExists(const std::string& path, bool createIfMissing)
    {
        try
        {
            namespace fs = std::filesystem;
            fs::path filePath(path);

            if (fs::exists(filePath))
            {
                return fs::is_regular_file(filePath);
            }

            if (createIfMissing)
            {
                // Create an empty file
                std::ofstream ofs(path);
                return ofs.good();
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
