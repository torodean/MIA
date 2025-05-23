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
}
