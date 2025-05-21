/**
 * File: FileUtils.hpp
 * Author: Antonius Torode
 * Created on: 03/07/2021
 * Description: Provides utility functions for reading, writing, and processing files.
 */
#pragma once

#include <string>

namespace files
{
    /**
     * Returns the random line of a specified text file.
     * @param fileName[std::string] - Input file to parse.
     * @return [std::string] - Returns a random line of the file.
     */
    static std::string getRandomLineOfFile(std::string& fileName);

    /**
     * Takes a file as an input and prints an input number of random lines from it.
     * @param filePath[std::string] - the input file path to use.
     * @param numberOfLines[int] - The number of lines to return.
     */
    [[deprecated("This method has changed. Review the new usage.")]]
    static void printRandomLinesFromFile(std::string filePath, int numberOfLines);

} // namespace files
