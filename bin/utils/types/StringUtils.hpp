/**
 * @file StringUtils.hpp
 * @author Antonius Torode
 * @date 03/01/2021
 *
 * This file provides a collection of utility functions for string parsing,
 * transformation, and analysis. It includes operations such as character
 * filtering, case conversion, string splitting, format validation, and
 * substring extraction between delimiters. The utilities are designed to
 * simplify common string manipulation tasks and support text processing
 * across different parts of the application.
 * 
 * These functions are encapsulated within the `StringUtils` namespace and are
 * intended for general-purpose use with standard string types.
 */
#pragma once

#include <string>
#include <vector>

namespace StringUtils
{
    /**
     * @brief Converts a string to lower case.
     *
     * @param input The string to convert.
     * @return A lower case copy of the input.
     */
    std::string toLower(const std::string& input);
    
    /**
     * @brief Converts a string to upper case.
     *
     * @param input The string to convert.
     * @return An upper case copy of the input.
     */
    std::string toUpper(const std::string& input);

    /**
     * Removes a specific character from a string.
     * @param str the input string to parse.
     * @param c the character to remove.
     * @return The string without hte character.
     */
    std::string removeCharInString(const std::string& str, char c);

    /**
     * Finds the first occurrence of a character in a string.
     * Delegates the actual search to BasicUtilities::findCharInString to avoid
     * core dependencies and promote reuse.
     *
     * @param input The string to search.
     * @param ch The character to find.
     * @return The index of the first occurrence of ch in str, or -1 if not found.
     */
    int findCharInString(const std::string& input, char c);

    /**
     * @brief Determines if a character is contained within a string.
     * @param input Reference to the input string to parse.
     * @param c The character to find.
     * @return Returns true if the character is in the string.
     */
    bool stringContainsChar(const std::string& input, char c);

    /**
     * @brief Separates a string into components via a delimiter.
     * @param input Reference to the input string to parse.
     * @param delimiter Delimiter to use when parsing.
     * @param verboseMode Enables verboseMode output (default = false).
     * @return
     */
    std::vector<std::string> delimiterString(const std::string& input, const std::string &delimiter, bool verboseMode = false);

    /**
     * Function for determining if all characters in a string are digits/integers.
     * @param input Reference to the input string to parse.
     * @return True if all characters are integers.
     */
    bool isDigits(const std::string& input);

    /**
     * Determines if a string is of the format for a dice roll. i.e 1d20 or 3d8.
     * @param input Reference to the input string to parse.
     * @return True if the string is a dice roll.
     */
    bool inputRoll(const std::string& input);

    /**
     * A function used to determine if an answer is equivalent to yes.
     * @param input Reference to the input string to parse.
     * @return True if answer is a form of yes.
     */
    bool formOfYes(const std::string& input);

    /**
     * Returns the date for today.
     * @return A string representing the current date.
     */
    std::string today();

    /**
     * Returns a shuffled string.
     * @param input The string to shuffle.
     * @return That has been shuffled.
     */
    std::string shuffleString(const std::string& input);

    /**
     * Returns the substring of `line` that appears before the first occurrence of character `c`.
     * If `c` is not found, returns the entire string.
     *
     * @param line The input string to process.
     * @param c The delimiter character.
     * @param verboseMode Enables verboseMode output (default = false).
     * @return Substring before `c`.
     */
    std::string getBeforeChar(std::string line, char c, bool verboseMode = false);
    
    /**
     * Returns the substring of `line` that appears after the first occurrence of character `c`.
     * If `c` is not found, returns an empty string.
     *
     * @param line The input string to process.
     * @param c The delimiter character.
     * @param verboseMode Enables verboseMode output (default = false).
     * @return Substring after `c`.
     */
    std::string getAfterChar(std::string line, char c, bool verboseMode = false);
    
    /**
     * Extracts and returns the substring between the first '=' and the first ';' in the input string.
     * It first trims the string at the ';' and then extracts the portion after the '='.
     * If '=' or ';' are not found, behavior depends on `findCharInString` return values and substr calls.
     *
     * @param line The input string to parse.
     * @param verboseMode Enables verboseMode output (default = false).
     * @return Substring between '=' and ';', or possibly empty if delimiters are missing.
     */
    [[deprecated("Use getBetweenXAndY(line, '=', ';') instead")]]
    std::string getBetweenEqualAndSemiColon(std::string line, bool verboseMode = false);

    /**
     * Extracts and returns the substring between the first occurrences of characters `x` and `y`
     * in the input string. The result excludes the delimiter characters themselves.
     * If either character is not found, an empty string is returned.
     * If 'x' == 'y', the second occurance of 'y' is used. 
     *
     * @param line The input string to parse.
     * @param x The starting delimiter character.
     * @param y The ending delimiter character.
     * @param verboseMode Enables verboseMode output (default = false).
     * @return Substring between `x` and `y`, or empty if delimiters are not found.
     */
    std::string getBetweenXAndY(const std::string& line, char x, char y, bool verboseMode = false);

    /**
     * This will create two strings of 'entangled' text. The first string contains every other character
     * of the input string and the second string contains the opposite characters. Spaces are added
     * in place of the characters not shown in each output string so that they can be easily matched.
     *
     * @param input The input string to entangle.
     * @return The first element is the first half, and second element the second half.
     */
    std::vector<std::string> entangleText(const std::string& input);
    
    /**
     * Checks if the given line contains exactly one occurrence of each specified delimiter.
     *
     * @param line The string to check.
     * @param delimiterA The first delimiter to count.
     * @param delimiterB The second delimiter to count.
     * @return True if the line contains exactly one occurrence of delimiterA and exactly one occurrence of delimiterB; otherwise false.
     */
     bool hasExactlyOneOfEachDelimiter(const std::string& line, char delimiterA, char delimiterB);
     
    /**
     * Checks if the given line contains exactly one occurrence of a specified delimiter.
     *
     * @param line The string to check.
     * @param delimiterA The first delimiter to count.
     * @return True if the line contains exactly one occurrence of the delimiter; otherwise false.
     */
     bool hasExactlyOneOfADelimiter(const std::string& line, char delimiterA);
     
    /**
     * @brief Checks if one string contains another substring.
     * @param haystack The string to search within.
     * @param needle The substring to search for.
     * @return true if 'needle' is found within 'haystack'; false otherwise.
     */
    bool contains(const std::string& haystack, const std::string& needle);
    
    /**
     * @brief Removes leading and trailing whitespace characters from the given string.
     * @param str The string to trim.
     * @return The trimmed string.
     */
    std::string trim(const std::string& str);
    
    /**
     * Converts a vector of characters into a single concatenated string.
     * @param charVec The vector of characters to convert.
     * @return A string formed by concatenating all characters in the vector.
     */
    std::string charVecToString(const std::vector<char>& charVec);

    /**
     * Converts a string into a vector of its individual characters.
     * @param str The input string to convert.
     * @return A vector containing each character from the string.
     */
    std::vector<char> stringToCharVec(const std::string& str);
    
    /**
     * @brief Converts a string into a vector of integer values.
     *
     * Each character in the input string is cast to its corresponding
     * integer (ASCII) value and stored in a vector of integers.
     *
     * @param inputStr The input string to convert.
     * @return A vector of integers representing the ASCII values of the input string's characters.
     */
    std::vector<int> stringToIntVector(const std::string& inputStr);
    
    /**
     * @brief Converts a vector of integers to a string.
     *
     * Each integer in the input vector is cast to a character and inserted
     * into the corresponding position of the output string. Assumes that all
     * integers are valid ASCII character codes.
     *
     * @param inputVec[const std::vector<int>&] - The vector of integers to convert.
     * @return A string composed of characters corresponding to the input integers.
     */
    std::string intVectorToString(const std::vector<int>& inputVec);
    
    /**
     * @brief Returns the reversed version of the input string.
     *
     * If the input string is empty, returns an empty string.
     *
     * @param inputStr The string to invert.
     * @return A new string which is the reverse of inputStr.
     */
    std::string invertString(const std::string& inputStr);
    
    /**
     * @brief Centers text within a field of the specified width.
     *
     * If the text is wider than or equal to the specified width, the original
     * text is returned unchanged.
     *
     * @param text The text to center.
     * @param width The total width of the resulting field.
     * @return The text centered within the specified width.
     */
    std::string centerText(const std::string& text, std::size_t width);

} // namespace types
