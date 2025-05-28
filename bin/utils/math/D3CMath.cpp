/**
 * File: D3CMath.cpp
 * Author: Antonius Torode
 * Created on: October 5, 2014
 * Description:
 *     This file contains a collection of legacy math functions originally developed
 *     for the D3C program, now adapted for integration within the MIA project.
 * 
 *     The functions provide various mathematical utilities supporting the broader application.
 */

#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <bitset>
#include <limits>
#include "D3CMath.hpp"

using std::endl;
using std::stringstream;
using std::cout;
using std::string;
using std::vector;
using std::ifstream;
using std::ios;
using std::istream;
using std::to_string;
using std::streamsize;
using std::ofstream;
using std::numeric_limits;
using std::ws;

namespace math
{
    D3CMath::D3CMath() : primeNlocation("../resources/PrimeNumberList.txt") {
        //Chuck Norris's keyboard doesn't have a Ctrl key because nothing controls Chuck Norris.
    }


    D3CMath::~D3CMath()
    {
        //Chuck Norris can delete the Recycling Bin.
    }


    long sumOfDigits(string number)
    {
        int size = number.size(), sum = 0;
        int n[size];
        for (int i = 0; i < size; i++)
        {
            n[i] = number[i] - 48;
        }
        for (int q = 0; q < size; q++)
        {
            sum += n[q];
        }
        return sum;
    }
    

    int numberOfFactors(long x)
    {
        int num = 0;
        for (long i = 1; i <= x; i++)
        {
            if (x % i == 0)
            {
                num++;
            }
        }
        return num;
    }
    

    bool isTriangleNumber(long x)
    {
        int num = 0;
        for (int i = 1; i < x; i++)
        {
            num += i;
            if (x == num)
            {
                return true;
                break;
            }
        }
        return false;
    }
    
    // @TODO - ensure this is deprecated then delete.
    int numOfDigitsString(const string& x)
    {
        return x.size();
    }
    
    istream& ignoreline(ifstream& in, ifstream::pos_type& pos)
    {
        pos = in.tellg();
        return in.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    string getLastLine(ifstream& in)
    {
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
    
    void D3CMath::primeFactors(long x)
    {
       // cout << "The prime factors are: " << endl;
        for (long i = 1; i <= (x + 1) / 2; i++)
        {
                if (isPrime(i) && x % i == 0)
                {
                    //cout << i << endl;
                }
            }
    }
    
    //Converts a string to a long.
    // @TODO - move this to StringUtils. 
    long stringToLong(string input)
    {
        long output=0;
        int size = input.size();
        int temp[size];
        for(int i=0;i < size;i++){
            temp[i] = input[i]-48;
            output += temp[i]*pow(10, size-i-1);
        }
        return output;
    }
    
    long seperateFileStart(string line)
    {
        string lineStartString;
        long lineStart;
        int lineLength = line.size();
        int position=0;
        for(int i=0;i<lineLength;i++)
        {
            if(line[i] == ':'){
                position = i;
                break;
            }
        }
        lineStartString.resize(position-1);
        lineStartString.reserve(position-1);
        for(int x=0;x<position-1;x++)
        {
            lineStartString[x] = line[x];
        }
        lineStart = stringToLong(lineStartString);
    
        return lineStart;
    }
    
    long seperateFileEnd(string line)
    {
        string lineEndString;
        long lineEnd;
        int lineLength = line.size();
        int position=0;
        for(int i=0;i<lineLength;i++)
        {
            if(line[i] == ':')
            {
                position = i;
                break;
            }
        }
        lineEndString.resize(lineLength - position-2);
        lineEndString.reserve(lineLength - position-2);
    
        for(int x=0;x<lineLength-(position+2);x++)
        {
            lineEndString[x] = line[x+position+2];
        }
        lineEnd = stringToLong(lineEndString);
        return lineEnd;
    }
    
    //populates all prime numbers, this can be used to populate a list of all primes 
    //to increase the efficiency of primeNumberN(); Up to a maximum number of 2147483647.
    void D3CMath::primeNumberNpopulate()
    {
        ofstream myfile;
        string line;
        ifstream myfilein(primeNlocation);
        long max = 2147483647;
    
        if (myfilein.is_open())
        {
            line = getLastLine(myfilein);
        }
        myfilein.close();
    
        myfile.open(primeNlocation, ios::app);
    
        if (line.empty())
        {
            myfile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
            myfile << "~~ List of Prime Numbers ~~" << endl;
            myfile << "~~~~~~~ DO NOT EDIT ~~~~~~~" << endl;
            myfile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
    
            long fileStart = 2;
            long primeCount = 0;
            for (long i = fileStart; i <= max; i++)
            {
                if (isPrime(i))
                {
                    primeCount++;
                    myfile << primeCount << " : " << i << endl;
                }
            }
        } else {
    
            long primeStart = seperateFileEnd(line);
            long primeCount = seperateFileStart(line);
            for (long i = primeStart+1; i <= max; i++)
            {
                if (isPrime(i))
                {
                    primeCount++;
                    myfile << primeCount << " : " << i << endl;
                }
            }
        }
    
        myfile.close();
    }
    
    //Clears the primeNumber.txt file
    void D3CMath::primeNumberNerase()
    {
        ofstream myfile;
        myfile.open(primeNlocation);
        myfile.clear();
        myfile.close();
    }
    
    //calculates the n'th prime number, starting with 2 as prime number one. 
    //Up to a maximum number of 2147483647.
    long D3CMath::primeNumberN(long n)
    {
        long max = 2147483647;
        long largePrime = 0, primeCount = 0;
    
        string line;
        ifstream myfilein(primeNlocation);
    	
        if (myfilein.is_open())
        {
            line = getLastLine(myfilein);
        }
        myfilein.close();
    
        if(!line.empty())
        {
            primeCount = seperateFileStart(line);
        }
        if(line.empty()){
            for (long i = 2; i < max; i++)
            {
                if (isPrime(i) && primeCount < n)
                {
                    primeCount++;
                    largePrime = i;
                }
                else if (primeCount >= n)
                {
                    break;
                }
            }    
        }
        else if(primeCount < n)
        {    
            long primeStart = seperateFileEnd(line);
            for (long i = primeStart+1; i < max; i++)
            {
                if (isPrime(i) && primeCount < n)
                {
                    primeCount++;
                    largePrime = i;
                }
                else if (primeCount >= n)
                {
                    break;
                }
            }    
        } 
        else if(primeCount > n)
        {
            long lineNumber = 1;
            long requestedLineNumber = n + 5;
            ifstream primeFile(primeNlocation);
            while (getline(primeFile, line))
            {
            if (lineNumber == requestedLineNumber)
                break;
            lineNumber++;
            }
            primeFile.close();
            largePrime = seperateFileEnd(line);
            primeCount = n;
        }
        return largePrime;
    }
    
    
    int numOfDigitsInt(int x)
    {
        int count;
        if(x>0)
        {
            count = log10(x)+1;
        }
        else if(x<0)
        {
            x=x/-1;
            count=log10(x)+1;
        } else {
            count=0;
        }
        return count;
    }
    
    
    int setLargerInt(int One, int Two)
    {
        int largerInt;
        if (One >= Two)
        { //sets the variable of largerInt to the length of the larger of the 2 ints.
            largerInt = One;
        } else {
            largerInt = Two;
        }
        return largerInt;
    }
    
    // @TODO - Move this to StringUtils
    string zeroString(int length)
    {
        string Total;
        Total.resize(length);
        for (int i = 0; i < length; i++)
        { //sets all of the numbers in Total to zero.
            Total[i] = 48; //48 is 0 is string language.
        }
        return Total;
    }
    
    //Converts a string to a vector
    vector<int> stringToVector(string a)
    {
        int length = a.size();
        vector<int> vector(length);
        for (int i = 0; i < length; i++)
        { //converts string one into an array of integers.
            vector[i] = a[i] - 48;
        }
        return vector;
    }
    
    //converts a vector of numbers to a string and removes the preceding zeros.
    string vectorToString(vector<int> a)
    {
    
        bool lastDigitZero = false;
        int lengthTotal = a.size(), count = 0;
        string Total = zeroString(lengthTotal);
        while (a[0] == 0) { //if the first number is a 0, this will place it at the end instead of the beginning, in term multiplying the number by 10. The increase by a factor of 10 will be corrected once the number is converted back to a string and the string is down sized one digit (the last digit).
            for (int i = 0; i < lengthTotal - 1; i++)
            {
                a[i] = a[i + 1];
            }
            a[lengthTotal - 1] = 0;
            lastDigitZero = true;
            count++;
        }
        for (int i = 0; i < lengthTotal; i++)
        {
            Total[i] = a[i] + 48;
        }
        while (lastDigitZero && count != 0)
        { //removes the last digit if it is zero, but only if it was moved from the front to the back, to correct for the increase in factor of 10 that was produced earlier.
            Total.resize(lengthTotal - 1);
            lengthTotal = Total.size();
            if (Total[lengthTotal - 1] == 48)
            {
                lastDigitZero = true;
            } else {
                lastDigitZero = false;
            }
            count--;
        }
        return Total;
    }
    
    
    string resizeString(const string& a, string b)
    {
        int lengthOne = a.size();
        int lengthTwo = b.size();
        int origlengthTwo = lengthTwo;
        int temp = 0;

        // Finds the difference in size of the two strings so we know how much one needs stretched by.
        int difference = lengthOne - lengthTwo; 
        b.resize(lengthTwo + difference);
        for (int i = lengthOne; i >= 0; i--)
        {
            if (i > difference - 1)
            {
                b[i] = b[origlengthTwo - temp];
            } else {
                b[i] = 48;
            }
            temp++;
        }
        return b;
    }
    
    string addNegativeSign(string Total)
    {
        int stringLength = Total.size();
        string Totality;
        Totality.resize(stringLength + 1);
        Totality[0] = '-';
        for(int i=0; i<stringLength; i++)
        {
            Totality[i+1] = Total[i];
        }
        return Totality;
    }
    
    
    string addTwoStrings(string stringOne, string stringTwo)
    {
        int lengthOne = stringOne.size(), lengthTwo = stringTwo.size(),
                largerLength = setLargerInt(lengthOne, lengthTwo), lengthTotal,
                number, var; //largerLength is now the length of the largest string.
        bool isPositive = true;
    
        if(stringOne[0] == '-' && stringTwo[0] == '-')
        {
            isPositive = false;
            for(int i=0;i<lengthOne;i++)
            {
                stringOne[i] = stringOne[i+1];
            }
            for(int i=0;i<lengthTwo;i++)
            {
                stringTwo[i] = stringTwo[i+1];
            }
            stringOne.resize(lengthOne-1);
            stringTwo.resize(lengthTwo-1);
            lengthOne -=1;
            lengthTwo -=1;
            largerLength -=1;
        }else if(stringOne[0] =='-' || stringTwo[0] == '-')
        {
            return "Error. Use the subtract function for one negative number and one positive number.";
        }
    
        string Total = zeroString(largerLength + 1); //creates a blank string for the total of the two added strings.
        lengthTotal = Total.size();
    
        if (lengthOne == lengthTwo)
        {
            ; //if the lengths of the starting two strings are equal in length, do nothing, otherwise set the lengths equal to each other and make the empty digits before the smaller number 0.
        }
        else if (lengthOne > lengthTwo)
        {
            stringTwo = resizeString(stringOne, stringTwo);
            lengthTwo = stringTwo.size();
        }
        else if (lengthTwo > lengthOne)
        {
            stringOne = resizeString(stringTwo, stringOne);
            lengthOne = stringOne.size();
        }
    
        vector<int> TotalArray = stringToVector(Total), stringOneArray =
                stringToVector(stringOne), stringTwoArray = stringToVector(
                stringTwo); //creates an int array for each string to be converted to and then converts the, to an array of integers using a vector.
    
        for (int i = 1; i <= lengthTotal - 1; i++)
        { //Adds the two string arrays together and combine them into the total array.
            number = stringOneArray[lengthOne - i] + stringTwoArray[lengthTwo - i];
            TotalArray[lengthTotal - i] += number % 10;
            TotalArray[lengthTotal - 1 - i] += number / 10;
        }
        var = lengthTotal - 1;
        while (var > 0) { //in some cases, if the two positions added up to 9, then were added to an existing one in the total array, the number was 10. So this corrects for that.
            if (TotalArray[var] == 10)
            {
                TotalArray[var] = 0;
                TotalArray[var - 1] += 1;
            }
            var--;
        }
    
        Total = vectorToString(TotalArray);
    	
        if(!isPositive)
        {
            Total = addNegativeSign(Total);
        }
    
        return Total; //returns the Total of the two strings.
    }
    
    
    string subtractTwoStrings(string stringOne, string stringTwo)
    {
    
        int lengthOne = stringOne.size(), lengthTwo = stringTwo.size(), largerLength = setLargerInt(lengthOne, lengthTwo), lengthTotal; //largerLength is now the length of the largest string.
        bool isPositive = false, sameLength;
        string Difference = zeroString(largerLength+1); //creates a blank string for the total of the two strings.
        lengthTotal = Difference.size();
    
        if(stringOne == stringTwo)
        {
            return "0";
        }
    
        if (lengthOne > lengthTwo)
        {
            stringTwo = resizeString(stringOne, stringTwo);
            lengthTwo = stringTwo.size();
            sameLength = false;
            isPositive = true;
        }
        else if (lengthTwo > lengthOne)
        {
            stringOne = resizeString(stringTwo, stringOne);
            lengthOne = stringOne.size();
            sameLength = false;
            isPositive = false;
        } else {
            sameLength = true;
        }
    
        vector<int> tempArray, TotalArray = stringToVector(Difference), stringOneArray = stringToVector(stringOne), stringTwoArray = stringToVector(stringTwo); //creates an int array for each string to be converted to and then converts the, to an array of integers using a vector.
    
        if(sameLength)
        {
    		for(int i=0;i<lengthOne;i++)
    		{
                if(stringOneArray[i] > stringTwoArray[i])
                {
    				isPositive = true;
    				break;
    			}
                else if(stringOneArray[i] < stringTwoArray[i])
    			{
    				isPositive = false;
    				break;
    			}
    		}
        }
    
        if(!isPositive)
        {
            stringOneArray = stringToVector(stringTwo);
            stringTwoArray = stringToVector(stringOne);
        }
    
        for (int i = 0; i <= lengthTotal; i++)
        { //Subtracts the two string arrays and creates the Difference array.
            if(stringOneArray[lengthOne - i] >= stringTwoArray[lengthTwo - i])
            {
                TotalArray[lengthTotal - i] = stringOneArray[lengthOne - i] - stringTwoArray[lengthTwo - i];
            } else {
                while (stringOneArray[lengthOne - i] < stringTwoArray[lengthTwo - i])
                {
                    for (int x=0;x<=lengthOne-i;x++)
                    {
                        if(stringOneArray[lengthOne - i -x] == 0)
                        {
                            //cout << "stringOneArray[lengthOne - i -x] == 0" << endl;
                        } else {
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
    
        if(!isPositive)
        {
    		Difference = addTwoStrings(Difference, "1");
            Difference = addNegativeSign(Difference);
        }
    
        return Difference; //returns the Difference two strings.
    }
    
    
    string multiplyTwoStrings(string stringOne, string stringTwo)
    {
    
        if(stringOne == "0" || stringTwo == "0")
        {
            return "0";
        }
    
        bool isPositive;
        int l1 = stringOne.size(), l2 = stringTwo.size();
        if(stringOne[0] == '-' && stringTwo[0] == '-')
        {
            isPositive = true;
            for(int i=0;i<l1;i++)
            {
                stringOne[i]=stringOne[i+1];
            }
            stringOne.resize(stringOne.size()-1);
            for(int i=0;i<l2;i++)
            {
                stringTwo[i]=stringTwo[i+1];
            }
            stringTwo.resize(stringTwo.size()-1);
        }
        else if(stringOne[0] == '-')
        {
            isPositive = false;
            for(int i=0;i<l1;i++)
            {
                stringOne[i]=stringOne[i+1];
            }
            stringOne.resize(stringOne.size()-1);
        }
        else if(stringTwo[0] == '-')
        {
            isPositive = false;
            for(int i=0;i<l2;i++)
            {
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
    
        if (lengthOne == lengthTwo)
        {
            difference = 0; //if the lengths of the starting two strings are equal in length, do nothing, otherwise set the lengths equal to each other and make the empty digits before the smaller number 0.
        }
        else if (lengthOne > lengthTwo)
        {
            difference = lengthOne-lengthTwo;
            stringTwo = resizeString(stringOne, stringTwo);
            lengthTwo = stringTwo.size();
        }
        else if (lengthTwo > lengthOne)
        {
            difference = lengthTwo - lengthOne;
            stringOne = resizeString(stringTwo, stringOne);
            lengthOne = stringOne.size();
        }
    
        vector<int> TotalArray = stringToVector(Total), stringOneArray =
                stringToVector(stringOne), stringTwoArray = stringToVector(
                stringTwo);
    
        if (origLengthOne > origLengthTwo) { ///multiplies the 2 vectors together and combines them in the TotalArray. Each position in the total vector can have multiple digits, which will be fixed later.
            for (int i = 0; i < largerLength; i++) {
                for (int j = 0; j < largerLength - difference; j++)
                {
                    TotalArray[lengthTotal - 1 - i - j] += stringOneArray[lengthOne-1-i]
                            * stringTwoArray[lengthTwo-1-j];
                }
            }
        } else {
            for (int i = 0; i < largerLength - difference; i++)
            {
                for (int j = 0; j < largerLength; j++)
                {
                    TotalArray[lengthTotal - 1 - i - j] += stringOneArray[lengthOne-1-i]
                            * stringTwoArray[lengthTwo-1-j];
                }
            }
        }
    
        //fixes any areas in the array that have more than one digit by taking the number of tens and adding it to the next place holder while making the current place holder the number % 10.
        for(int i=0;i<lengthTotal;i++)
        {
            number=TotalArray[lengthTotal-1-i];
            temp = numOfDigitsInt(number);
            TotalArray[lengthTotal-1-i]=0;
            for(int j=0;j<temp;j++)
            {
                TotalArray[lengthTotal-1-j-i] += number%10;
                number=number/10;
            }
    
        }
    
        //Converts the vector back into a string and returns it.
        Total = vectorToString(TotalArray);
        if(!isPositive)
        {
            Total = addNegativeSign(Total);
        }
        return Total;
    }
    
    
    long largestProductOfXConsecutiveNumbers(string str, long x)
    {
        int sL = str.size(), temp = 0;
        long max = 0;
        int num[sL];
        for (int i = 0; i < sL; i++)
        {
            num[i] = (str[i] - 48); // "-48" converts the ascii number of 0,1,2,3,... (48,49,50,51,... in ascii) to a regular int.
        }
        for (int p = 0; p <= sL - x; p++)
        {
            temp = num[p];
            for (int q = 1; q < x; q++)
            {
                temp *= (num[p + q]);
            }
            if (temp > max)
            {
                max = temp;
            }
        }
        return max;
    }


    string productCollatzSequence(long n)
    {
        string sequence, temp;
    
        if(n == 1)
        {
            cout << "1" << endl;
        } else {
            while (n > 1)
            {
                if (n % 2 == 0)
                {
                    n = n / 2;
                } else {
                    n = (3 * n) + 1;
                }
                stringstream strstream;
                strstream << n;
                strstream >> temp;
                sequence.append(temp);
                sequence.append(" ");
            }
        }
    return sequence;
    }
    
    
    long latticePathsOfSquare(long x, long y)
    {
        long space[x + 1][y + 1];
        for (int i = 0; i <= x; i++)
        {
            for (int j = 0; j <= y; j++)
            {
                if (i == 0)
                {
                    space[i][j] = 1;
                }
                else if (j == 0)
                {
                    space[i][j] = 1;
                } else {
                    space[i][j] = space[i - 1][j] + space[i][j - 1];
                }
            }
        }
        return space[x][y];
    }


    string D3CMath::returnPrimeNlocation()
    {
    	return primeNlocation;
    }
    
    
    string solveQuadraticFormula(double a, double b, double c)
    {
    	string answer;
    	answer.append(to_string(-b*b/2.0/a));
    	if (b*b-4.0*a*c == 0)
    	{
    		cout << "b*b-4*a*c == 0" << endl;
    	}
    	else if(b*b-4.0*a*c > 0)
    	{
    		cout << "b*b-4*a*c > 0" << endl;
    		answer.append(" +/- ");
    		answer.append(to_string(sqrt(b*b-4.0*a*c)));
    	} 
        else 
        {
    		cout << "b*b-4*a*c < 0" << endl;
    		answer.append(" +/- ");
    		answer.append(to_string(sqrt(4.0*a*c-b*b)));
    		answer.append("*i");
    	}
    	return answer;
    }
    
    /* Still working on these.
    string intToBinaryStr(int input, int bits){
    	string binary = bitset<8>(input).to_string(); //to binary.
    	return binary;
    }
    
    unsigned long binaryStrToInt(string input, int bits){
        unsigned long decimal = bitset<8>(input).to_ulong //to decimal.
    	return decimal;
    }
    */
    
    
    bool isPalindrome(long number)
    {
        int size = log10(number) + 1;
        int n[size];
        for (int i = 0; i < size; i++)
        {
            long temp = pow(10, size - 1 - i);
            n[i] = number / (temp) % 10;
        }
        for (int q = 0; q < size; q++)
        {
            if (n[q] != n[size - 1 - q])
                return false;
        }
        return true;
    }
} // namespace math
