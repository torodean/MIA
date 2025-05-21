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
#include <filesystem>  // C++17
#include <iterator>

// Include the associated header file.
#include "FileUtils.hpp"

// Used for error handling.
#include "Error.hpp"

using std::string;
using std::cout;
using std::endl;

namespace files
{
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


    bool fileExists(const std::string& filePath)
    {
        return std::filesystem::exists(filePath);
    }


    std::vector<std::string> readAllLines(const std::string& filePath)
    {
        std::vector<std::string> lines;
        std::ifstream file(filePath);
        if (!file.is_open()) 
            return lines;

        std::string line;
        while (std::getline(file, line))
            lines.push_back(line);
        
        return lines;
    }


    void writeLinesToFile(const std::string& filePath, const std::vector<std::string>& lines)
    {
        std::ofstream file(filePath, std::ios::trunc);
        if (!file.is_open()) 
            return; // @TODO - Add MIA Exception.

        for (const auto& line : lines)
            file << line << "\n";
    }


    std::string readEntireFile(const std::string& filePath)
    {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        if (!file.is_open()) 
            return "";

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return content;
    }


    void appendLineToFile(const std::string& filePath, const std::string& line)
    {
        std::ofstream file(filePath, std::ios::app);
        if (!file.is_open()) 
            return; // @TODO - Add MIA Exception.

        file << line << "\n";
    }

    size_t countLinesInFile(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open()) return 0;

        size_t count = 0;
        std::string line;
        while (std::getline(file, line))
        {
            ++count;
        }
        return count;
    }


    bool copyFile(const std::string& sourcePath, const std::string& destPath)
    {
        std::error_code ec;
        std::filesystem::copy_file(sourcePath, destPath, std::filesystem::copy_options::overwrite_existing, ec);
        return !ec; // @TODO - Add MIA Exception.
    }


    bool deleteFile(const std::string& filePath)
    {
        std::error_code ec;
        bool removed = std::filesystem::remove(filePath, ec);
        return removed && !ec; // @TODO - Add MIA Exception.
    }
} // namespace files
