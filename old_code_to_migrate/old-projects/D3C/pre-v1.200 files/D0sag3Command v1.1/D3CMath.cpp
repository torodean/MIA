/*
 * D3CMath.cpp
 *
 *  Created on: Oct 5, 2014
 *      Author: Antonius Torode
 */

#include <vector>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include "D3CMath.h"
#include "d3cwindow.h"

namespace std {

D3CMath::D3CMath() {
    // TODO Auto-generated constructor stub
}

//determines if a number is prime or not
bool D3CMath::isPrime(long number) {  
    int Counter = 0;   //P is a counter for how many numbers can divide evenly into x
    if (number == 0 || number == 1 || number == 4) {
        return false;
    }
    for (long i = 1; i <= ((number + 1) / 2); i++) {
        if (Counter < 2) {
            if (number % i == 0) {
                Counter++;
            }
        } else {
            return false;
            break;
        }
    }
    return true;
}

//determines if a number is a palindrome.
bool D3CMath::isPalindrome(long number) {  
    int size = log10(number) + 1;
    int n[size];
    for (int i = 0; i < size; i++) {
        long temp = pow(10, size - 1 - i);
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

//returns the sum of the digits (as an int) within a number (entered as a string).
long D3CMath::sumOfDigits(string number) { 
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

//returns the number of factors in a number
int D3CMath::numberOfFactors(long x) { 
    int num = 0;
    for (long i = 1; i <= x; i++) {
        if (x % i == 0) {
            num++;
        }
    }
    return num;
}

bool D3CMath::isTriangleNumber(long x) {
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

//returns the number of digits in a string
int D3CMath::numOfDigitsString(string x) { 
    return x.size();
}

istream& ignoreline(ifstream& in, ifstream::pos_type& pos){
    pos = in.tellg();
    return in.ignore(numeric_limits<streamsize>::max(), '\n');
}

string getLastLine(ifstream& in){
    ifstream::pos_type pos = in.tellg();

    ifstream::pos_type lastPos;
    while (in >> ws && ignoreline(in, lastPos))
        pos = lastPos;

    in.clear();
    in.seekg(pos);

    string line;
    getline(in, line);
    return line;
}

void D3CMath::primeFactors(long x){
   // cout << "The prime factors are: " << endl;
    for (long i = 1; i <= (x + 1) / 2; i++) {
            if (isPrime(i) == true && x % i == 0) {
                //cout << i << endl;
            }
        }
}

long stringToLong(string input){
//	cout << "input: " << input << endl;
    long output=0;
    int size = input.size();
    int temp[size];
//	cout << "temp[i]: ";
    for(int i=0;i < size;i++){
        temp[i] = input[i]-48;
//		cout << temp[i];
        output += temp[i]*pow(10, size-i-1);
    }
//	cout << endl;
    return output;
}

long seperateFileStart(string line){
    string lineStartString;
    long lineStart;
    int lineLength = line.size();
    int position=0;
    for(int i=0;i<lineLength;i++){
        if(line[i] == ':'){
            position = i;
            break;
        }
    }
    lineStartString.resize(position-1);
    lineStartString.reserve(position-1);
    for(int x=0;x<position-1;x++){
        lineStartString[x] = line[x];
    }
    lineStart = stringToLong(lineStartString);

    return lineStart;
}

long seperateFileEnd(string line){
    string lineEndString;
    long lineEnd;
    int lineLength = line.size();
    int position=0;
    for(int i=0;i<lineLength;i++){
        if(line[i] == ':'){
            position = i;
            break;
        }
    }
    lineEndString.resize(lineLength - position-2);
    lineEndString.reserve(lineLength - position-2);

//	cout << "lineEndString[x]: ";
    for(int x=0;x<lineLength-(position+2);x++){
        lineEndString[x] = line[x+position+2];
//		cout << lineEndString[x];
    }
//	cout << endl << lineEndString << endl;;
    lineEnd = stringToLong(lineEndString);
//	cout << "lineEnd: " << lineEnd << endl;
    return lineEnd;
}

//populates all prime numbers, this can be used to populate a list of all primes 
//to increase the efficiency of primeNumberN(); Up to a maximum number of 2147483647.
void D3CMath::primeNumberNpopulate() { 

    D3CWindow window;
    ofstream myfile;
    string line;
    ifstream myfilein("../build-D0sag3Command-Desktop_Qt_5_3_MinGW_32bit-Release/PrimeNumberList.txt");
    long max = 2147483647;
    int percent;

    if (myfilein.is_open()) {
        line = getLastLine(myfilein);
    }
    myfilein.close();

    myfile.open("../build-D0sag3Command-Desktop_Qt_5_3_MinGW_32bit-Release/PrimeNumberList.txt", ios::app);

    if (line.empty()) {
        myfile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        myfile << "~~ List of Prime Numbers ~~" << endl;
        myfile << "~~~~~~~ DO NOT EDIT ~~~~~~~" << endl;
        myfile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;

        long fileStart = 2;
        long primeCount = 0;
        for (long i = fileStart; i <= max; i++) {
            if (isPrime(i)) {
                primeCount++;
                myfile << primeCount << " : " << i << endl;
                percent = (i/max)*100;
                window.primePopulateProgressBar(percent);
            }
        }
    } else {

        long primeStart = seperateFileEnd(line);
        long primeCount = seperateFileStart(line);
        for (long i = primeStart+1; i <= max; i++) {
            if (isPrime(i)) {
                primeCount++;
                myfile << primeCount << " : " << i << endl;
                percent = (i/max)*100;
                window.primePopulateProgressBar(percent);
            }
        }
    }

    myfile.close();
}

void D3CMath::primeNumberNerase(){
    ofstream myfile;
    myfile.open("../build-D0sag3Command-Desktop_Qt_5_3_MinGW_32bit-Release/PrimeNumberList.txt");
    myfile.clear();
    myfile.close();
}

//calculates the n'th prime number, starting with 2 as prime number one. 
//Up to a maximum number of 2147483647.
long D3CMath::primeNumberN(long n) { 
    long max = 2147483647;
    long largePrime = 0, primeCount = 0;

    D3CWindow window;
    int percent;

    string line;
    ifstream myfilein("../build-D0sag3Command-Desktop_Qt_5_3_MinGW_32bit-Release/PrimeNumberList.txt");
    if (myfilein.is_open()) {
        line = getLastLine(myfilein);
    }
    myfilein.close();

    if(!line.empty()){
        primeCount = seperateFileStart(line);
    }
    if(line.empty()){
        for (long i = 2; i < max; i++) {
            if (isPrime(i) == true && primeCount < n) {
                percent = (i*100/primeCount);
                window.primePopulateProgressBar(percent);
                primeCount++;
                largePrime = i;
            } else if (primeCount >= n) {
                break;
            }
        }

    }else if(primeCount < n){

        long primeStart = seperateFileEnd(line);
        for (long i = primeStart+1; i < max; i++) {
                if (isPrime(i) == true && primeCount < n) {
                    percent = (i*100/primeCount);
                    window.primePopulateProgressBar(percent);
                    primeCount++;
                    largePrime = i;
                } else if (primeCount >= n) {
                    break;
                }
            }

    }else if(primeCount > n){
        long lineNumber = 1;
        long requestedLineNumber = n+5;
        ifstream myfilein("../build-D0sag3Command-Desktop_Qt_5_3_MinGW_32bit-Release/PrimeNumberList.txt");
        while (getline(myfilein, line)){
                if (lineNumber == requestedLineNumber){
                        break;
                }
                lineNumber++;
        }
        myfilein.close();
        largePrime = seperateFileEnd(line);
        primeCount = n;
    }
    return largePrime;
}

//returns the number of digits within a non-zero integer. 
//Negative values neglect the negative sign when given the number of digits. 
//If the value of the input is 0 or a string of 0's the output will be zero.
int D3CMath::numOfDigitsInt(int x){ 
    int count;
    if(x>0){
        count = log10(x)+1;
    } else if(x<0){
        x=x/-1;
        count=log10(x)+1;
    }else{
        count=0;
    }
    return count;
}

int D3CMath::setLargerInt(int One, int Two) {
    int largerInt;
    if (One >= Two) { //sets the variable of largerInt to the length of the larger of the 2 ints.
        largerInt = One;
    } else {
        largerInt = Two;
    }
    return largerInt;
}

//Creates a string of all zero's of a certain length.
string D3CMath::zeroString(int length) { 
    string Total;
    Total.resize(length);
    for (int i = 0; i < length; i++) { //sets all of the numbers in Total to zero.
        Total[i] = 48; //48 is 0 is string language.
    }
    return Total;
}

//Converts a string to a vector
vector<int> D3CMath::stringToVector(string a) { 
    int length = a.size();
    vector<int> vector(length);
    for (int i = 0; i < length; i++) { //converts string one into an array of integers.
        vector[i] = a[i] - 48;
    }
    return vector;
}

//converts a vector of numbers to a string and removes the preceding zeros.
string D3CMath::vectorToString(vector<int> a) { 

    bool lastDigitZero = false;
    int lengthTotal = a.size(), count = 0;
    string Total = zeroString(lengthTotal);
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

//resizes a smaller string to be the same as a second string by placing zero's in front of it until it is the same length as the second string. 
//The larger string is inputed first (a) and the smaller string to be resized is inputed as the second string (b).
string D3CMath::resizeString(string a, string b) { 
    int lengthOne = a.size(), lengthTwo = b.size(), origlengthTwo = lengthTwo,
            temp = 0;
    int difference = lengthOne - lengthTwo; //finds the difference in size of the two strings so we know how much one needs stretched by.
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

string D3CMath::addNegativeSign(string Total){
    int stringLength = Total.size();
    string Totality;
    Totality.resize(stringLength+1);
    Totality[0] = '-';
    for(int i=0;i<stringLength;i++){
        Totality[i+1] = Total[i];
    }
    return Totality;
}

//adds 2 strings together and returns their sum as a string.
string D3CMath::addTwoStrings(string stringOne, string stringTwo) { 
    int lengthOne = stringOne.size(), lengthTwo = stringTwo.size(),
            largerLength = setLargerInt(lengthOne, lengthTwo), lengthTotal,
            number, var; //largerLength is now the length of the largest string.
    bool isPositive = true;

    if(stringOne[0] == '-' && stringTwo[0] == '-'){
        isPositive = false;
        for(int i=0;i<lengthOne;i++){
            stringOne[i] = stringOne[i+1];
        }
        for(int i=0;i<lengthTwo;i++){
            stringTwo[i] = stringTwo[i+1];
        }
        stringOne.resize(lengthOne-1);
        stringTwo.resize(lengthTwo-1);
        lengthOne -=1;
        lengthTwo -=1;
        largerLength -=1;
    }else if(stringOne[0] =='-' || stringTwo[0] == '-'){
        return "Error. Use the subtract function for one negative number and one positive number.";
    }

    string Total = zeroString(largerLength + 1); //creates a blank string for the total of the two added strings.
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

    Total = vectorToString(TotalArray);
    cout << "total: " << Total << endl;
    if(isPositive == false){
        Total = addNegativeSign(Total);
    }

    return Total; //returns the Total of the two strings.
}

//subtracts 2 strings together and returns their sum as a string.
string D3CMath::subtractTwoStrings(string stringOne, string stringTwo) { 

    int lengthOne = stringOne.size(), lengthTwo = stringTwo.size(), largerLength = setLargerInt(lengthOne, lengthTwo), lengthTotal; //largerLength is now the length of the largest string.
    bool isPositive = NULL, sameLength = NULL;
    string Difference = zeroString(largerLength+1); //creates a blank string for the total of the two strings.
    lengthTotal = Difference.size();

    if(stringOne == stringTwo){
        return "0";
    }

    if (lengthOne > lengthTwo) {
        stringTwo = resizeString(stringOne, stringTwo);
        cout << "stringTwo: " << stringTwo << endl;
        lengthTwo = stringTwo.size();
        sameLength = false;
        isPositive = true;
    } else if (lengthTwo > lengthOne) {
        stringOne = resizeString(stringTwo, stringOne);
        lengthOne = stringOne.size();
        sameLength = false;
        isPositive = false;
    }else{
        sameLength = true;
    }

    vector<int> tempArray, TotalArray = stringToVector(Difference), stringOneArray = stringToVector(stringOne), stringTwoArray = stringToVector(stringTwo); //creates an int array for each string to be converted to and then converts the, to an array of integers using a vector.

    if(sameLength){
        while((isPositive != true) || (isPositive != false)){
            for(int i=0;i<lengthOne;i++){
                 if(stringOneArray[i] > stringTwoArray[i]){
                    isPositive = true;
                    break;
                }else if(stringOneArray[i] < stringTwoArray[i]){
                    isPositive = false;
                    break;
                }
            }
        }
    }

    if(isPositive == false){
        stringOneArray = stringToVector(stringTwo);
        stringTwoArray = stringToVector(stringOne);
    }

    for (int i = 0; i <= lengthTotal; i++) { //Subtracts the two string arrays and creates the Difference array.
        if(stringOneArray[lengthOne - i] >= stringTwoArray[lengthTwo - i]){
            TotalArray[lengthTotal - i] = stringOneArray[lengthOne - i] - stringTwoArray[lengthTwo - i];
        } else{
            while (stringOneArray[lengthOne - i] < stringTwoArray[lengthTwo - i]){
                for (int x=0;x<=lengthOne-i;x++){
                    if(stringOneArray[lengthOne - i -x] == 0){
                        //cout << "stringOneArray[lengthOne - i -x] == 0" << endl;
                    }else{
                        stringOneArray[lengthOne - i -x]-=1;
                        stringOneArray[lengthOne - i -x +1]+=10;
                        break;
                    }
                }
            }
            TotalArray[lengthTotal - i] = stringOneArray[lengthOne - i] - stringTwoArray[lengthTwo - i];
        }
    }

    Difference = vectorToString(TotalArray);

    if(isPositive == false){
        Difference = addNegativeSign(Difference);
    }

    return Difference; //returns the Difference two strings.
}

//multiplies two strings of numbers of any length together.
string D3CMath::multiplyTwoStrings(string stringOne, string stringTwo) { 

    if(stringOne == "0" || stringTwo == "0"){
        return "0";
    }

    bool isPositive;
    int l1 = stringOne.size(), l2 = stringTwo.size();
    if(stringOne[0] == '-' && stringTwo[0] == '-'){
        isPositive = true;
        for(int i=0;i<l1;i++){
            stringOne[i]=stringOne[i+1];
        }
        stringOne.resize(stringOne.size()-1);
        for(int i=0;i<l2;i++){
            stringTwo[i]=stringTwo[i+1];
        }
        stringTwo.resize(stringTwo.size()-1);
    } else if(stringOne[0] == '-'){
        isPositive = false;
        for(int i=0;i<l1;i++){
            stringOne[i]=stringOne[i+1];
        }
        stringOne.resize(stringOne.size()-1);
    } else if(stringTwo[0] == '-'){
        isPositive = false;
        for(int i=0;i<l2;i++){
            stringTwo[i]=stringTwo[i+1];
        }
        stringTwo.resize(stringTwo.size()-1);
    } else {
        isPositive = true;
    }
    int lengthOne = stringOne.size(), lengthTwo = stringTwo.size(),
            largerLength = setLargerInt(lengthOne, lengthTwo), lengthTotal, difference = 0, origLengthOne = lengthOne, origLengthTwo = lengthTwo, number, temp;
    string Total = zeroString(largerLength *2); //creates a blank string for the total of the two added strings.
    lengthTotal = Total.size();

    if (lengthOne == lengthTwo) {
        difference = 0; //if the lengths of the starting two strings are equal in length, do nothing, otherwise set the lengths equal to each other and make the empty digits before the smaller number 0.
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

    //fixes any areas in the array that have more than one digit by taking the number of tens and adding it to hte next place holder while making the current place holder the number % 10.
    for(int i=0;i<lengthTotal;i++){
        number=TotalArray[lengthTotal-1-i];
        temp = numOfDigitsInt(number);
        TotalArray[lengthTotal-1-i]=0;
        for(int j=0;j<temp;j++){
            TotalArray[lengthTotal-1-j-i] += number%10;
            number=number/10;
        }

    }

    //Converts the vector back into a string and returns it.
    Total = vectorToString(TotalArray);
    if(isPositive == false){
        Total = addNegativeSign(Total);
    }
    return Total;
}

// finds the largest product of x consecutive numbers in a string of numbers that is of length sL.
long D3CMath::largestProductOfXConsecutiveNumbers(string string, long x) { 

    int sL = string.size(), temp = 0;
    long max = 0;
    int num[sL];
    for (int i = 0; i < sL; i++) {
        num[i] = (string[i] - 48); // "-48" converts the ascii number of 0,1,2,3,... (48,49,50,51,... in ascii) to a regular int.
    }
    for (int p = 0; p <= sL - x; p++) {
        temp = num[p];
        for (int q = 1; q < x; q++) {
            temp *= (num[p + q]);
        }
        if (temp > max) {
            max = temp;
        }
    }
    return max;
}

string D3CMath::productCollatzSequence(long n){
    string sequence, temp;

    if(n == 1){
        cout << "1" << endl;
    }else{
        while (n > 1) {
            if (n % 2 == 0) {
                n = n / 2;
            } else {
                n = (3 * n) + 1;
            }
            std::stringstream strstream;
            strstream << n;
            strstream >> temp;
            sequence.append(temp);
            sequence.append(" ");
        }
    }
return sequence;
}

//determines the amount of lattice paths to the bottom right corner of a square grid of size s x s
long D3CMath::latticePathsOfSquare(long x, long y){ 
    long space[x + 1][y + 1];
    for (int i = 0; i <= x; i++) {
        for (int j = 0; j <= y; j++) {
            if (i == 0) {
                space[i][j] = 1;
            } else if (j == 0) {
                space[i][j] = 1;
            } else {
                space[i][j] = space[i - 1][j] + space[i][j - 1];
            }
        }
    }
    return space[x][y];
}

D3CMath::~D3CMath() {
    // TODO Auto-generated destructor stub
}

} /* namespace std */
