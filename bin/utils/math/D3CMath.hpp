/**
 * @file D3CMath.hpp
 * @author Antonius Torode
 * @date 10/05/2014
 * 
 * Header file for D3CMath.cpp containing declarations for mathematical
 * functions and utilities. Designed to support various numerical operations
 * used throughout the application. This file contains old math methods that 
 * were copied from old programs.
 *
 * A lot of these methods may seem random, but they were from way back in the
 * day when I was doing ProjectEuler puzzles - and are mostly used primarily
 * (at the time) for that.
 */
#pragma once

#include <vector>
#include <string>

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

        std::string returnPrimeNlocation();
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
        bool isPrime(numberType number)
        {
            short Counter = 0;   // A counter for how many numbers can divide evenly into the number.
            if (number == 0 || number == 1 || number == 4)
            {
                return false;
            }
            for (numberType i = 1; i <= ((number + 1) / 2); i++)
            {
                if (Counter < 2)
                {
                    if (number % i == 0)
                    {
                        Counter++;
                    }
                }
                else
                {
                    return false;
                }
            }
            return true;
        }
    private:
        std::string primeNlocation;
    }; // class D3CMath

    /**
     * Calculates the sum of the digits within a number provided as a string.
     * @note Assumes that the input string contains only digit characters ('0'-'9').
     * @param number The string representation of the number.
     * @return The sum of all digits in the number.
     */
    long sumOfDigits(std::string number);

    /**
     * Counts the total number of positive factors of a given number.
     * @param x The number to find factors of.
     * @return The number of factors of x.
     */
    int numberOfFactors(long x);

    /**
     * Checks if a given number is a triangular number.
     * A triangular number is a number that can be represented as the sum of
     * consecutive integers starting from 1.
     * @param x The number to check.
     * @return True if x is a triangular number, false otherwise.
     */
    bool isTriangleNumber(long x);

    [[deprecated("just use x.size()!")]]    
    int numOfDigitsString(const std::string& x);

    /**
     * Returns the number of digits in a non-zero integer.
     * Negative values ignore the negative sign when counting digits.
     * If the input is zero or a string of zeros, the output will be zero.
     * @param x The integer to evaluate.
     * @return The number of digits in the integer.
     */
    int numOfDigitsInt(int x);

    /**
     * Returns the larger of two integers.
     * @param One The first integer.
     * @param Two The second integer.
     * @return The larger integer.
     */
    int setLargerInt(int One, int Two);

    /**
     * Creates a string consisting of zeros of the specified length.
     * @param length The length of the resulting zero string.
     * @return A string composed entirely of '0' characters.
     * @TODO - Move this to StringUtils
     */
    std::string zeroString(int length);

    /**
     * Converts a string of digit characters to a vector of integers.
     * Each character '0'–'9' is converted to its integer equivalent 0–9.
     * @param a Input string of digit characters.
     * @return Vector containing the integer values of each character.
     * @TODO - Move this to StringUtils
     */
    std::vector<int> stringToVector(std::string a);

    /**
     * Converts a vector of integers to a string, removing leading zeros.
     * Leading zeros are shifted to the end before trimming to handle factor adjustment.
     * @param a Vector of integer digits.
     * @return String representation of the vector without leading zeros.
     * @TODO - Move this to StringUtils
     */
    std::string vectorToString(std::vector<int> a);

    /**
     * Resizes the second string to match the length of the first string by prefixing zeros.
     * The first string is the larger one; the second string is padded to match its length.
     * @param a The larger string.
     * @param b The smaller string to be resized.
     * @return The resized string with leading zeros added.
     * @TODO - Move this to StringUtils
     */
    std::string resizeString(const std::string& a, std::string b);

    /**
     * Adds two numeric strings representing non-negative integers and returns their sum as a string.
     * Supports adding two negative numbers but returns an error if only one is negative.
     * 
     * @param stringOne First numeric string.
     * @param stringTwo Second numeric string.
     * @return Sum of the two input strings as a string, with a negative sign if both inputs were negative.
     */
    std::string addTwoStrings(std::string stringOne, std::string stringTwo);

    /**
     * Prepends a negative sign ('-') to the input string.
     * @param Total The original numeric string.
     * @return A new string with a '-' added at the beginning.
     * @TODO - Move this to StringUtils
     */
    std::string addNegativeSign(std::string Total);

    /**
     * Subtracts the second numeric string from the first and returns the result as a string.
     * Handles resizing of inputs and determines the sign of the result.
     * @param stringOne Minuend numeric string.
     * @param stringTwo Subtrahend numeric string.
     * @return Result of the subtraction as a string, including negative sign if needed.
     */
    std::string subtractTwoStrings(std::string stringOne, std::string stringTwo);

    /**
     * Multiplies two numeric strings representing integers of any length.
     * Supports negative numbers by adjusting the sign of the result accordingly.
     * @param stringOne First numeric string.
     * @param stringTwo Second numeric string.
     * @return Product of the two input strings as a string, with correct sign.
     */
    std::string multiplyTwoStrings(std::string stringOne, std::string stringTwo);

    /**
     * Finds the largest product of x consecutive numbers within a numeric string.
     * Converts the string digits to integers, then calculates the product of every consecutive sequence of length x.
     * @param str A string containing the numeric sequence.
     * @param x The number of consecutive digits to multiply.
     * @return The largest product of x consecutive digits as a long integer.
     */
    long largestProductOfXConsecutiveNumbers(std::string string, long x);

    /**
     * Generates the Collatz sequence starting from n and returns the sequence as a space-separated string.
     * Applies the Collatz rules until reaching 1: if even, divide by 2; if odd, multiply by 3 and add 1.
     * @param n The starting positive integer for the Collatz sequence.
     * @return A string containing the sequence numbers separated by spaces (excluding the starting number).
     */
    std::string productCollatzSequence(long n);

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
     * @param a Coefficient of x²
     * @param b Coefficient of x
     * @param c Constant term
     * @return A string representing the roots in the form "(-b*b/(2*a)) +/- sqrt(discriminant)".
     *         For real roots, includes the square root of the discriminant.
     *         For complex roots, includes the imaginary part with '*i'.
     * Note: The returned string does not explicitly separate the two roots; it shows the formula with "+/-".
     */
    std::string solveQuadraticFormula(double a, double b, double c);

    /**
     * Determines if a long number is a palindrome.
     * @param number number to check for palindromeness
     * @return returns true if the input is a palindrome.
     * @note Old function from 2011-ish
     */
    bool isPalindrome(long number);
} // namespace math
