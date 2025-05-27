/**
 * File: Logger.cpp
 * Author: Antonius Torode
 * Creation Date: 05/26/2025
 * Description: This file implements the main logging features for MIA apps and code.
 */
 
#include <string>
#include <iostream>

// Include the associated header file.
#include "Logger.hpp"
// Used for exception throws.
#include "MIAException.hpp"
#include "Paths.hpp"


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
        } 
        else 
        {
            fullpath = paths::getDefaultLogDirToUse() + "/" + filename;
        }
        
        std::ofstream ofs(fullpath, std::ios::app);
        if (ofs.is_open())
        {
            ofs << message << std::endl;
        }
        if (verbose)
        {
            std::cout << message << std::endl;
        }
    }


    Logger::Logger(const std::string& filename)
        : currentLogFileName(filename)
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
            logStream << message << std::endl;
            
        if (verbose)
            std::cout << message << std::endl;
    }


    void Logger::openLogFile()
    {
        // Get the full path of the log file.
        if (currentLogFileName.front() == '/') 
        {
            currentLogFileFullPath = currentLogFileName; // Assume absolute path.
        } 
        else 
        {
            currentLogFileFullPath = paths::getDefaultLogDirToUse() + "/" + currentLogFileName;
        }
    
        logStream.open(currentLogFileFullPath, std::ios::app);
    }

} // namespace logger
