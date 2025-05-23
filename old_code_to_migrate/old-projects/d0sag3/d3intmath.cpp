#include "d3intmath.h"
#include <tgmath.h>

using namespace std;

d3intmath::d3intmath()
{
}
bool d3intmath::isPrime(long x){  //determines if a long is prime or not
    int P = 0;   //P is a counter for how many numbers can divide evenly into x
    if (x == 0 || x == 1 || x == 4) {
        return false;
    }
    for (long i = 1; i <= ((x + 1) / 2); i++) {
        if (P < 2) {
            if (x % i == 0) {
                P++;
            }
        } else {
            return false;
            break;
        }
    }
    return true;
}

bool d3intmath::isPrime(int x){  //determines if an int is prime or not
    int P = 0;   //P is a counter for how many numbers can divide evenly into x
    if (x == 0 || x == 1 || x == 4) {
        return false;
    }
    for (int i = 1; i <= ((x + 1) / 2); i++) {
        if (P < 2) {
            if (x % i == 0) {
                P++;
            }
        } else {
            return false;
            break;
        }
    }
    return true;
}

bool d3intmath::isPalindrome(int number) {  //determines if an int is a palindrome.
    int size = log10(number) + 1;
    int n[size];
    for (int i = 0; i < size; i++) {
        int temp = pow(10, size - 1 - i);
        n[i] = number / (temp) % 10;
    }
    for (int q = 0; q < size; q++) {
        if (n[q] == n[size - 1 - q]) {
        } else {
            return false;
            break;
        }
    }
    return true;
}

bool d3intmath::isPalindrome(long number) {  //determines if an int is a palindrome.
    long size = log10(number) + 1;
    long n[size];
    for (long i = 0; i < size; i++) {
        long temp = pow(10, size - 1 - i);
        n[i] = number / (temp) % 10;
    }
    for (long q = 0; q < size; q++) {
        if (n[q] == n[size - 1 - q]) {
        } else {
            return false;
            break;
        }
    }
    return true;
}

int d3intmath::numberOfFactors(long x) { //returns the number of factors in a number
    int num = 0;
    for (long i = 1; i <= x; i++) {
        if (x % i == 0) {
            num++;
        }
    }
    return num;
}

int d3intmath::numberOfFactors(int x) { //returns the number of factors in a number
    int num = 0;
    for (int i = 1; i <= x; i++) {
        if (x % i == 0) {
            num++;
        }
    }
    return num;
}

bool d3intmath::isTriangleNumber(long x) {
    long num = 0;
    for (long i = 1; i < x; i++) {
        num += i;
        if (x == num) {
            return true;
            break;
        }
    }
    return false;
}

bool d3intmath::isTriangleNumber(int x) {
    int num = 0;
    for (int i = 1; i < x; i++) {
        num += i;
        if (x == num) {
            return true;
            break;
        }
    }
    return false;
}

int d3intmath::numOfDigits(int x){ //returns the number of digits within an integer.
    int count;
    if(x!=0){
        count = log10(x)+1;
    } else{
        count=0;
    }
    return count;
}

int d3intmath::numOfDigits(long x){ //returns the number of digits within an integer.
    int count;
    if(x!=0){
        count = log10(x)+1;
    } else{
        count=0;
    }
    return count;
}

int d3intmath::returnLargerInt(int One, int Two) {
    int largerInt;
    if (One >= Two) { //sets the variable of largerInt to the length of the larger of the 2 ints.
        largerInt = One;
    } else {
        largerInt = Two;
    }
    return largerInt;
}

int d3intmath::returnLargerInt(long One, long Two) {
    long largerInt;
    if (One >= Two) { //sets the variable of largerInt to the length of the larger of the 2 ints.
        largerInt = One;
    } else {
        largerInt = Two;
    }
    return largerInt;
}

int d3intmath::sumOfFactors(int x){
    int total = 0;
    for(int i=1;i<x;i++){
        if(x%i == 0){
            total += i;
            //cout << i << " ";
        }
    }
   // cout << endl;
    return total;
}
