//============================================================================
// Name        : D3CEncryptPW.h
// Author      : Antonius Torode
// Copyright   : This file can be used under the conditions of Antonius' 
//				 General Purpose License (AGPL).
// Created on  : October, 2014
// Description : The header file for use with D3CEncryptPW.cpp
//============================================================================
#pragma once

#include <string>
#include <vector>

using std::string;

/**
 * And old class made for some simple encryption of strings with a password.
 */
class D3CEncryptPW
{
public:
    D3CEncryptPW();
    ~D3CEncryptPW();

    /**
     * Main UI runner for the D3CEncryptPW (d0s2) program feature. Encryption
     */
    void d0s2CryptRunner();

    /**
     * Main UI runner for the D3CEncryptPW (d0s2) program feature. DeCryption
     */
    void d0s2DeCryptRunner();

private:
    string EncryptPW(string message, string password);
    string DecryptPW(const string& message, string password);
    std::vector<int> stringToVector(string a);
    string PWRepeat(string PW, int PWR, int PWM);
    string PWmessageCombine(string message, string PW);
    string PWmessageUnCombine(string message, string PW);
    string invertString(string str);

};

