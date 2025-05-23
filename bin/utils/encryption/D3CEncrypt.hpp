//============================================================================
// Name        : D3CEncrypt.hpp
// Author      : Antonius Torode
// Copyright   : This file can be used under the conditions of Antonius' 
//				 General Purpose License (AGPL).
// Created on  : September 24, 2014
// Description : Header file for the D3CEncrypt.cpp file.
//============================================================================
#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

/**
 * And old class made for some simple encryption of strings.
 */
class D3CEncrypt
{
public:

    D3CEncrypt();
    ~D3CEncrypt();

    /**
     * Main UI runner for the D3CEncrypt (d0s1) program feature. Encryption
     */
    void d0s1CryptRunner();

    /**
     * Main UI runner for the D3CEncrypt (d0s1) program feature. DeCryption
     */
    void d0s1DeCryptRunner();


    string Crypt(const string& input, bool toSquish);

    string DeCrypt(string input, bool toSquish);

    string CryptNoRand(string input, bool toSquish);

private:

    //Functions from original program.
    vector<int> stringToVector(string a);
    vector<int> binaryVector(vector<int> inputVector);
    int random7bit();
    int numberToBinary(int num);
    string a11b12(int x);
    string stringVectorToString(vector<string> a);
    string cryptChars(vector<int> inputVector);
    string cryptCharsNoRand(vector<int> inputVector);

    vector<string> cryptedStringToVector(string a);
    vector<string> seperateRandom(vector<string> input);
    vector<int> vectorStringToInt(vector<string> a);
    vector<int> seperateBinary(vector<string> input);
    int a11b12toReg(string a11b12);
    vector<int> converta11b12vecToReg(vector<string> a);
    vector<int> numberVectorToBinaryVector(vector<int> a);
    vector<int> DeCryptChars(vector<int> a, vector<int> b);
    vector<int> binaryVectorToASCII(vector<int> a);
    string intVectorToString(vector<int> a);

    //Added functions after MIA implimentation.
    string squish(string input);
    string expand(string input);

    int stringLength,
            vectorSize,
            digit,
            powerofDigit,
            powerofTen,
            devise,
            random7bitNum,
            random7bitbinary,
            encodedVec;

};

