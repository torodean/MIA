//============================================================================
// Name        : D3CEncryptPW.cpp
// Author      : Antonius Torode
// Copyright   : This file can be used under the conditions of Antonius' 
//				 General Purpose License (AGPL).
// Created on  : October, 2014
// Description : A old encryption program (d0s2) re-adapted for use with MIA.
//============================================================================

#include <string>
#include <cmath>
#include <iostream>
#include <unordered_map>

#include "D3CEncryptPW.hpp"
#include "D3CEncrypt.hpp"
// Used for various string manipulations.
#include "StringUtils.hpp"


namespace d0s2
{
    namespace internal
    {
        std::string PWRepeat(std::string PW, int PWNeeded, int PWRemainder)
        {
            // Decares needed variables
            std::string output;
            std::string temp;
            output = "";
            int counter = 0;

            // Sets the output string to the PW repeated as many times as needed to be the same length as the message.
            while (PWNeeded > counter)
            {
                output.append(PW);
                counter++;
            }
            counter = 0;
            while (PWRemainder > counter)
            {
                temp = PW[counter];
                output.append(temp);
                counter ++;
            }

            return output;
        }
        
        
        // TODO - Figure out why this only supports up to 14 (this is very old code).
        std::vector<int> messageVectorStringToInt(std::vector<std::string> messageVector)
        {

            long size = messageVector.size();
            std::vector<int> output;
            output.resize(size);

            std::unordered_map<std::string, int> mapping = {
                {"0", 0}, {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4},
                {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9},
                {"A", 10}, {"B", 11}, {"C", 12}, {"D", 13}, {"E", 14}
            };

            for (int i = 0; i < size; ++i) 
                if (mapping.find(messageVector[i]) != mapping.end()) 
                    output[i] = mapping[messageVector[i]];

            return output;
        }


        std::vector<int> combineMandPW(std::vector<int> messageVectorValues, std::vector<int> PWVectorValues)
        {
            std::vector<int> total;
            int size = messageVectorValues.size();
            total.resize(size);
            total.reserve(size);
            
            for(int i=0; i<size; i++)
                total[i] = messageVectorValues[i] + PWVectorValues[i];
                
            return total;
        }


        std::vector<int> UncombineMandPW(std::vector<int> messageVectorValues, std::vector<int> PWVectorValues)
        {
            std::vector<int> total;
            total.resize(messageVectorValues.size());
            total.reserve(messageVectorValues.size());

            int size = messageVectorValues.size();
            for(int i=0;i<size;i++)
                total[i] = messageVectorValues[i] - PWVectorValues[i];
                
            return total;
        }


        // TODO - Figure out why this only supports up to 'P' (this is very old code).
        std::string combinedVecTostring(std::vector<int> a)
        {
            std::string output;
            int size = a.size();
            char temp = a[0]+48;
            output = temp;

            for(int i=1;i<size;i++)
            {
                if(a[i] == 0){
                    output += '0';
                }else if(a[i] == 1){
                    output += '1';
                }else if(a[i] == 2){
                    output += '2';
                }else if(a[i] == 3){
                    output += '3';
                }else if(a[i] == 4){
                    output += '4';
                }else if(a[i] == 5){
                    output += '5';
                }else if(a[i] == 6){
                    output += '6';
                }else if(a[i] == 7){
                    output += '7';
                }else if(a[i] == 8){
                    output += '8';
                }else if(a[i] == 9){
                    output += '9';
                }else if(a[i] == 10){
                    output += 'A';
                }else if(a[i] == 11){
                    output += 'B';
                }else if(a[i] == 12){
                    output += 'C';
                }else if(a[i] == 13){
                    output += 'D';
                }else if(a[i] == 14){
                    output += 'E';
                }else if(a[i] == 15){
                    output += 'F';
                }else if(a[i] == 16){
                    output += 'G';
                }else if(a[i] == 17){
                    output += 'H';
                }else if(a[i] == 18){
                    output += 'I';
                }else if(a[i] == 19){
                    output += 'J';
                }else if(a[i] == 20){
                    output += 'K';
                }else if(a[i] == 21){
                    output += 'L';
                }else if(a[i] == 22){
                    output += 'M';
                }else if(a[i] == 23){
                    output += 'N';
                }else if(a[i] == 24){
                    output += 'O';
                }else if(a[i] == 25){
                    output += 'P';
                }
            }

            return output;
        }
        

        std::vector<std::string> normalStringToVector(std::string input)
        {
            std::vector<std::string> output;
            int size = input.size();
            output.resize(size);

            for(int i=0;i<size;i++)
                output[i] = input[i];

            return output;
        }
        

        std::string PWmessageCombine(std::string message, std::string PW)
        {
            std::vector<std::string> messageVector = normalStringToVector(message);
            std::vector<std::string> PWVector = normalStringToVector(PW);
            std::vector<int> messageVectorValues = messageVectorStringToInt(messageVector);
            std::vector<int> PWVectorValues = messageVectorStringToInt(PWVector);
            std::vector<int> combinedVector = combineMandPW(messageVectorValues, PWVectorValues);
            std::string output = combinedVecTostring(combinedVector);

            return output;
        }

        std::string PWmessageUnCombine(std::string message, std::string PW)
        {
            std::vector<std::string> messageVector = normalStringToVector(message);
            std::vector<std::string> PWVector = normalStringToVector(PW);
            std::vector<int> messageVectorValues = messageVectorStringToInt(messageVector);
            std::vector<int> PWVectorValues = messageVectorStringToInt(PWVector);
            std::vector<int> combinedVector = UncombineMandPW(messageVectorValues, PWVectorValues);
            std::string output = combinedVecTostring(combinedVector);

            return output;
        }
    } // namespace internal


    std::string EncryptPW(std::string message, std::string PW)
    {
        PW = StringUtils::invertString(PW);
	    
	    //CM stands for crypted message, which is the string that will be returned by the function.
        std::string CM; 
	    
	    //creates variables with the length for each string. 
	    //PWNeeded is used to determine how many times the password needs to be repeated in order to meet the length fo the message. 
	    //PWRemainder determines the remaining letters after the password is used to fill on the log.
        int MLength = message.length(), PWLength = PW.length(), PWNeeded = MLength/PWLength, PWRemainder = MLength % PWLength; 

        //does a regular d0s crypt if the password field is left blank.
        if (PW == "")
            return d0s1::Crypt(message, true);

        std::string PWRepeated = internal::PWRepeat(PW, PWNeeded, PWRemainder);

        //converts the message and PWRepeated strings to a crpyted form of the strings, using the original crypt function.
        message = d0s1::Crypt(message, false);
        PW = d0s1::CryptNoRand(PWRepeated, false);

        //combines the password with the message.
        CM = internal::PWmessageCombine(message, PW);

	    //CM = d0s1::squish(CM);
	    
        return CM;
    }


    std::string DecryptPW(const std::string& message, std::string PW){  	
        PW = StringUtils::invertString(PW);
	    
	    //message = d0s1::expand(message);
	    std::cout << message << std::endl;
	    
	    //DM stands for decrypted message, which is the string that will be returned by the function.
        std::string DM; 
	    
	    //creates variables with the length for each string. 
	    //PWNeeded is used to determine how many times the password needs to be repeated in order to meet the length fo the message. 
	    //PWRemainder determines the remaining letters after the password is used to fill on the log.
        int MLength = message.length()/9, PWLength = PW.length(), PWNeeded = MLength/PWLength, PWRemainder = MLength % PWLength; 

        //does a regular d0s DeCrypt if the password field is left blank.
        if (PW == "$@d0sag3_EmptyPW.fail_error555$$&**&()"){
            return d0s1::DeCrypt(message, true);
        }

        std::string PWRepeated = internal::PWRepeat(PW, PWNeeded, PWRemainder);

        PW = d0s1::CryptNoRand(PWRepeated, false);

        DM = internal::PWmessageUnCombine(message, PW);
        DM = d0s1::DeCrypt(DM, false);

        return DM;
    }


    void d0s2CryptRunner()
    {
        std::cout << "..." << std::endl;
        std::cout << "...Please enter a string to encrypt: " << std::endl;
        std::cout << std::endl;

        std::string input, password, output;
        getline(std::cin,input);

        std::cout << "..." << std::endl;
        std::cout << "...Please enter the password to encrypt: " << std::endl;
        std::cout << std::endl;

        getline(std::cin,password);
        output = EncryptPW(input, password);

        std::cout << std::endl;
        std::cout << "...The encrypted output is: " << std::endl;
        std::cout << std::endl;
        std::cout << output << std::endl;
    }


    void d0s2DeCryptRunner()
    {
        std::cout << "..." << std::endl;
        std::cout << "...Please enter a string to de-crypt: " << std::endl;
        std::cout << std::endl;

        std::string input, password, output;
        getline(std::cin,input);

        std::cout << "..." << std::endl;
        std::cout << "...Please enter the password to de-crypt: " << std::endl;
        std::cout << std::endl;

        getline(std::cin,password);
        output = DecryptPW(input, password);

        std::cout << std::endl;
        std::cout << "...The de-crypted output is: " << std::endl;
        std::cout << std::endl;
        std::cout << output << std::endl;
    }
} // namespace d0s2
