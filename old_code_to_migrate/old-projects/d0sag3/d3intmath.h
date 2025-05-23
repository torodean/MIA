#ifndef D3INTMATH_H
#define D3INTMATH_H

using namespace std;

class d3intmath
{
public:
    d3intmath();
    bool isPrime(long x);
    bool isPrime(int x);
    bool isPalindrome(int x);
    bool isPalindrome(long x);
    int numberOfFactors(long x);
    int numberOfFactors(int x);
    bool isTriangleNumber(long x);
    bool isTriangleNumber(int x);
    int numOfDigits(long x);
    int numOfDigits(int x);
    int returnLargerInt(int One, int Two);
    int returnLargerInt(long One, long Two);
    int sumOfFactors(int x);
};

#endif // D3INTMATH_H
