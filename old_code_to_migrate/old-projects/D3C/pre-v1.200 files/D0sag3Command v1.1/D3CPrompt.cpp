//============================================================================
// Name        : D0sag3.cpp
// Author      : Antonius Torode
// Version     : 1.005
// Copyright   : Copywrite Antonius Torode 2014
// Description : D0sag3 Command in C++, Ansi-style
//============================================================================

#include <iostream>
#include "D3CPrompt.h"

using namespace std;

D3CPrompt::D3CPrompt(){
    // TODO Auto-generated constructor stub
}

D3CPrompt::~D3CPrompt(){

}

int D3CPrompt::caseDecider(string function){
    int variable;
    if(function == "crypt" || function =="Crypt"){
        variable = 1;
    }else if(function == "decrypt" || function == "Decrypt" || function == "DeCrypt"){
        variable = 2;
    }else if(function == "multiply" || function == "Multiply"){
        variable = 3;
    }else if(function == "add" || function == "Add"){
        variable = 4;
    }else if(function == "exit" || function == "Exit"){
        variable = 5;
    }else if(function == "help" || function == "Help"){
        variable = 6;
    }else if(function == "kinematics" || function == "Kinematics"){
        variable = 7;
    }else if(function == "convert" || function == "Convert"){
        variable = 8;
    }else if(function == "subtract" || function == "Subtract" || function == "sub"){
        variable = 9;
    }else if(function == "prime" || function == "Prime"){
        variable = 10;
    }else if(function == "help extended" || function == "Help extended"){
        variable = 11;
    }else if(function == "palindrome" || function == "Palindrome"){
        variable = 12;
    }else if(function == "sum of digits" || function == "Sum of digits"){
        variable = 13;
    }else if(function == "number of factors" || function == "Number of factors"){
        variable = 14;
    }else if(function == "triangle number" || function == "Triangle number"){
        variable = 15;
    }else if(function == "prime n" || function == "Prime n"){
        variable = 16;
    }else if(function == "prime n populate" || function == "Prime n populate"){
        variable = 17;
    }else if(function == "prime n erase" || function == "Prime n erase"){
        variable = 18;
    }else if(function == "prime factors" || function == "Prime factors"){
        variable = 19;
    }else if(function == "consecutive digit product" || function == "Consecutive digit product"){
        variable = 20;
    }else if(function == "collatz" || function == "Collatz"){
        variable = 21;
    }else if(function == "lattice paths" || function == "Lattice paths"){
        variable = 22;
    }


    else{
        variable = 0;
    }
    return variable;
}
