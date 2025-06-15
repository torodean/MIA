/**
 * File: BasicUtilities.hpp
 * Author: Antonius Torode
 * Created on: 05/22/2025
 * Description: Declares basic utility functions that provide
 *              lightweight, dependency-free string and other helpers
 *              for use across the core codebase.
 */
#pragma once

#include <string>

namespace BasicUtilities
{
    /**
     * Finds the first occurrence of a character in a string.
     * @param str [const std::string&] - The string to search.
     * @param ch [char] - The character to find.
     * @return [int] - The index of the first occurrence of ch in str,
     *                 or -1 if not found.
     */
    int findCharInString(const std::string& str, char ch);
    
    /**
     * @brief Removes leading whitespace characters from the input string.
     * @param input[const std::string&] - The input string to trim.
     * @return [std::string] - A new string with leading whitespace removed.
     */
    std::string lstrip(const std::string& input);

    /**
     * @brief Removes trailing whitespace characters from the input string.
     * @param input[const std::string&] - The input string to trim.
     * @return [std::string] - A new string with trailing whitespace removed.
     */
    std::string rstrip(const std::string& input);

    /**
     * @brief Removes leading and trailing whitespace characters from the input string.
     * @param input[const std::string&] - The input string to trim.
     * @return [std::string] - A new string with both leading and trailing whitespace removed.
     */
    std::string strip(const std::string& input);
    
    /**
     * @brief Returns the current local date and time as a formatted string.
     * The format is "YYYY-MM-DD HH:MM:SS", suitable for use in log file entries.
     * Thread-safe on both POSIX and Windows systems.
     * @return std::string The current date and time as a human-readable string.
     */
    std::string getCurrentDateTime();
    
    /**
     * @brief Checks if a directory exists, and optionally creates it if it does not.
     * @param path The path to the directory.
     * @param createIfMissing If true, the directory will be created (including parent directories) 
     *     if it doesn't exist. This will default to true.
     * @return true if the directory exists or was successfully created; false otherwise.
     */
    bool ensureDirectoryExists(const std::string& path, bool createIfMissing = true);
    
    /**
     * @brief Ensures that a file exists at the given path.
     * 
     * Checks if the file exists. If it does, returns true.
     * If it doesn't exist and createIfMissing is true, creates an empty file.
     * Returns false if the path exists but is not a regular file, or on error.
     * 
     * @param path The path to the file.
     * @param createIfMissing If true, the directory will be created (including parent directories) 
     *     if it doesn't exist. This will default to true.
     * @return true if file exists or was successfully created, false otherwise.
     */
    bool ensureFileExists(const std::string& path, bool createIfMissing = true);
}
