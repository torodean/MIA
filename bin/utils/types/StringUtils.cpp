/**
 * File: StringUtils.cpp
 * Author: Antonius Torode
 * Created on: 03/01/2021
 * Description:
 *     Utility implementations for common string operations such as parsing, transformation,
 *     formatting, and analysis. These functions support general-purpose tasks including
 *     case conversion, character search/removal, delimiter-based splitting, and pattern matching,
 *     useful across multiple modules in the application.
 */

#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <algorithm>

// The corresponding header file.
#include "StringUtils.hpp"

// Include the core utilities for some re-defined methods.
#include "BasicUtilities.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::to_string;

namespace StringUtils
{
    std::string toLower(std::string &input)
    {
        std::string output = input;
        for(int i=0; input[i]; i++)
        {
            output[i] = std::tolower(input[i]);
        }
        return output;
    }
    
    
    std::string removeCharInString(string str, char c)
    {
        str.erase(remove(str.begin(), str.end(), c), str.end());
        return str;
    }
    
    
    int findCharInString(string& input, char c)
    {
        return BasicUtilities::findCharInString(input, c);
    }
    
    
    bool stringContainsChar(string& input, char c)
    {
        if(findCharInString(input, c) == -1)
            return false;
        return true;
    }
    
    
    vector<string> delimiterString(string& input, const string& delimiter, bool verboseMode)
    {
        if(verboseMode)
            cout << "input string: " << input << endl;
        vector<string> output;
        size_t pos;
        std::string item;
        while ((pos = input.find(delimiter)) != string::npos)
        {
            item = input.substr(0, pos);
            output.push_back(item);
            if(verboseMode)
                cout << "delimiter item: " << item << endl;
            input.erase(0, pos + delimiter.length());
        }
        output.push_back(input);
        if(verboseMode)
            cout << "delimiter item: " << input << endl;
        int outputSize = output.size();
        if(verboseMode)
        {
            cout << "Contents of output: ";
            for(int i=0; i<outputSize; i++)
            {
                cout << output[i];
                if(i != outputSize - 1)
                    cout << delimiter;
            }
            cout << endl;
        }
        return output;
    }
    
    
    bool is_digits(const string& input)
    {
        if(input.find_first_not_of("0123456789") == string::npos)
            return true;
        return false;
    }
    
    
    bool inputRoll(string& input)
    {
        if(input.size() <= 6 && input.find('d') != string::npos &&
           input.find_first_not_of("1234567890d") == string::npos &&
           delimiterString(input, "d", false).size() == 2)
        {
            return true;
        }
        return false;
    }
    
    
    bool formOfYes(string& input)
    {
        if (toLower(input) == "y" ||
            toLower(input) == "yes" ||
            toLower(input) == "sure" ||
            toLower(input) == "indeed" ||
            toLower(input) == "ok" ||
            toLower(input) == "okay" ||
            toLower(input) == "true" ||
            toLower(input) == "on")
        {
            return true;
        } else {
            return false;
        }
    }
    
    
    string today()
    {
        std::time_t t = std::time(nullptr);   // get time now
        std::tm* now = std::localtime(&t);
        string month = to_string(now->tm_mon + 1);
        string day = to_string(now->tm_mday);
        string year = to_string(now->tm_year + 1900);
        string todaysDate =  month + "-" + day + "-" + year;
        return todaysDate;
    }
    
    
    string shuffleString(string input)
    {
        string output = std::move(input);
        std::shuffle(output.begin(), output.end(), std::mt19937(std::random_device()()));
        return output;
    }
    
    
    string getBeforeChar(string line, char c, bool verboseMode)
    {
        int equalSignLocation = findCharInString(line, c);
    
        std::string out = line.substr(0, equalSignLocation);
        if(verboseMode)
            cout << "...stringBeforeEqual: " << out << endl;
    
        return trim(out);
    }
    
    
    string getAfterChar(string line, char c, bool verboseMode)
    {
        int semiColonLocation = findCharInString(line, c);
        
        std::string out = line.substr(semiColonLocation+1, line.size()-1);
        if(verboseMode)
            cout << "...stringAfterSemiColon: " << out << endl;
    
        return trim(out);
    }
    
    
    string getBetweenEqualAndSemiColon(string line, bool verboseMode)
    {
        int equalSignLocation = findCharInString(line, '=');
        int semiColonLocation = findCharInString(line, ';');
    
        //First remove everything after the semi colon sign. Then keep everything after the equal sign.
        line = line.substr(0,semiColonLocation);
        line = line.substr(equalSignLocation+1, line.size()-1);
    
        if(verboseMode)
            cout << "...stringBetweenEqualAndSemiColon: " << line << endl;
    
        return line;
    }


    string getBetweenXAndY(std::string line, char x, char y, bool verboseMode)
    {
        int xLocation = findCharInString(line, x);
        int yLocation = findCharInString(line, y);
    
        // Trim the string at the end delimiter, then extract the portion after the start delimiter
        line = line.substr(0, yLocation);
        line = line.substr(xLocation + 1, line.size() - 1);
    
        if (verboseMode)
            std::cout << "...stringBetweenXAndY: " << line << std::endl;
    
        return line;
    }
    
    
    std::vector<std::string> entangleText(const string &input)
    {
        int counter = 0;
    
        std::vector<std::string> output;
        std::string strEven, strOdd;
        for (char c : input)
        {
            // Even case.
            if (counter % 2 == 0)
                strEven += c;
            else
                strEven += ' ';
            // Odd case
            if (counter % 2 == 1)
                strOdd += c;
            else
                strOdd += ' ';
            counter++;
        }
        output.push_back(strEven);
        output.push_back(strOdd);
        return output;
    }
    
    
    bool hasExactlyOneOfEachDelimiter(const std::string& line, char delimiterA, char delimiterB)
    {
        return std::count(line.begin(), line.end(), delimiterA) == 1 &&
               std::count(line.begin(), line.end(), delimiterB) == 1;
    }
    
    
    bool hasExactlyOneOfADelimiter(const std::string& line, char delimiterA)
    {
        return std::count(line.begin(), line.end(), delimiterA) == 1;
    }
    
    
    bool contains(const std::string& haystack, const std::string& needle) 
    {
        return haystack.find(needle) != std::string::npos;
    }
    
    
    std::string trim(const std::string& str) 
    {
        // Find the first non-whitespace character
        auto first = std::find_if(str.begin(), str.end(), [](char c)
        { return !std::isspace(c); });

        // Find the last non-whitespace character
        auto last = std::find_if(str.rbegin(), str.rend(), [](char c)
        { return !std::isspace(c); }).base();

        // Return the trimmed string
        return (first <= last) ? std::string(first, last) : "";
    }
    
    
    std::string charVecToString(const std::vector<char>& charVec)
    {
        return std::string(charVec.begin(), charVec.end());
    }


    std::vector<char> stringToCharVec(const std::string& str)
    {
        return std::vector<char>(str.begin(), str.end());
    }
    
    
    std::vector<int> stringToIntVector(const std::string& inputStr) 
    { 
        std::vector<int> outVec(inputStr.size());
	    
	    // converts string one into an array of integers.
        for (std::size_t i=0; i<inputStr.size(); i++)
            outVec[i] = inputStr[i];
            
        return outVec;
    }
    
    
    std::string intVectorToString(const std::vector<int>& inputVec)
    {
    
        std::string outputStr;
        outputStr.resize(inputVec.size());
        
        for (std::size_t i=0; i<inputVec.size(); i++)
            outputStr[i] = inputVec[i];
            
        return outputStr;
    }
    
    
    std::string invertString(const std::string& inputStr)
    {
        std::string output;
        std::size_t strSize = inputStr.size();
        output.resize(strSize);
	    
	    if(strSize == 0)
		    return "";
		    
        for(std::size_t i=0; i<strSize; i++)
            output[i] = inputStr[strSize-1-i];

        return output;
    }
} // namespace types
