#include "d3stringmath.h"
#include <vector>
#include "d3string.h"
#include "d3intmath.h"

using namespace std;

d3stringmath::d3stringmath()
{
}

long d3stringmath::sumOfDigits(string number) { //returns the sum of the digits (as an int) within a number (entered as a string).
    long size = number.size(), sum = 0;
    long n[size];
    for (long i = 0; i < size; i++) {
        n[i] = number[i] - 48;
    }
    for (long q = 0; q < size; q++) {
        sum += n[q];
    }
    return sum;
}

vector<int> d3stringmath::stringToVector(string a) { //Converts a string of ints to a vector of ints
    int length = a.size();
    vector<int> vector(length);
    for (int i = 0; i < length; i++) { //converts string one into an array of integers.
        vector[i] = a[i] - 48;
    }
    return vector;
}

string d3stringmath::vectorToStringZeroTrim(vector<int> a) { //converts a vector to a string and removes the preceeding zeros.

    d3string dstring;
    bool lastDigitZero = false;
    int lengthTotal = a.size(), count = 0;
    string Total = dstring.zeroString(lengthTotal);
    while (a[0] == 0) { //if the first number is a 0, this will place it at the end instead of the beginning, in term multiplying the number by 10. The increase by a factor of 10 will be corrected once the number is converted back to a string and the string is down sized one digit (the last digit).
        for (int i = 0; i < lengthTotal - 1; i++) {
            a[i] = a[i + 1];
        }
        a[lengthTotal - 1] = 0;
        lastDigitZero = true;
        count++;
    }
    for (int i = 0; i < lengthTotal; i++) {
        Total[i] = a[i] + 48;
    }
    while (lastDigitZero == true && count != 0) { //removes the last digit if it is zero, but only if it was moved from the front to the back, to correct for the increase in factor of 10 that was produced earlier.
        Total.resize(lengthTotal - 1);
        lengthTotal = Total.size();
        if (Total[lengthTotal - 1] == 48) {
            lastDigitZero = true;
        } else {
            lastDigitZero = false;
        }
        count--;
    }
    return Total;
}

string d3stringmath::resizeString(string a, string b) { //resizes a smaller string to be the same as a second string by placing zeroe's in front of it until it is the same length as the second string. The larger string is inputted first (a) and the smaller string to be resized is inputted as teh second string (b).
    int lengthOne = a.size(), lengthTwo = b.size(), origlengthTwo = lengthTwo,
            temp = 0;
    int difference = lengthOne - lengthTwo;
    b.resize(lengthTwo + difference);
    for (int i = lengthOne; i >= 0; i--) {
        if (i > difference - 1) {
            b[i] = b[origlengthTwo - temp];
        } else {
            b[i] = 48;
        }
        temp++;
    }
    return b;
}

string d3stringmath::addTwoStrings(string stringOne, string stringTwo) { //adds 2 strings of integers together and returns their sum as a string.
    d3intmath dmath;
    d3string dstring;

    int lengthOne = stringOne.size(), lengthTwo = stringTwo.size(),
            largerLength = dmath.returnLargerInt(lengthOne, lengthTwo), lengthTotal,
            number, var; //largerLength is now the length of the largest string.
    //bool lastDigitZero = false;

    string Total = dstring.zeroString(largerLength + 1); //creates a blank string for the total of the two added strings.
    lengthTotal = Total.size();

    if (lengthOne == lengthTwo) {
        ; //if the lengths of the starting two strings are equal in length, do nothing, otherwise set the lengths equal to each other and make the empty digits before the smaller number 0.
    } else if (lengthOne > lengthTwo) {
        stringTwo = resizeString(stringOne, stringTwo);
        lengthTwo = stringTwo.size();
    } else if (lengthTwo > lengthOne) {
        stringOne = resizeString(stringTwo, stringOne);
        lengthOne = stringOne.size();
    }

    vector<int> TotalArray = stringToVector(Total), stringOneArray =
            stringToVector(stringOne), stringTwoArray = stringToVector(
            stringTwo); //creates an int array for each string to be converted to and then converts the, to an array of integers using a vector.

    for (int i = 1; i <= lengthTotal - 1; i++) { //Adds the two string arrays together and combine them into the total array.
        number = stringOneArray[lengthOne - i] + stringTwoArray[lengthTwo - i];
        TotalArray[lengthTotal - i] += number % 10;
        TotalArray[lengthTotal - 1 - i] += number / 10;
    }
    var = lengthTotal - 1;
    while (var > 0) { //in some cases, if the two positions added up to 9, then were added to an existing one in the total array, the number was 10. So this corrects for that.
        if (TotalArray[var] == 10) {
            TotalArray[var] = 0;
            TotalArray[var - 1] += 1;
        }
        var--;
    }
    Total = vectorToStringZeroTrim(TotalArray);
    return Total; //returns the Total of the two strings.
}

string d3stringmath::multiplyTwoStrings(string stringOne, string stringTwo) {
    d3string dstring;
    d3intmath dmath;

    if(stringOne == "0" || stringTwo == "0"){
        return "0";
    }
    int lengthOne = stringOne.size(), lengthTwo = stringTwo.size(),
            largerLength = dmath.returnLargerInt(lengthOne, lengthTwo), lengthTotal, difference, origLengthOne = lengthOne, origLengthTwo = lengthTwo, number, temp;
    string Total = dstring.zeroString(largerLength *2); //creates a blank string for the total of the two added strings.
    lengthTotal = Total.size();

    if (lengthOne == lengthTwo) {
        difference=0; //if the lengths of the starting two strings are equal in length, do nothing, otherwise set the lengths equal to each other and make the empty digits before the smaller number 0.
    } else if (lengthOne > lengthTwo) {
        difference = lengthOne-lengthTwo;
        stringTwo = resizeString(stringOne, stringTwo);
        lengthTwo = stringTwo.size();
    } else if (lengthTwo > lengthOne) {
        difference = lengthTwo - lengthOne;
        stringOne = resizeString(stringTwo, stringOne);
        lengthOne = stringOne.size();
    }

    vector<int> TotalArray = stringToVector(Total), stringOneArray =
            stringToVector(stringOne), stringTwoArray = stringToVector(
            stringTwo);

    if (origLengthOne > origLengthTwo) { ///multiplies the 2 vectors together and combines them in the TotalArray. Each position in the total vector can have multiple digits, which will be fixed later.
        for (int i = 0; i < largerLength; i++) {
            for (int j = 0; j < largerLength - difference; j++) {
                TotalArray[lengthTotal - 1 - i - j] += stringOneArray[lengthOne-1-i]
                        * stringTwoArray[lengthTwo-1-j];
            }
        }
    } else {
        for (int i = 0; i < largerLength - difference; i++) {
            for (int j = 0; j < largerLength; j++) {
                TotalArray[lengthTotal - 1 - i - j] += stringOneArray[lengthOne-1-i]
                        * stringTwoArray[lengthTwo-1-j];
            }
        }
    }

    for(int i=0;i<lengthTotal;i++){
        number=TotalArray[lengthTotal-1-i];
        temp = dmath.numOfDigits(number);
        TotalArray[lengthTotal-1-i]=0;
        for(int j=0;j<temp;j++){
            TotalArray[lengthTotal-1-j-i] += number%10;
            number=number/10;
        }

    }

    Total = vectorToStringZeroTrim(TotalArray);
    return Total;
}











