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

// The corresponding header file.
#include "StringUtils.hpp"

// Include the core utilities for some re-defined methods.
#include "BasicUtilities.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::to_string;

namespace types
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
        if(findCharInString(input, c) == 0)
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
        shuffle(output.begin(), output.end(), std::mt19937(std::random_device()()));
        return output;
    }
    
    
    string getBeforeChar(string line, char c, bool verboseMode)
    {
        int equalSignLocation = findCharInString(line, c);
    
        if(verboseMode)
            cout << "...stringBeforeEqual: " << line.substr(0, equalSignLocation) << endl;
    
        return line.substr(0, equalSignLocation);
    }
    
    
    string getBetweenEqualAndSemiColon(string line, bool verboseMode)
    {
        int equalSignLocation = findCharInString(line, '=');
        int semiColonLocation = findCharInString(line, ';');
    
        //First remove everything after the semi colon sign. Then keep everything after the equal sign.
        line = line.substr(0,semiColonLocation);
        line = line.substr(equalSignLocation+1,line.size()-1);
    
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
    
    
    string getAfterChar(string line, char c, bool verboseMode)
    {
        int semiColonLocation = findCharInString(line, c);
    
        if(verboseMode)
            cout << "...stringAfterSemiColon: " << line.substr(semiColonLocation+1,line.size()-1) << endl;
    
        return line.substr(semiColonLocation+1,line.size()-1);
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
} // namespace types
