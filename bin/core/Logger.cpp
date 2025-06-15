/**
 * File: Logger.cpp
 * Author: Antonius Torode
 * Creation Date: 05/26/2025
 * Description: This file implements the main logging features for MIA apps and code.
 */
 
#include <string>
#include <iostream>
#include <filesystem>

// Include the associated header file.
#include "Logger.hpp"
// Used for exception throws.
#include "MIAException.hpp"
#include "Paths.hpp"
// Used for printing the time string in the logs output.
#include "BasicUtilities.hpp"


namespace logger
{
    void logToDefaultFile(const std::string& message, bool verbose)
    {
        logToFile(message, DEFAULT_LOG_FILE, verbose);
    }


    void logToFile(const std::string& message, const std::string& filename, bool verbose)
    {
        std::string fullpath;
        // Get the full path of the log file.
        if (filename.front() == '/') 
        {
            fullpath = filename; // Assume absolute path.
        
            // Create the necessary directory if it does not exist.
            std::filesystem::path filePath(fullpath);
            std::string parentDir = filePath.parent_path().string();
            BasicUtilities::ensureDirectoryExists(parentDir, true);
        } 
        else 
        {
            fullpath = paths::getDefaultLogDirToUse() + "/" + filename;
        }
        
        if (!BasicUtilities::ensureFileExists(fullpath))
            MIA_THROW(error::ErrorCode::Failed_To_Open_File);
        
        std::ofstream ofs(fullpath, std::ios::app);
        if (ofs.is_open())
        {
            ofs << BasicUtilities::getCurrentDateTime() << ": " << message << std::endl;
        }
        if (verbose)
        {
            std::cout << message << std::endl;
        }
    }


    void logMethodCallToFile(const std::string& methodName, 
                             const std::string& filename, 
                             const std::string& params, 
                             bool verbose) 
    {
        std::string msg = methodName + "(" + params + ")";
        logToFile(msg, filename, verbose);
    }


    Logger::Logger() : currentLogFileName(DEFAULT_LOG_FILE)
    {
        openLogFile();
    }

    Logger::Logger(const std::string& filename) : currentLogFileName(filename)
    { 
        openLogFile();
    }
    
    
    Logger::~Logger()
    {
        if (logStream.is_open())
            logStream.close();
    }


    void Logger::setLogFile(const std::string& filename)
    {
        if (logStream.is_open())
            logStream.close();
            
        currentLogFileName = filename;
        openLogFile();
    }


    void Logger::log(const std::string& message, bool verbose) const
    {
        if (logStream.is_open())
            logStream << BasicUtilities::getCurrentDateTime() << ": " << message << std::endl;
            
        if (verbose)
            std::cout << message << std::endl;
    }


    void Logger::logMethodCall(const std::string& methodName,
                               const std::string& params, 
                               bool verbose) 
    {
        std::string msg = methodName + "(" + params + ")";
        log(msg, verbose);
    }


    void Logger::openLogFile()
    {
        // Get the full path of the log file.
        if (currentLogFileName.front() == '/') 
        {
            currentLogFileFullPath = currentLogFileName; // Assume absolute path.
        
            // Create the necessary directory if it does not exist.
            std::filesystem::path filePath(currentLogFileFullPath);
            std::string parentDir = filePath.parent_path().string();            
            BasicUtilities::ensureDirectoryExists(parentDir, true);
        } 
        else 
        {
            currentLogFileFullPath = paths::getDefaultLogDirToUse() + "/" + currentLogFileName;
        }
        
        if (!BasicUtilities::ensureFileExists(currentLogFileFullPath))
            MIA_THROW(error::ErrorCode::Failed_To_Open_File);
    
        logStream.open(currentLogFileFullPath, std::ios::app);
        if (!logStream.is_open())
        {
            // TODO - throw MIAException.
            std::cerr << "Failed to open logStream: " << currentLogFileFullPath << std::endl;
        }
    }

} // namespace logger
