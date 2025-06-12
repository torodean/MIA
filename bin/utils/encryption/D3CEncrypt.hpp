//============================================================================
// Name        : D3CEncrypt.hpp
// Author      : Antonius Torode
// Copyright   : This file can be used under the conditions of Antonius' 
//				 General Purpose License (AGPL).
// Created on  : September 24, 2014
// Description : Header file for the D3CEncrypt.cpp file.
//               This file is very old. Many of the comments don't make much 
//               sense or are non-existant. I have kept it mostly in its
//               original form with only a few minor improvements since then.
//               Most of the improvements involve moving utility methods outside of
//               this class, refactoring, and adding comments.
//============================================================================
#pragma once

#include <vector>
#include <string>


/**
 * The name of this encryption algorithm was d0s1, which originally lived in the 
 * D3CEncrypt class. Since there weren't really any class-specific features, it
 * was decided to move these features to a namespace instead of a class.
 */
namespace d0s1
{
    /**
     * Main UI runner for the D3CEncrypt (d0s1) program feature. Encryption
     */
    void d0s1CryptRunner();

    /**
     * Main UI runner for the D3CEncrypt (d0s1) program feature. DeCryption
     */
    void d0s1DeCryptRunner();

    /// Encrypts a string.
    std::string Crypt(const std::string& input, bool toSquish);

    /// Decrypts a string encrypted with D3Crypt.
    std::string DeCrypt(std::string input, bool toSquish);

    /// Encrypts a string.
    std::string CryptNoRand(std::string input, bool toSquish);
    
    /**
     * This namespace is for code which is internal to this encryption algorithm.
     */
    namespace internal
    {  
        //Functions from original program.
        
        /// Converts each term in the vector array to a binary representation of that term.
        std::vector<int> binaryVector(std::vector<int> inputVector);
        
        /// Creates a random number between 0-127.
        int random7bit();
        
        /**
         * Changes a number from base 10 format to a binary look but still in base 
         * 10 so that addition in base ten can be used on the binary number.
         */
        int numberToBinary(int num);
        
        /**
         * Converts an integer to a custom a11b12 formatted number. 
         * the first digit is in base 11 and the second in base 12.
         */
        std::string a11b12(int x);
        
        /// Converts a vector<string> (each position holding a string of length 9) to one combined string.
        std::string stringVectorToString(std::vector<std::string> a);
        
        /// Encrypts the characters of a vector array with a random number.
        std::string cryptChars(std::vector<int> inputVector);
        std::string cryptCharsNoRand(std::vector<int> inputVector);

        /**
         * Converts a string after being encrypted using crypt() to a vector seperating each 
         * character into it's own position.
         */
        std::vector<std::string> cryptedStringToVector(std::string a);
        std::vector<std::string> seperateRandom(std::vector<std::string> input);
        std::vector<int> vectorStringToInt(std::vector<std::string> a);
        std::vector<int> seperateBinary(std::vector<std::string> input);
        int a11b12toReg(std::string a11b12);
        std::vector<int> converta11b12vecToReg(std::vector<std::string> a);
        std::vector<int> numberVectorToBinaryVector(std::vector<int> a);
        std::vector<int> DeCryptChars(std::vector<int> a, std::vector<int> b);
        
        /**
         * @brief Converts a vector of decimal-encoded 7-bit binary integers to ASCII codes.
         *
         * Each integer in inputVec encodes a 7-bit binary number as a decimal number
         * (e.g., 1000001 represents binary "1000001"). This function decodes each integer
         * into its ASCII integer equivalent.
         *
         * @param inputVec[const vector<int>&] - Vector of decimal-encoded 7-bit binary integers.
         * @return [std::vector<int>] - Vector of decoded ASCII integer codes.
         */
        std::vector<int> binaryVecToASCIIVec(const std::vector<int>& inputVec);

        // Added functions after MIA implimentation.
        
        /// Shortens the encrypted message created by the d0s1 encryption.
        std::string squish(std::string input);
        /// Expands the encrypted message created by the d0s1 encryption.
        std::string expand(std::string input);
    } // namespace internal
} // namespace d0s1
