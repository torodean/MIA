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
#include "MIAException.hpp"

using std::string;
using std::cout;
using std::endl;

namespace files
{
    string getRandomLineOfFile(string& fileName)
    {
        try
        {
            int counter = 0;
            string output;
            std::ifstream file(fileName);
            if (!file.is_open()) return "ERROR";

            while (std::getline(file, output))
            {
                counter++;
            }
            file.close();

            if (counter == 0) return "ERROR";

            std::ifstream file2(fileName);
            if (!file2.is_open()) return "ERROR";

            static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
            int random = std::uniform_int_distribution<int>(1, counter)(rng);
            counter = 0;

            while (std::getline(file2, output))
            {
                counter++;
                if (counter == random)
                {
                    return output;
                }
            }
        }
        catch (const std::exception& e)
        {
            // TODO - Add MIAException here.
            return "ERROR";
        }
        return "ERROR";
    }


    void printRandomLinesFromFile(string filePath, int numberOfLines)
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
            throw error::MIAException(error::ErrorCode::FATAL_File_Not_Found);
        }
    }


    bool fileExists(const string& filePath)
    {
        return std::filesystem::exists(filePath);
    }


    std::vector<string> readAllLines(const string& filePath)
    {
        std::vector<string> lines;
        std::ifstream file(filePath);
        if (!file.is_open()) 
            return lines;

        string line;
        while (std::getline(file, line))
            lines.push_back(line);
        
        return lines;
    }


    void writeLinesToFile(const string& filePath, const std::vector<string>& lines)
    {
        std::ofstream file(filePath, std::ios::trunc);
        if (!file.is_open()) 
            return; // @TODO - Add MIA Exception.

        for (const auto& line : lines)
            file << line << "\n";
    }


    string readEntireFile(const string& filePath)
    {
        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        if (!file.is_open()) 
            return "";

        string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return content;
    }


    void appendLineToFile(const string& filePath, const string& line)
    {
        std::ofstream file(filePath, std::ios::app);
        if (!file.is_open()) 
            return; // @TODO - Add MIA Exception.

        file << line << "\n";
    }

    size_t countLinesInFile(const string& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open()) return 0;

        size_t count = 0;
        string line;
        while (std::getline(file, line))
        {
            ++count;
        }
        return count;
    }


    bool copyFile(const string& sourcePath, const string& destPath)
    {
        std::error_code ec;
        std::filesystem::copy_file(sourcePath, destPath, std::filesystem::copy_options::overwrite_existing, ec);
        return !ec; // @TODO - Add MIA Exception.
    }


    bool deleteFile(const string& filePath)
    {
        std::error_code ec;
        bool removed = std::filesystem::remove(filePath, ec);
        return removed && !ec; // @TODO - Add MIA Exception.
    }
} // namespace files
