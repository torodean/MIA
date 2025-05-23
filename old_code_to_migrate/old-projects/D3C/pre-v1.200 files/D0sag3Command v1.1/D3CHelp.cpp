/*
 * D3CHelp.cpp
 *
 *  Created on: Oct 10, 2014
 *      Author: antonius
 */
#include <iostream>
#include "D3CHelp.h"

namespace std {

D3CHelp::D3CHelp() {
    // TODO Auto-generated constructor stub

}

string D3CHelp::helpRunner(){
    string help =
    "~~ Valid commands are ~~\n"
    "    - add : Adds two positive integers of any length.\n"
    "    - convert : Converts units to other units. (only mass conversions programmed so far)\n"
    "    - crypt : Encrypts any string.\n"
    "    - deCrypt : Decrypts any string.\n"
    "    - help extended : Gives the user a list of extended commands that are not as commonly used.\n"
    "    - kinematics : Solves for unknowns using kinematic equations. (only 1 dimensional programmed)\n"
    "    - mutiply : Multiplies two positive or negative integers of any length.\n"
    "    - subtract : subtracts two positive integers of any length. (Still buggy for some problems)\n"
    "    - exit : Terminates the program.\n";
    return help;
}

string D3CHelp::helpRunnerExtended(){
    string helpExtended =
    "~~ Extended valid extended commands are ~~\n"
    "    - collatz : Produces a collatz sequence based on a starting integer that hte user inputs.\n"
    "    - consecutive digit product : Finds the largest product of x consecutive numbers in a positive integer of any length.\n"
    "    - lattice paths : Determines the amount of lattice paths to the bottom right corner of a square grid of size l x w\n"
    "    - number of factors : Returns the number of factors within an integer.\n"
    "    - palindrome : Determines if a positive integer is palindrome.\n"
    "    - prime : Determines if a positive integer is prime or not.\n"
    "    - prime n : Calculates the n'th prime number up to a maximum number of 2147483647.\n"
    "    - prime n populate : Creates a file of all prime numbers up to a maximum number of 2147483647. Run this file to increase the efficiency of the 'prime n' function greatly.\n"
    "    - prime n erase: Clears the file created by 'prime n populate'.\n"
    "    - prime factors : Determines all of the prime factors of a positive integer.\n"
    "    - sum of digits : Returns the sum of the digits within a number.\n"
    "    - triangle number : Determines if a number is a triangle number or not.\n";
    return helpExtended;
}

D3CHelp::~D3CHelp() {
    // TODO Auto-generated destructor stub
}

} /* namespace std */
