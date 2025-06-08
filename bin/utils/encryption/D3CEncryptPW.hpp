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
    /// Message is the original message and PW is the password used.
    string EncryptPW(string message, string password);
    
    /// Message is the original message and PW is the password used.
    string DecryptPW(const string& message, string password);
    
    string PWRepeat(string PW, int PWR, int PWM);
    string PWmessageCombine(string message, string PW);
    string PWmessageUnCombine(string message, string PW);
};

