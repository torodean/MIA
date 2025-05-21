/**
 * File: FileUtils.cpp
 * Author: Antonius Torode
 * Date: 03/07/2021
 * Description: Implementation for the FuleUtil methods.
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>

// Include the associated header file.
#include "FileUtils.hpp"

// Used for error handling.
#include "Error.hpp"

using std::string;
using std::cout;
using std::endl;

string getRandomLineOfFile(string& fileName)
{
    int counter = 0;
    string output;
    std::ifstream file(fileName);

    while (getline(file, output))
    {
        counter++;
    }
    file.close();

    std::ifstream file2(fileName);
    // Create a seeded random engine (usually done once, not every call)
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());    
    // Generate a random integer between 1 and counter (inclusive)
    int random = std::uniform_int_distribution<int>(1, counter)(rng);
    counter = 0;

    while (getline(file2, output))
    {
        counter++;
        if(counter == random)
        {
            return output;
        }
    }
    return "ERROR"; // @TODO - Add MIA Exception here.
}

void printRandomLinesFromFile(std::string filePath, int numberOfLines)
{
    // Grabs the file.
    std::ifstream file(filePath, std::ifstream::in);

    // Checks if the file exists and runs the code.
    if (file.good())
    {
        string line;
        std::vector<string> lines;

        while(getline(file,line))
        {
            //cout << line << endl;
            lines.push_back(line);
        }

        int listSize = lines.size();
        srand(time(nullptr));
        int randomNum;

        for(int i=0; i<numberOfLines; i++)
        {
            randomNum = rand() % listSize;
            cout << lines[randomNum] << endl;
        }
    } 
    else 
    {
        // @TODO - change this to use the new MIA Exception class.
        error::returnError(41404);
    }
}
