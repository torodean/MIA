/**
 * @file D3CMath.cpp
 * @author Antonius Torode
 * @date October 5, 2014
 * 
 * This file contains a collection of legacy math functions originally developed
 * for the D3C program, now adapted for integration within the MIA project.
 * 
 * The functions provide various mathematical utilities supporting the broader application.
 */

#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <bitset>
#include <limits>
#include <algorithm>

#include "D3CMath.hpp"
#include "StringUtils.hpp"

// Used for error handling.
#include "MIAException.hpp"
#include "Error.hpp"

// These are used a lot in this file.
using std::string;
using std::vector;

namespace math
{
    D3CMath::D3CMath() : primeNlocation("../resources/PrimeNumberList.txt") {
        //Chuck Norris's keyboard doesn't have a Ctrl key because nothing controls Chuck Norris.
    }


    D3CMath::~D3CMath()
    {
        //Chuck Norris can delete the Recycling Bin.
    }


    long sumOfDigits(const string& number)
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


    uint32_t numberOfFactors(long x)
    {
        uint32_t num = 0;
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
        for (int i = 1; i <= x; i++)
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


    std::istream& ignoreline(std::ifstream& in, std::ifstream::pos_type& pos)
    {
        pos = in.tellg();
        return in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }


    string getLastLine(std::ifstream& in)
    {
        std::ifstream::pos_type pos = in.tellg();

        std::ifstream::pos_type lastPos;
        while (in >> std::ws && ignoreline(in, lastPos))
            pos = lastPos;

        in.clear();
        in.seekg(pos);

        string line;
        getline(in, line);
        return line;
    }


    void D3CMath::primeFactors(long x)
    {
       // std::cout << "The prime factors are: " << std::endl;
        for (long i = 1; i <= (x + 1) / 2; i++)
        {
                if (isPrime(i) && x % i == 0)
                {
                    //std::cout << i << std::endl;
                }
            }
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
        lineStart = std::stol(lineStartString);

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
        lineEnd = std::stol(lineEndString);
        return lineEnd;
    }


    //populates all prime numbers, this can be used to populate a list of all primes 
    //to increase the efficiency of primeNumberN(); Up to a maximum number of 2147483647.
    void D3CMath::primeNumberNpopulate()
    {
        std::ofstream myfile;
        string line;
        std::ifstream myfilein(primeNlocation);
        long max = 2147483647;

        if (myfilein.is_open())
        {
            line = getLastLine(myfilein);
        }
        myfilein.close();

        myfile.open(primeNlocation, std::ios::app);

        if (line.empty())
        {
            myfile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            myfile << "~~ List of Prime Numbers ~~" << std::endl;
            myfile << "~~~~~~~ DO NOT EDIT ~~~~~~~" << std::endl;
            myfile << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl << std::endl;

            long fileStart = 2;
            long primeCount = 0;
            for (long i = fileStart; i <= max; i++)
            {
                if (isPrime(i))
                {
                    primeCount++;
                    myfile << primeCount << " : " << i << std::endl;
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
                    myfile << primeCount << " : " << i << std::endl;
                }
            }
        }

        myfile.close();
    }


    //Clears the primeNumber.txt file
    void D3CMath::primeNumberNerase()
    {
        std::ofstream myfile;
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
        std::ifstream myfilein(primeNlocation);

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
            std::ifstream primeFile(primeNlocation);
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


    uint32_t numOfDigitsInt(int x)
    {
        uint32_t count;
        if(x > 0)
        { // Positive numbers.
            count = log10(x)+1;
        }
        else if(x < 0)
        { // Negative numbers.
            x=x/-1;
            count=log10(x)+1;
        }
        else 
        { // The number is zero. Special case returns zero.
            count = 0; 
        }
        return count;
    }


    int getLargerInt(int One, int Two)
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


    string vectorToString(vector<int> a)
    {
        // Check if it is a zero'd vector first.
        if (std::all_of(a.begin(), a.end(), [](int digit) { return digit == 0; }))
        {
            return "0";
        }

        bool lastDigitZero = false;
        int lengthTotal = a.size(), count = 0;
        string total = string_utils::zeroString(lengthTotal);

        /*
         * If the first number is a 0, this will place it at the end instead of the beginning, 
         * in term multiplying the number by 10. The increase by a factor of 10 will be corrected 
         * once the number is converted back to a string and the string is down sized one digit 
         * (the last digit).
         */
        while (a[0] == 0) 
        {
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
            total[i] = a[i] + 48;
        }

        /*
         * Removes the last digit if it is zero, but only if it was moved from the front to the 
         * back, to correct for the increase in factor of 10 that was produced earlier.
         */
        while (lastDigitZero && count != 0)
        {
            total.resize(lengthTotal - 1);
            lengthTotal = total.size();
            if (total[lengthTotal - 1] == 48)
            {
                lastDigitZero = true;
            } 
            else 
            {
                lastDigitZero = false;
            }
            count--;
        }
        return total;
    }


    string addNegativeSign(const string& input)
    {
        if (input[0] == '-')
        { // String already contains a negative.
            return input;
        }

        // Check non-numerical cases.
        if (!string_utils::isDigits(input))
        {
            MIA_THROW(error::ErrorCode::Invalid_Parameter, 
                      "The addNegativeSign method expects the input string to represent an integer.");
        }

        // Check if the strings is equivalent to zero.
        std::string zStr = string_utils::zeroString(input.size());
        if (input == zStr)
        { // Zero should be neither positive or negative.
            return input;
        }

        int stringLength = input.size();
        string output;
        output.resize(stringLength + 1);
        output[0] = '-';
        for(int i=0; i<stringLength; i++)
        {
            output[i+1] = input[i];
        }
        return output;
    }


    string addTwoStrings(string stringOne, string stringTwo)
    {
        int lengthOne = stringOne.size(); 
        int lengthTwo = stringTwo.size();

        // Set largerLength to the length of the largest string.
        int largerLength = getLargerInt(lengthOne, lengthTwo);
        int lengthTotal;
        int number;
        int var; 
        bool isPositive = true;

        if(stringOne[0] == '-' && stringTwo[0] == '-')
        { // If both numbers are negative, then their summation is also negative.
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
        }
        else if(stringOne[0] =='-' || stringTwo[0] == '-')
        { // If just one of the numbers is negative, the summation could be positive or negative.
            return "Error. Use the subtract function for one negative number and one positive number.";
        }

        // Creates a blank string for the total of the two added strings.
        string total = string_utils::zeroString(largerLength + 1);
        lengthTotal = total.size();

        if (lengthOne == lengthTwo)
        {
            /*
             * If the lengths of the starting two strings are equal in length, do nothing, 
             * otherwise set the lengths equal to each other and make the empty digits before 
             * the smaller number 0.
             */
            ;
        }
        else if (lengthOne > lengthTwo)
        {
            stringTwo = string_utils::resizeString(stringOne, stringTwo, '0');
            lengthTwo = stringTwo.size();
        }
        else if (lengthTwo > lengthOne)
        {
            stringOne = string_utils::resizeString(stringTwo, stringOne, '0');
            lengthOne = stringOne.size();
        }

        /*
         * Creates an int array for each string to be converted to and then converts the, 
         * to an array of integers using a vector.
         */
        vector<int> totalArray = stringToVector(total);
        vector<int> stringOneArray = stringToVector(stringOne); 
        vector<int> stringTwoArray = stringToVector(stringTwo);

        for (int i = 1; i <= lengthTotal - 1; i++)
        { // Adds the two string arrays together and combine them into the total array.
            number = stringOneArray[lengthOne - i] + stringTwoArray[lengthTwo - i];
            totalArray[lengthTotal - i] += number % 10;
            totalArray[lengthTotal - 1 - i] += number / 10;
        }
        var = lengthTotal - 1;

        /*
         * In some cases, if the two positions added up to 9, then were added to an existing 
         * one in the total array, the number was 10. So this corrects for that.
         */
        while (var > 0) 
        { 
            if (totalArray[var] == 10)
            {
                totalArray[var] = 0;
                totalArray[var - 1] += 1;
            }
            var--;
        }

        total = vectorToString(totalArray);

        if(!isPositive)
        {
            total = addNegativeSign(total);
        }

        return total; // Returns the total of the two strings.
    }


    string subtractTwoStrings(const string& strOne, const string& strTwo)
    {
        string stringOne = strOne;
        string stringTwo = strTwo;

        if(stringOne == stringTwo)
        { // The strings are the same so they cancel out to zero.
            return "0";
        }

        // Check if the first string is equivalent to zero.
        string zStr = string_utils::zeroString(stringOne.size());
        if (stringOne == zStr)
        { // The first input being zero implies the output is just -1*stringTwo.
            if (stringTwo[0] == '-')
            { // The second string is already negative so flip it's sign.
                return stringTwo.substr(1,stringTwo.size());
            }
            else
            { // Make the second input negative and return it.
                return addNegativeSign(stringTwo);
            }
        }

        // Check if we can simplify the problem any. 
        if (stringOne[0] == '-')
        { // The first string is negative.
            if (stringTwo[0] == '-')
            { // The second string is also negative. (-A) - (-B) = B - A
                return subtractTwoStrings(stringTwo.substr(1), stringOne.substr(1));
            }
            else
            { // The second string is positive. (-A) - B = - (A + B)
                return addNegativeSign(addTwoStrings(stringOne.substr(1), stringTwo));
            }
        }
        else if (stringTwo[0] == '-')
        { // Only the second string is negative. A - (-B) = A + B
            return addTwoStrings(stringOne, stringTwo.substr(1));
        }

        // If reaching this point, both strings should always be positive values.
        if (stringOne[0] == '-' || stringTwo[0] == '-')
        { // This is a sanity check - should never happen unless the above logic changes.
            MIA_THROW(error::ErrorCode::Catastrophic_Failure);
        }

        int lengthOne = stringOne.size(); 
        int lengthTwo = stringTwo.size();

        if (lengthOne > lengthTwo)
        { // If the first string has more digits than the second, the difference is positive.
            stringTwo = string_utils::resizeString(stringOne, stringTwo, '0');
            lengthTwo = stringTwo.size();
        }
        else if (lengthTwo > lengthOne)
        { // => Negative difference.
            stringOne = string_utils::resizeString(stringTwo, stringOne, '0');
            lengthOne = stringOne.size();
        }
        // Now lengthOne == lengthTwo.

        /*
         * Creates an int array for each string to be converted to and then converts 
         * the, to an array of integers using a vector.
         */
        vector<int> totalArray(lengthOne, 0);
        vector<int> stringOneArray = stringToVector(stringOne);
        vector<int> stringTwoArray = stringToVector(stringTwo);

        bool isPositive = false;

        // Check which string is the larger number to determine sign of output.
		for(int i=0; i<lengthOne; i++)
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

        if(!isPositive)
        { // Make the first string array the larger value always - add sign later.
            std::swap(stringOneArray, stringTwoArray);
        }

        int borrow = 0;
        for (int i = lengthOne - 1; i >= 0; --i)
        { // Start at last digit and move towards the first.
            int diff = stringOneArray[i] - stringTwoArray[i] - borrow;
            if (diff < 0)
            {
                diff += 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            totalArray[i] = diff;
        }

        string difference = vectorToString(totalArray);

        if(!isPositive)
        { // Apply the negative sign if the answer should be negative.
            difference = addNegativeSign(difference);
        }

        return difference; // Returns the difference two strings.
    } // string subtractTwoStrings()


    string multiplyTwoStrings(const string& strOne, const string& strTwo)
    {
        string stringOne = strOne;
        string stringTwo = strTwo;

        if(stringOne == "0" || stringTwo == "0")
        { // Handle the trivial case of zero multiplication.
            return "0";
        }

        // Determine the sign of the output.
        bool isPositive;
        if(stringOne[0] == '-' && stringTwo[0] == '-')
        {
            isPositive = true;
            stringOne = stringOne.substr(1);
            stringTwo = stringTwo.substr(1);
        }
        else if(stringOne[0] == '-')
        {
            isPositive = false;
            stringOne = stringOne.substr(1);
        }
        else if(stringTwo[0] == '-')
        {
            isPositive = false;
            stringTwo = stringTwo.substr(1);
        } 
        else 
        {
            isPositive = true;
        }

        int lengthOne = stringOne.size();
        int lengthTwo = stringTwo.size();
        int largerLength = getLargerInt(lengthOne, lengthTwo);
        int lengthTotal;
        int difference = 0;
        int origLengthOne = lengthOne;
        int origLengthTwo = lengthTwo;
        int number;
        int temp;

        // Creates a blank string for the total of the two added strings.
        string total = string_utils::zeroString(largerLength *2);
        lengthTotal = total.size();

        if (lengthOne == lengthTwo)
        {
            /*
             * If the lengths of the starting two strings are equal in length, do nothing, 
             * otherwise set the lengths equal to each other and make the empty digits 
             * before the smaller number 0.
             */
            difference = 0;
        }
        else if (lengthOne > lengthTwo)
        {
            difference = lengthOne-lengthTwo;
            stringTwo = string_utils::resizeString(stringOne, stringTwo, '0');
            lengthTwo = stringTwo.size();
        }
        else if (lengthTwo > lengthOne)
        {
            difference = lengthTwo - lengthOne;
            stringOne = string_utils::resizeString(stringTwo, stringOne, '0');
            lengthOne = stringOne.size();
        }

        vector<int> totalArray = stringToVector(total), stringOneArray =
                stringToVector(stringOne), stringTwoArray = stringToVector(
                stringTwo);

        /*
         * Multiplies the 2 vectors together and combines them in the totalArray. Each 
         * position in the total vector can have multiple digits, which will be fixed later.
         */
        if (origLengthOne > origLengthTwo) 
        {
            for (int i = 0; i < largerLength; i++) 
            {
                for (int j = 0; j < largerLength - difference; j++)
                {
                    totalArray[lengthTotal - 1 - i - j] += 
                        stringOneArray[lengthOne-1-i] * stringTwoArray[lengthTwo-1-j];
                }
            }
        }
        else 
        {
            for (int i = 0; i < largerLength - difference; i++)
            {
                for (int j = 0; j < largerLength; j++)
                {
                    totalArray[lengthTotal - 1 - i - j] += 
                        stringOneArray[lengthOne-1-i] * stringTwoArray[lengthTwo-1-j];
                }
            }
        }

        /*
         * Fixes any areas in the array that have more than one digit by taking the 
         * number of tens and adding it to the next place holder while making the 
         * current place holder the number % 10.
         */
        for(int i=0;i<lengthTotal;i++)
        {
            number=totalArray[lengthTotal-1-i];
            temp = numOfDigitsInt(number);
            totalArray[lengthTotal-1-i]=0;
            for(int j=0;j<temp;j++)
            {
                totalArray[lengthTotal-1-j-i] += number%10;
                number=number/10;
            }

        }

        // Converts the vector back into a string and returns it.
        total = vectorToString(totalArray);
        if(!isPositive)
        { // Adds the negative sign if the total should be negative.
            total = addNegativeSign(total);
        }
        return total;
    }


    long largestProductOfXConsecutiveNumbers(const string& input, uint32_t x)
    {
        if (x == 0 && !input.empty()) // input.empty() is a special case handled below.
        { // Zero is a special case.
            return 0;
        }

        if (x > input.size() || input.empty())
        { // x cannot be larger than the string.
            std::stringstream errorStream;
            errorStream << "Cannot have " << std::to_string(x) 
                        << " consecutive numbers in a string of size " << input.size();

            std::string errorStr = errorStream.str();
            MIA_THROW(error::ErrorCode::Invalid_Parameter, errorStr);
        }

        uint32_t length = input.size();
        uint32_t temp = 0;
        long max = 0;
        std::vector<int> num;
        num.resize(length);

        for (uint32_t i = 0; i < length; i++)
        {
            // "-48" converts the ascii number of 0,1,2,3,... (48,49,50,51,... in ascii) to a regular int.
            num[i] = (input[i] - 48);
        }

        for (uint32_t p = 0; p <= length - x; p++)
        {
            temp = num[p];
            for (uint32_t q = 1; q < x; q++)
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
            std::cout << "1" << std::endl;
        } else {
            while (n > 1)
            {
                if (n % 2 == 0)
                {
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
    	answer.append(std::to_string(-b*b/2.0/a));
    	if (b*b-4.0*a*c == 0)
    	{
    		std::cout << "b*b-4*a*c == 0" << std::endl;
    	}
    	else if(b*b-4.0*a*c > 0)
    	{
    		std::cout << "b*b-4*a*c > 0" << std::endl;
    		answer.append(" +/- ");
    		answer.append(std::to_string(sqrt(b*b-4.0*a*c)));
    	} 
        else 
        {
    		std::cout << "b*b-4*a*c < 0" << std::endl;
    		answer.append(" +/- ");
    		answer.append(std::to_string(sqrt(4.0*a*c-b*b)));
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
