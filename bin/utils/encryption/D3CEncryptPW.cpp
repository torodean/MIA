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

#include "D3CEncryptPW.hpp"
#include "D3CEncrypt.hpp"
// Used for various string manipulations.
#include "StringUtils.hpp"

using std::string;
using std::cout;
using std::endl;

//Main Constructor for the D3CEncryptPW class.
D3CEncryptPW::D3CEncryptPW(){
	//Chuck Norris's beard can type 140 wpm.
}

//Deconstructor for D3CEncryptPW class.
D3CEncryptPW::~D3CEncryptPW() {
    //All arrays Chuck Norris declares are of infinite size, because Chuck Norris knows no bounds.
}

string D3CEncryptPW::PWRepeat(string PW, int PWNeeded, int PWRemainder){
    //Decares needed variables
    string output;
    string temp;
    output = "";
    int counter = 0;

    //sets the output string to the PW repeated as many times as needed to be the same length as the message.
    while (PWNeeded > counter){
        output.append(PW);
        counter++;
    }
    counter = 0;
    while (PWRemainder > counter){
        temp = PW[counter];
        output.append(temp);
        counter ++;
    }

    return output;
}

std::vector<int> messageVectorStringToInt(std::vector<string> messageVector){

    long size = messageVector.size();
    std::vector<int> output;
    output.resize(size);

    for(int i=0;i<size;i++){

        if(messageVector[i] == "1"){
            output[i] = 1;
        }else if(messageVector[i] == "2"){
            output[i] = 2;
        }else if(messageVector[i] == "3"){
            output[i] = 3;
        }else if(messageVector[i] == "4"){
            output[i] = 4;
        }else if(messageVector[i] == "5"){
            output[i] = 5;
        }else if(messageVector[i] == "6"){
            output[i] = 6;
        }else if(messageVector[i] == "7"){
            output[i] = 7;
        }else if(messageVector[i] == "8"){
            output[i] = 8;
        }else if(messageVector[i] == "9"){
            output[i] = 9;
        }else if(messageVector[i] == "0"){
            output[i] = 0;
        }else if(messageVector[i] == "A"){
            output[i] = 10;
        }else if(messageVector[i] == "B"){
            output[i] = 11;
        }else if(messageVector[i] == "C"){
            output[i] = 12;
        }else if(messageVector[i] == "D"){
            output[i] = 13;
        }else if(messageVector[i] == "E"){
            output[i] = 14;
        }
    }

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

std::vector<int> UncombineMandPW(std::vector<int> messageVectorValues, std::vector<int> PWVectorValues){
    std::vector<int> total;
    total.resize(messageVectorValues.size());
    total.reserve(messageVectorValues.size());

    int size = messageVectorValues.size();
    for(int i=0;i<size;i++){
        total[i] = messageVectorValues[i] - PWVectorValues[i];
    }
    return total;
}

string combinedVecTostring(std::vector<int> a){
    string output;
    char temp;
    int size = a.size();
    temp = a[0]+48;
    output = temp;

    for(int i=1;i<size;i++){
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

std::vector<string> normalStringToVector(string input){
    std::vector<string> output;
    int size = input.size();
    output.resize(size);

    for(int i=0;i<size;i++){
        output[i] = input[i];
    }



    return output;
}

string D3CEncryptPW::PWmessageCombine(string message, string PW){
    string output;
    D3CEncrypt crypt;
    std::vector<string> messageVector = normalStringToVector(message), PWVector = normalStringToVector(PW);

    std::vector<int> messageVectorValues = messageVectorStringToInt(messageVector), PWVectorValues = messageVectorStringToInt(PWVector);

    std::vector<int> combinedVector = combineMandPW(messageVectorValues, PWVectorValues);

    output = combinedVecTostring(combinedVector);

    return output;
}

string D3CEncryptPW::PWmessageUnCombine(string message, string PW){
    string output;
    std::vector<string> messageVector = normalStringToVector(message), PWVector = normalStringToVector(PW);

    std::vector<int> messageVectorValues = messageVectorStringToInt(messageVector), PWVectorValues = messageVectorStringToInt(PWVector);

    std::vector<int> combinedVector = UncombineMandPW(messageVectorValues, PWVectorValues);

    output = combinedVecTostring(combinedVector);

    return output;
}


string D3CEncryptPW::EncryptPW(string message, string PW){  
    D3CEncrypt crypt;

    PW = StringUtils::invertString(PW);
	
	//CM stands for crypted message, which is the string that will be returned by the function.
    string CM; 
	
	//creates variables with the length for each string. 
	//PWNeeded is used to determine how many times the password needs to be repeated in order to meet the length fo the message. 
	//PWRemainder determines the remaining letters after the password is used to fill on the log.
    int MLength = message.length(), PWLength = PW.length(), PWNeeded = MLength/PWLength, PWRemainder = MLength % PWLength; 

    //does a regular d0s crypt if the password field is left blank.
    if (PW == "")
        return crypt.Crypt(message, true);

    string PWRepeated = PWRepeat(PW, PWNeeded, PWRemainder);

    //converts the message and PWRepeated strings to a crpyted form of the strings, using the original crypt function.
    message = crypt.Crypt(message, false);
    PW = crypt.CryptNoRand(PWRepeated, false);

    //combines the password with the message.
    CM = PWmessageCombine(message, PW);

	//CM = crypt.squish(CM);
	
    return CM;
}


string D3CEncryptPW::DecryptPW(const string& message, string PW){  	
    D3CEncrypt crypt;
    PW = StringUtils::invertString(PW);
	
	//message = crypt.expand(message);
	cout << message << endl;
	
	//DM stands for decrypted message, which is the string that will be returned by the function.
    string DM; 
	
	//creates variables with the length for each string. 
	//PWNeeded is used to determine how many times the password needs to be repeated in order to meet the length fo the message. 
	//PWRemainder determines the remaining letters after the password is used to fill on the log.
    int MLength = message.length()/9, PWLength = PW.length(), PWNeeded = MLength/PWLength, PWRemainder = MLength % PWLength; 

    //does a regular d0s DeCrypt if the password field is left blank.
    if (PW == "$@d0sag3_EmptyPW.fail_error555$$&**&()"){
        return crypt.DeCrypt(message, true);
    }

    string PWRepeated = PWRepeat(PW, PWNeeded, PWRemainder);

    PW = crypt.CryptNoRand(PWRepeated, false);

    DM = PWmessageUnCombine(message, PW);
    DM = crypt.DeCrypt(DM, false);

    return DM;
}

void D3CEncryptPW::d0s2CryptRunner()
{
    cout << "..." << endl;
    cout << "...Please enter a string to encrypt: " << endl;
    cout << endl;

    string input, password, output;
    getline(std::cin,input);

    cout << "..." << endl;
    cout << "...Please enter the password to encrypt: " << endl;
    cout << endl;

    getline(std::cin,password);
    output = EncryptPW(input, password);

    cout << endl;
    cout << "...The encrypted output is: " << endl;
    cout << endl;
    cout << output << endl;
}

void D3CEncryptPW::d0s2DeCryptRunner()
{
    cout << "..." << endl;
    cout << "...Please enter a string to de-crypt: " << endl;
    cout << endl;

    string input, password, output;
    getline(std::cin,input);

    cout << "..." << endl;
    cout << "...Please enter the password to de-crypt: " << endl;
    cout << endl;

    getline(std::cin,password);
    output = DecryptPW(input, password);

    cout << endl;
    cout << "...The de-crypted output is: " << endl;
    cout << endl;
    cout << output << endl;
}
