/**
 * File: D3CMath.hpp
 * Author: Antonius Torode
 * Created on: 10/05/2014
 * Description:
 *     Header file for D3CMath.cpp containing declarations for mathematical
 *     functions and utilities. Designed to support various numerical operations
 *     used throughout the application. This file contains old math methods that 
 *     were copied from old programs.
 */

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
    private:
        string primeNlocation;
    public:
        D3CMath();
        string returnPrimeNlocation();
        void primeFactors(long x);
        long primeNumberN(long n);
        void primeNumberNpopulate(); // populates all prime numbers, this can be used to populate a list of all primes to increase the efficiency of primeNumberN();
        void primeNumberNerase();
    
        //std::string intToBinaryStr(int input, int bits);
        //unsigned long binaryStrToInt(std::string input, int bits);
        ~D3CMath();
    
        /**
        * Determines if a number is prime or not.
        * @tparam numberType - the type of parameter to use (int, long, short, etc)
        * @param number[numberType] - The value to check for primeness.
        * @return returns true if the number is prime.
        */
        template<class numberType>
        bool isPrime(numberType number);
    }; // class D3CMath

    static long sumOfDigits(string number);
    static int numberOfFactors(long x);
    static bool isTriangleNumber(long x);
    static int numOfDigitsString(const string& x);
    static int numOfDigitsInt(int x);
    static int setLargerInt(int One, int Two);
    static string zeroString(int length);
    static vector<int> stringToVector(string a);
    static string vectorToString(vector<int> a);
    static string resizeString(const string& a, string b);
    static string addTwoStrings(string stringOne, string stringTwo);
    static string addNegativeSign(string Total);
    static string subtractTwoStrings(string stringOne, string stringTwo);
    static string multiplyTwoStrings(string stringOne, string stringTwo);
    static long largestProductOfXConsecutiveNumbers(string string, long x);
    static string productCollatzSequence(long n);
    static long latticePathsOfSquare(long x, long y);
    static string solveQuadraticFormula(double a, double b, double c);

    /**
     * etermines if a long number is a palindrome.
     * @param number[long] - number to check for palindromeness
     * @return returns true if the input is a palindrome.
     * Old function from 2011-ish
     */
    static bool isPalindrome(long number);
} // namespace math
