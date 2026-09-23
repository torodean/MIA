/**
 * @file Logger.hpp
 * @author Antonius Torode
 * @date 05/26/2025
 * Description: This file contains the main logging features for MIA apps and code.
 */
#pragma once

#include <string>
#include <fstream>
#include <vector>

namespace logger
{
    /// Default log file name used by the free functions
    const std::string DEFAULT_LOG_FILE = "MIA.log";

    /**
     * Logs a message to the default log file. This calls logToFile().
     * Optionally prints the message to stdout if verbose is true.
     * @param message The message to log.
     * @param verbose Whether to print the message to stdout. Default: false.
     */
    void logToDefaultFile(const std::string& message,
                          bool verbose = false);

    /**
     * Logs a message with optional tags to the default log file. This calls the tags
     * overload of logToFile(), with the tags formatted the same way as the Logger
     * class tags overload (i.e. [tag1, tag2, ...]: message).
     * @param message The message to log.
     * @param tags Optional tags to prepend to the log messages. These will appear before
     *        the log messages between brackets (i.e. [tag1, tag2, tag3, ...]: message).
     * @param verbose Whether to print the message to stdout. Default: false.
     */
    void logToDefaultFile(const std::string& message,
                          const std::vector<std::string>& tags,
                          bool verbose = false);

    /**
     * Logs a message to a specified log file. This will attempt to use a somewhat smart
     * lookup to determnine the full file path if only a file name is input. This uses
     * paths::getDefaultLogDirToUse() to find the full file name. If a "/" is the first
     * character of the filename, a full path is assumed.
     * Optionally prints the message to stdout if verbose is true.
     * 
     * @note This will assume the log directory for the output file already exists.
     * @param message The message to log.
     * @param filename The log file name to write to.
     * @param verbose Whether to print the message to stdout. Default: false.
     * @see paths::getDefaultLogDirToUse()
     */
    void logToFile(const std::string& message,
                   const std::string& filename,
                   bool verbose = false);

    /**
     * Logs a message with optional tags to a specified log file. This behaves the same
     * as logToFile(), with the tags formatted the same way as the Logger class tags
     * overload (i.e. [tag1, tag2, ...]: message).
     *
     * @note This will assume the log directory for the output file already exists.
     * @param message The message to log.
     * @param filename The log file name to write to.
     * @param tags Optional tags to prepend to the log messages. These will appear before
     *        the log messages between brackets (i.e. [tag1, tag2, tag3, ...]: message).
     * @param verbose Whether to print the message to stdout. Default: false.
     * @see paths::getDefaultLogDirToUse()
     */
    void logToFile(const std::string& message,
                   const std::string& filename,
                   const std::vector<std::string>& tags,
                   bool verbose = false);

    /**
     * @brief Logs the name of the calling method along with optional parameters.
     * 
     * This function should be called at the start of a method to automatically log
     * the method name and optionally any parameters. It delegates to the `logToFile()` method.
     * This method is mostly for debugging and tracking method calls.
     * 
     * @param methodName Name of the calling method (typically passed via __func__).
     * @param filename The log file name to write to.
     * @param params Optional string representing parameters to include in the log.
     * @param verbose Whether to print the message to stdout. Default: false.
     */
    void logMethodCallToFile(const std::string& methodName, 
                             const std::string& filename, 
                             const std::string& params = "", 
                             bool verbose = false);

    /**
     * Logger class for simplified logging in apps.
     * Allows changing log file and automatically logs messages to current file.
     */
    class Logger
    {
    public:
    
        /**
         * @brief Default constructor for Logger. This will initialize the log file name
         * to DEFAULT_LOG_FILE.
         * 
         * Initializes the Logger with the default log file name and opens the log file.
         * Ensures that logging is ready to use immediately after construction.
         */
        Logger();
        
        /**
         * Construct a Logger with a specific log file name. This will open the log file 
         * as logStream to use for logging.
         * @param filename The log file to use.
         */
        explicit Logger(const std::string& filename);

        /**
         * Destructor closes the log file. This will close the log file open by logStream.
         */
        ~Logger();

        /**
         * Change the log file used by this Logger instance.
         * Closes the current stream and opens the new file.
         * 
         * @param filename The new log file name.
         */
        void setLogFile(const std::string& filename);

        /**
         * Log a message using the Logger's current log file.
         * Optionally prints the message to stdout if verbose is true.
         * 
         * @param message The message to log.
         * @param verbose Whether to print the message to stdout. Default: false.
         */
        void log(const std::string& message,
                 bool verbose = false) const;        
                 
        /**
         * Log a message using the Logger's current log file.
         * Optionally prints the message to stdout if verbose is true.
         * 
         * @param message The message to log.
         * @param tags Optional tags to prepend to the log messages. These will appear before 
         *        the log messages between brackets (i.e. [tag1, tag2, tag3, ...]: message).
         * @param verbose Whether to print the message to stdout. Default: false.
         */
        void log(const std::string& message,
                 const std::vector<std::string>& tags,
                 bool verbose = false) const;

        /**
         * @brief Logs the name of the calling method and optional parameters using the Logger's 
         *        current log file.
         * 
         * This function should be called at the start of a method to automatically log
         * the method name and optionally any parameters. It delegates to the `log()` method.
         * This method is mostly for debugging and tracking method calls.
         * 
         * @param methodName Name of the calling method (typically passed via __func__).
         * @param params Optional string representing parameters to include in the log.
         * @param verbose Whether to print the message to stdout. Default: false.
         */
        void logMethodCall(const std::string& methodName,
                           const std::string& params = "",
                           bool verbose = false);

        /**
         * Get the current log file name.
         * @return The current log file name.
         */
        std::string getLogFile() const;
        
        /**
         * Sets the optional applicationName variable to be prepended to logs. When this is
         * set, the log messages will be prepended with the application name. To disable this
         * option, call the clearApplicationName() method. When set, the application name will
         * be treated as a tag, and attached to the front of the log message (after the 
         * timestamp) surrounded by brackets (i.e., <timestamp> [appName]: logMessage).
         */
        void setApplicationName(const std::string& appName);
        
        /**
         * Clears the applicationName.
         */
        void clearApplicationName();

    private:
    
        /// The log file for this object. This can be a full path or just a file name.
        std::string currentLogFileName;
        /// The full path to the log file. This is auto-set in openLogFile()
        std::string currentLogFileFullPath;
        
        /**
         * The optional application name to tag log messages with. When this is set via 
         * setApplicationName(), the app name will appear attached to the log messages.
         */
        std::string applicationName;
        
        /**
         * Output file stream used for writing log messages.
         * Declared mutable to allow logging from const methods without altering the logical state.
         */
        mutable std::ofstream logStream;

        /**
         * Opens the current log file in append mode. This will attempt to use a somewhat smart
         * lookup to determnine the full file path based on currentLogFileName. This uses
         * paths::getDefaultLogDirToUse() to find the full file name. If a "/" is the first
         * character of the filename, a full path is assumed. Called during construction and 
         * when the log file is changed. Ensures logStream is ready for writing.
         * @see paths::getDefaultLogDirToUse()
         */
        void openLogFile();
        
    }; // class Logger
} // namespace logger
