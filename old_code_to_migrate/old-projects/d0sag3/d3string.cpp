#include "d3string.h"
#include <string>

using namespace std;

d3string::d3string()
{
}

long d3string::sumOfDigits(string number) { //returns the sum of the digits (as an int) within a number (entered as a string).
    int size = number.size(), sum = 0;
    int n[size];
    for (int i = 0; i < size; i++) {
        n[i] = number[i] - 48;
    }
    for (int q = 0; q < size; q++) {
        sum += n[q];
    }
    return sum;
}

int d3string::numOfDigits(string x) { //returns the number of digits in a string
    return x.size();
}

string d3string::zeroString(int length) { //Creates a string of all zero's of a certain length.
    string Total;
    Total.resize(length);
    for (int i = 0; i < length; i++) { //sets all of the numbers in Total to zero.
        Total[i] = 48; //48 is 0 is string language.
    }
    return Total;
}

int d3string::spacesInAString(string text){
    int count = 0;
    int textLength = text.length();
    for(int i=0;i<textLength;i++){
        if(text[i] == ' '){
            count++;
        }
    }
    return count;
}

