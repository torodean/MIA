/**
 * File: D3CMath.hpp
 * Author: Antonius Torode
 * Created on: 10/05/2014
 * Description:
 *     Header file for D3CMath.cpp containing declarations for mathematical
 *     functions and utilities. Designed to support various numerical operations
 *     used throughout the application. This file contains old math methods that 
 *     were copied from old programs.
 *
 *     A lot of these methods may seem random, but they were from way back in the
 *     day when I was doing ProjectEuler puzzles - and are mostly used primarily
 *     (at the time) for that.
 */
#pragma once

#include <vector>
#include <string>

using std::string;
using std::vector;

namespace math
{
    /**
     * This class contains old math methods that were copied from old programs.
     * @TODO - comment and organize this class.
     */
    class D3CMath
    {
    public:
        /// Main constructor for D3CMath class.
        D3CMath();

        /// Deconstructor for D3CMath class.
        ~D3CMath();

        string returnPrimeNlocation();
        void primeFactors(long x);
        long primeNumberN(long n);
        void primeNumberNpopulate(); // populates all prime numbers, this can be used to populate a list of all primes to increase the efficiency of primeNumberN();
        void primeNumberNerase();
    
        //std::string intToBinaryStr(int input, int bits);
        //unsigned long binaryStrToInt(std::string input, int bits);
    
        /**
        * Determines if a number is prime or not.
        * @tparam numberType - the type of parameter to use (int, long, short, etc)
        * @param number[numberType] - The value to check for primeness.
        * @return returns true if the number is prime.
        */
        template<class numberType>
        bool isPrime(numberType number);
    private:
        string primeNlocation;
    }; // class D3CMath

    /**
     * Calculates the sum of the digits within a number provided as a string.
     * @note Assumes that the input string contains only digit characters ('0'-'9').
     * @param number[string] - The string representation of the number.
     * @return [long] - The sum of all digits in the number.
     */
    long sumOfDigits(string number);

    /**
     * Counts the total number of positive factors of a given number.
     * @param x[long] - The number to find factors of.
     * @return [int] - The number of factors of x.
     */
    int numberOfFactors(long x);

    /**
     * Checks if a given number is a triangular number.
     * A triangular number is a number that can be represented as the sum of
     * consecutive integers starting from 1.
     * @param x[long] - The number to check.
     * @return [bool] - True if x is a triangular number, false otherwise.
     */
    bool isTriangleNumber(long x);

    [[deprecated("just use x.size()!")]]    
    int numOfDigitsString(const string& x);

    /**
     * Returns the number of digits in a non-zero integer.
     * Negative values ignore the negative sign when counting digits.
     * If the input is zero or a string of zeros, the output will be zero.
     * @param x[int] - The integer to evaluate.
     * @return [int] - The number of digits in the integer.
     */
    int numOfDigitsInt(int x);

    /**
     * Returns the larger of two integers.
     * @param One[int] - The first integer.
     * @param Two[int] - The second integer.
     * @return [int] - The larger integer.
     */
    int setLargerInt(int One, int Two);

    /**
     * Creates a string consisting of zeros of the specified length.
     * @param length[int] - The length of the resulting zero string.
     * @return [string] - A string composed entirely of '0' characters.
     * @TODO - Move this to StringUtils
     */
    string zeroString(int length);

    /**
     * Converts a string of digit characters to a vector of integers.
     * Each character '0'–'9' is converted to its integer equivalent 0–9.
     * @param a[string] - Input string of digit characters.
     * @return [vector<int>] - Vector containing the integer values of each character.
     * @TODO - Move this to StringUtils
     */
    vector<int> stringToVector(string a);

    /**
     * Converts a vector of integers to a string, removing leading zeros.
     * Leading zeros are shifted to the end before trimming to handle factor adjustment.
     * @param a[vector<int>] - Vector of integer digits.
     * @return [string] - String representation of the vector without leading zeros.
     * @TODO - Move this to StringUtils
     */
    string vectorToString(vector<int> a);

    /**
     * Resizes the second string to match the length of the first string by prefixing zeros.
     * The first string is the larger one; the second string is padded to match its length.
     * @param a[const string&] - The larger string.
     * @param b[string] - The smaller string to be resized.
     * @return [string] - The resized string with leading zeros added.
     * @TODO - Move this to StringUtils
     */
    string resizeString(const string& a, string b);

    /**
     * Adds two numeric strings representing non-negative integers and returns their sum as a string.
     * Supports adding two negative numbers but returns an error if only one is negative.
     * 
     * @param stringOne[string] - First numeric string.
     * @param stringTwo[string] - Second numeric string.
     * @return [string] - Sum of the two input strings as a string, with a negative sign if both inputs were negative.
     */
    string addTwoStrings(string stringOne, string stringTwo);

    /**
     * Prepends a negative sign ('-') to the input string.
     * @param Total[string] - The original numeric string.
     * @return [string] - A new string with a '-' added at the beginning.
     * @TODO - Move this to StringUtils
     */
    string addNegativeSign(string Total);

    /**
     * Subtracts the second numeric string from the first and returns the result as a string.
     * Handles resizing of inputs and determines the sign of the result.
     * @param stringOne[string] - Minuend numeric string.
     * @param stringTwo[string] - Subtrahend numeric string.
     * @return [string] - Result of the subtraction as a string, including negative sign if needed.
     */
    string subtractTwoStrings(string stringOne, string stringTwo);

    /**
     * Multiplies two numeric strings representing integers of any length.
     * Supports negative numbers by adjusting the sign of the result accordingly.
     * @param stringOne[string] - First numeric string.
     * @param stringTwo[string] - Second numeric string.
     * @return [string] - Product of the two input strings as a string, with correct sign.
     */
    string multiplyTwoStrings(string stringOne, string stringTwo);

    /**
     * Finds the largest product of x consecutive numbers within a numeric string.
     * Converts the string digits to integers, then calculates the product of every consecutive sequence of length x.
     * @param str A string containing the numeric sequence.
     * @param x The number of consecutive digits to multiply.
     * @return The largest product of x consecutive digits as a long integer.
     */
    long largestProductOfXConsecutiveNumbers(string string, long x);

    /**
     * Generates the Collatz sequence starting from n and returns the sequence as a space-separated string.
     * Applies the Collatz rules until reaching 1: if even, divide by 2; if odd, multiply by 3 and add 1.
     * @param n The starting positive integer for the Collatz sequence.
     * @return A string containing the sequence numbers separated by spaces (excluding the starting number).
     */
    string productCollatzSequence(long n);

    /**
     * Computes the total number of lattice paths from the top-left to bottom-right corner of an x by y grid.
     * Only right and down moves are allowed; uses dynamic programming to count paths.
     * @param x The grid width in steps.
     * @param y The grid height in steps.
     * @return The number of distinct lattice paths as a long integer.
     */
    long latticePathsOfSquare(long x, long y);

    /**
     * Solves the quadratic equation ax² + bx + c = 0 and returns the roots as a string.
     * @parama[double] a - Coefficient of x²
     * @paramb[double] b - Coefficient of x
     * @paramc[double] c - Constant term
     * @return[string] A string representing the roots in the form "(-b*b/(2*a)) +/- sqrt(discriminant)".
     *                 For real roots, includes the square root of the discriminant.
     *                 For complex roots, includes the imaginary part with '*i'.
     * Note: The returned string does not explicitly separate the two roots; it shows the formula with "+/-".
     */
    string solveQuadraticFormula(double a, double b, double c);

    /**
     * Determines if a long number is a palindrome.
     * @param number[long] - number to check for palindromeness
     * @return returns true if the input is a palindrome.
     * @note Old function from 2011-ish
     */
    bool isPalindrome(long number);
} // namespace math
