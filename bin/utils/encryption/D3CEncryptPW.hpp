//============================================================================
// Name        : D3CEncryptPW.h
// Author      : Antonius Torode
// Copyright   : This file can be used under the conditions of Antonius' 
//				 General Purpose License (AGPL).
// Created on  : October, 2014
// Description : The header file for use with D3CEncryptPW.cpp
//               This file is very old. Many of the comments don't make much 
//               sense or are non-existant. I have kept it mostly in its
//               original form with only a few minor improvements since then.
//               Most of the improvements involve moving utility methods outside of
//               this class, refactoring, and adding comments.
//============================================================================
#pragma once

#include <string>
#include <vector>

namespace d0s2
{
    /**
     * Main UI runner for the D3CEncryptPW (d0s2) program feature. Encryption
     */
    void d0s2CryptRunner();

    /**
     * Main UI runner for the D3CEncryptPW (d0s2) program feature. DeCryption
     */
    void d0s2DeCryptRunner();

    /// Message is the original message and PW is the password used.
    std::string EncryptPW(std::string message, std::string password);
    
    /// Message is the original message and PW is the password used.
    std::string DecryptPW(const std::string& message, std::string password);
    
    /**
     * This namespace is for code which is internal to this encryption algorithm.
     */
    namespace internal
    {
        /**
         * @brief Repeats and truncates a password string to match a desired length.
         *
         * This method constructs a new string by repeating the input password (`PW`)
         * `PWNeeded` times and appending the first `PWRemainder` characters of `PW`
         * to reach the final length. This is typically used to generate a key or mask
         * that matches the length of a target message for encryption purposes.
         *
         * @param PW[std::string] - The input password std::string to be repeated.
         * @param PWNeeded[int] - The number of full repetitions of the password.
         * @param PWRemainder[int] - The number of additional characters from the password to append after the full repetitions.
         * @return [std::string] - A string consisting of `PW` repeated `PWNeeded` times, followed by the first `PWRemainder` characters of `PW`.
         */
        std::string PWRepeat(std::string PW, int PWNeeded, int PWRemainder);
        
        /**
         * @brief Converts a vector of string representations of characters into a vector of corresponding integer values.
         *
         * This function maps each string element in the input vector `messageVector` to its corresponding integer value.
         * The valid mappings are:
         * - "0" to "9" → 0 to 9
         * - "A" to "E" → 10 to 14
         * Any string not matching the above set will leave the corresponding index in the output vector uninitialized.
         *
         * @param messageVector[std::vector<string>] - A vector of strings containing characters ("0"-"9", "A"-"E").
         * @return A vector of integers where each element is the numeric equivalent of the corresponding string input.
         */
        std::vector<int> messageVectorStringToInt(std::vector<std::string> messageVector);
        
        /**
         * @brief Combines two integer vectors element-wise by addition.
         *
         * Adds each corresponding element from `messageVectorValues` and `PWVectorValues` to produce a resulting vector.
         * Assumes both input vectors are of the same length.
         *
         * @param messageVectorValues The first vector of integer values (e.g., message).
         * @param PWVectorValues The second vector of integer values (e.g., password).
         * @return A vector containing the sum of corresponding elements from the input vectors.
         */
        std::vector<int> combineMandPW(std::vector<int> messageVectorValues, std::vector<int> PWVectorValues);


        /**
         * @brief Reverses the element-wise addition of two integer vectors by subtraction.
         *
         * Subtracts each element of `PWVectorValues` from the corresponding element in `messageVectorValues`.
         * Assumes both input vectors are of the same length.
         *
         * @param messageVectorValues The combined vector of integer values.
         * @param PWVectorValues The vector that was added to create the combined vector.
         * @return A vector containing the difference of corresponding elements from the input vectors.
         */
        std::vector<int> UncombineMandPW(std::vector<int> messageVectorValues, std::vector<int> PWVectorValues);

        /**
         * @brief Converts a vector of integers to a string representation using 0–9 and A–P mappings.
         *
         * Each integer in the input vector is mapped to a corresponding character:
         * - 0–9 → '0'–'9'
         * - 10–25 → 'A'–'P'
         * The resulting characters are concatenated into a single string.
         * Assumes all values in the vector are within the range [0, 25].
         *
         * @param a A vector of integers to convert.
         * @return A string where each character represents an integer from the input vector.
         */
        std::string combinedVecTostring(std::vector<int> a);


        /**
         * @brief Converts a string into a vector of single-character strings.
         *
         * Each character from the input string is converted to a separate `std::string`
         * and stored in the output vector.
         *
         * @param input A standard string to be converted.
         * @return A vector where each element is a one-character string from the input.
         */
        std::vector<std::string> normalStringToVector(std::string input);

        /**
         * @brief Combines a message and a password string into an encoded output string.
         *
         * This function converts the input `message` and `PW` (password) strings into vectors of integer values using
         * predefined mappings (e.g., '0'-'9' → 0-9, 'A'-'E' → 10-14), adds corresponding elements together, and then
         * converts the resulting integer vector back to a single encoded string.
         *
         * @param message The input message string to encode.
         * @param PW The password string used for encoding.
         * @return A combined and encoded string resulting from the element-wise addition of message and password values.
         */
        std::string PWmessageCombine(std::string message, std::string PW);


        /**
         * @brief Reverses the encoding process done by PWmessageCombine.
         *
         * Converts the encoded `message` and the original `PW` into vectors of integers, subtracts the password values
         * from the message values element-wise, and reconstructs the original string using the inverse transformation.
         *
         * @param message The encoded message string to decode.
         * @param PW The password string originally used to encode the message.
         * @return The original message string obtained by reversing the encoding process.
         */
        std::string PWmessageUnCombine(std::string message, std::string PW);
        
    } // namespace internal
} // namespace d0s2

