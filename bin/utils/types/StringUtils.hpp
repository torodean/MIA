/**
 * @file StringUtils.hpp
 * @author Antonius Torode
 * @date 03/01/2021
 * Description: 
 *     This file provides a collection of utility functions for string parsing,
 *     transformation, and analysis. It includes operations such as character
 *     filtering, case conversion, string splitting, format validation, and
 *     substring extraction between delimiters. The utilities are designed to
 *     simplify common string manipulation tasks and support text processing
 *     across different parts of the application.
 * 
 *     These functions are encapsulated within the `StringUtils` namespace and are
 *     intended for general-purpose use with standard string types.
 */
#pragma once

#include <string>
#include <vector>

namespace StringUtils
{
    /**
     * Converts a string to lower case.
     * @param input[std::string&] - Reference to the string to convert.
     * @return [std::string] - Lower case string.
     */
    std::string toLower(std::string& input);

    /**
     * Removes a specific character from a string.
     * @param str[std::string] - the input string to parse.
     * @param c[char] - the character to remove.
     * @return [std::string] - The string without hte character.
     */
    std::string removeCharInString(std::string str, char c);

    /**
     * Finds the first occurrence of a character in a string.
     * Delegates the actual search to BasicUtilities::findCharInString to avoid
     * core dependencies and promote reuse.
     *
     * @param input[const std::string&] - The string to search.
     * @param ch[char] - The character to find.
     * @return [int] - The index of the first occurrence of ch in str, or -1 if not found.
     */
    int findCharInString(std::string& input, char c);

    /**
     * Determines if a character is contained within a string.
     * @param input[std::string] - Reference to the input string to parse.
     * @param c[char] - The character to find.
     * @return [bool] - Returns true if the character is in the string.
     */
    bool stringContainsChar(std::string& input, char c);

    /**
     * Separates a string into components via a delimiter.
     * @param input[std::string] - Reference to the input string to parse.
     * @param delimiter[std::string] - Delimiter to use when parsing.
     * @param verboseMode[bool] - Enables verboseMode output (default = false).
     * @return
     */
    std::vector<std::string> delimiterString(std::string& input, const std::string &delimiter, bool verboseMode = false);

    /**
     * Function for determining if all characters in a string are digits/integers.
     * @param input[std::string] - Reference to the input string to parse.
     * @return [bool] - True if all characters are integers.
     */
    bool is_digits(const std::string& input);

    /**
     * Determines if a string is of the format for a dice roll. i.e 1d20 or 3d8.
     * @param input[std::string] - Reference to the input string to parse.
     * @return [bool] - True if the string is a dice roll.
     */
    bool inputRoll(std::string &input);

    /**
     * A function used to determine if an answer is equivalent to yes.
     * @param input[std::string] - Reference to the input string to parse.
     * @return [bool] - True if answer is a form of yes.
     */
    bool formOfYes(std::string &input);

    /**
     * Returns the date for today.
     * @return[std::string] - A string representing the current date.
     */
    std::string today();

    /**
     * Returns a shuffled string.
     * @param input[std::string] - The string to shuffle.
     * @return [std::string] - That has been shuffled.
     */
    std::string shuffleString(std::string input);

    /**
     * Returns the substring of `line` that appears before the first occurrence of character `c`.
     * If `c` is not found, returns the entire string.
     * @param line[std::string] - The input string to process.
     * @param c[char] - The delimiter character.
     * @param verboseMode[bool] - Enables verboseMode output (default = false).
     * @return [std::string] - Substring before `c`.
     */
    std::string getBeforeChar(std::string line, char c, bool verboseMode = false);
    
    /**
     * Returns the substring of `line` that appears after the first occurrence of character `c`.
     * If `c` is not found, returns an empty string.
     * @param line[std::string] - The input string to process.
     * @param c[char] - The delimiter character.
     * @param verboseMode[bool] - Enables verboseMode output (default = false).
     * @return [std::string] - Substring after `c`.
     */
    std::string getAfterChar(std::string line, char c, bool verboseMode = false);
    
    /**
     * Extracts and returns the substring between the first '=' and the first ';' in the input string.
     * It first trims the string at the ';' and then extracts the portion after the '='.
     * If '=' or ';' are not found, behavior depends on `findCharInString` return values and substr calls.
     * @param line[std::string] - The input string to parse.
     * @param verboseMode[bool] - Enables verboseMode output (default = false).
     * @return [std::string] - Substring between '=' and ';', or possibly empty if delimiters are missing.
     */
    [[deprecated("Use getBetweenXAndY(line, '=', ';') instead")]]
    std::string getBetweenEqualAndSemiColon(std::string line, bool verboseMode = false);

    /**
     * Extracts and returns the substring between the first occurrences of characters `x` and `y`
     * in the input string. The result excludes the delimiter characters themselves.
     * If either character is not found, the behavior depends on the result of `findCharInString`.
     *
     * @param line [std::string] - The input string to parse.
     * @param x[char] - The starting delimiter character.
     * @param y[char] - The ending delimiter character.
     * @param verboseMode[bool] - Enables verboseMode output (default = false).
     * @return [std::string] - Substring between `x` and `y`, or possibly empty if delimiters are not found.
     */
    std::string getBetweenXAndY(std::string line, char x, char y, bool verboseMode = false);

    /**
     * This will create two strings of 'entangled' text. The first string contains every other character
     * of the input string and the second string contains the opposite characters. Spaces are added
     * in place of the characters not shown in each output string so that they can be easily matched.
     * @param input[const std::string&] - The input string to entangle.
     * @return [std::vector<std::string>] - The first element is the first half,
     *                                      and second element the second half.
     */
    std::vector<std::string> entangleText(const std::string& input);
    
    /**
     * Checks if the given line contains exactly one occurrence of each specified delimiter.
     *
     * @param line[const std::string&] The string to check.
     * @param delimiterA[char] - The first delimiter to count.
     * @param delimiterB[char] - The second delimiter to count.
     * @return [bool] True if the line contains exactly one occurrence of delimiterA and exactly one occurrence of delimiterB; otherwise false.
     */
     bool hasExactlyOneOfEachDelimiter(const std::string& line, char delimiterA, char delimiterB);
     
    /**
     * Checks if the given line contains exactly one occurrence of a specified delimiter.
     *
     * @param line[const std::string&] The string to check.
     * @param delimiterA[char] - The first delimiter to count.
     * @return [bool] True if the line contains exactly one occurrence of the delimiter; otherwise false.
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
     * @param str[const std::string&]The string to trim.
     * @return [std::string] - The trimmed string.
     */
    std::string trim(const std::string& str);
    
    /**
     * Converts a vector of characters into a single concatenated string.
     * @param charVec[const std::vector<char>&] - The vector of characters to convert.
     * @return [std::string] - A string formed by concatenating all characters in the vector.
     */
    std::string charVecToString(const std::vector<char>& charVec);

    /**
     * Converts a string into a vector of its individual characters.
     * @param str[const std::string&] - The input string to convert.
     * @return [std::vector<char>] - A vector containing each character from the string.
     */
    std::vector<char> stringToCharVec(const std::string& str);
    
    /**
     * @brief Converts a string into a vector of integer values.
     *
     * Each character in the input string is cast to its corresponding
     * integer (ASCII) value and stored in a vector of integers.
     *
     * @param inputStr[const std::string&] - The input string to convert.
     * @return [std::vector<int>] - A vector of integers representing the ASCII 
     *     values of the input string's characters.
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
     * @return [std::string] - A string composed of characters corresponding to the input integers.
     */
    std::string intVectorToString(const std::vector<int>& inputVec);
    
    /**
     * @brief Returns the reversed version of the input string.
     *
     * If the input string is empty, returns an empty string.
     *
     * @param inputStr[const std::string&] - The string to invert.
     * @return [std::string] - A new string which is the reverse of inputStr.
     */
    std::string invertString(const std::string& inputStr);

} // namespace types
