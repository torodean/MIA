/**
 * @file Logger.cpp
 * @author Antonius Torode
 * @date 05/26/2025
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
    namespace
    {
        /**
         * @brief Resolves the full path of a log file name.
         *
         * A file name starting with '/' is assumed to be an absolute path. Any other
         * name is resolved against the default log directory. The parent directory of
         * an absolute path is created when missing.
         *
         * @param filename The log file name to resolve.
         * @return The full path of the log file.
         * @throws MIAException if the log file does not exist and cannot be created.
         * @see paths::getDefaultLogDirToUse()
         */
        std::string resolveLogFilePath(const std::string& filename)
        {
            std::string fullpath;
            // Get the full path of the log file.
            if (filename.front() == '/')
            {
                fullpath = filename; // Assume absolute path.

                // Create the necessary directory if it does not exist.
                std::filesystem::path filePath(fullpath);
                std::string parentDir = filePath.parent_path().string();
                basic_utils::ensureDirectoryExists(parentDir, true);
            }
            else
            {
                fullpath = paths::getDefaultLogDirToUse() + "/" + filename;
            }

            if (!basic_utils::ensureFileExists(fullpath))
                MIA_THROW(error::ErrorCode::Failed_To_Open_File);

            return fullpath;
        }


        /**
         * @brief Builds the bracketed tag prefix for a log message.
         *
         * @param tags The tags to include. The tags are joined with ", " between
         *        brackets (i.e. [tag1, tag2]).
         * @return The formatted tag string, or an empty string when no tags are given.
         */
        std::string buildTagString(const std::vector<std::string>& tags)
        {
            std::string tagString;
            if (!tags.empty())
            { // Check if any tags are set.
                tagString = "[";
                for (size_t i=0; i<tags.size(); i++)
                { // Loop over the tags.
                    tagString += tags[i];
                    if (i+1<tags.size())
                        tagString += ", ";
                }
                tagString += "]";
            }

            return tagString;
        }


        /**
         * @brief Writes one timestamped log line to a log file.
         *
         * The line is written in the form "<timestamp> [tag]: message", where the
         * tag part (including its preceding space) is omitted when the tag string
         * is empty. Optionally prints the message to stdout.
         *
         * @param fullpath The full path of the log file to append to.
         * @param tagString The pre-formatted bracketed tag string, or an empty string.
         * @param message The message to log.
         * @param verbose Whether to print the message to stdout.
         */
        void writeLogEntry(const std::string& fullpath,
                           const std::string& tagString,
                           const std::string& message,
                           bool verbose)
        {
            std::ofstream ofs(fullpath, std::ios::app);
            if (ofs.is_open())
            {
                ofs << basic_utils::getCurrentDateTime()
                    << (tagString.empty() ? "" : " " + tagString)
                    << ": " << message << std::endl;
            }
            if (verbose)
            {
                std::cout << message << std::endl;
            }
        }
    } // namespace


    void logToDefaultFile(const std::string& message, bool verbose)
    {
        logToFile(message, DEFAULT_LOG_FILE, verbose);
    }


    void logToDefaultFile(const std::string& message,
                          const std::vector<std::string>& tags,
                          bool verbose)
    {
        logToFile(message, DEFAULT_LOG_FILE, tags, verbose);
    }


    void logToFile(const std::string& message, const std::string& filename, bool verbose)
    {
        writeLogEntry(resolveLogFilePath(filename), "", message, verbose);
    }


    void logToFile(const std::string& message,
                   const std::string& filename,
                   const std::vector<std::string>& tags,
                   bool verbose)
    {
        writeLogEntry(resolveLogFilePath(filename), buildTagString(tags), message, verbose);
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


    void Logger::log(const std::string& message, 
                     bool verbose) const
    {
        if (logStream.is_open())
            logStream << basic_utils::getCurrentDateTime()
                      << (applicationName.empty() ? "" : " [" + applicationName + "]")
                      << ": " << message
                      << std::endl;
            
        if (verbose)
            std::cout << message << std::endl;
    }


    void Logger::log(const std::string& message,
                     const std::vector<std::string>& tags,
                     bool verbose) const
    {
        // The app name, when set, is treated as the first tag.
        std::vector<std::string> allTags;
        if (!applicationName.empty())
            allTags.push_back(applicationName);
        allTags.insert(allTags.end(), tags.begin(), tags.end());

        const std::string tagString = buildTagString(allTags);

        if (logStream.is_open())
            logStream << basic_utils::getCurrentDateTime()
                      << (!tagString.empty() ? " " : "") << tagString
                      << ": " << message
                      << std::endl;

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


    std::string Logger::getLogFile() const
    { 
        return currentLogFileName; 
    }
    
    
    void Logger::setApplicationName(const std::string& appName)
    {
        applicationName = appName;
    }


    void Logger::clearApplicationName()
    {
        applicationName.clear();
    }


    void Logger::openLogFile()
    {
        currentLogFileFullPath = resolveLogFilePath(currentLogFileName);

        logStream.open(currentLogFileFullPath, std::ios::app);
        if (!logStream.is_open())
        {
            // TODO - throw MIAException.
            std::cerr << "Failed to open logStream: " << currentLogFileFullPath << std::endl;
        }
    }

} // namespace logger
