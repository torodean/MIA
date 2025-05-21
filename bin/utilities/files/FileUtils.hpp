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

    /**
     * Checks if a file exists at the specified path.
     * @param filePath [const std::string&] - Path to the file.
     * @return [bool] - True if the file exists, false otherwise.
     */
    static bool fileExists(const std::string& filePath);

    /**
     * Reads all lines from a file into a vector of strings.
     * @param filePath[const std::string&] - Path to the file.
     * @return [std::vector<std::string>] - Vector containing all lines from the file.
     */
    static std::vector<std::string> readAllLines(const std::string& filePath);

    /**
     * Writes the given lines to a file, overwriting any existing content.
     * @param filePath[const std::string&] - Path to the file.
     * @param lines[const std::vector<std::string>&] - Lines to write.
     * @return [bool] - True if write was successful, false otherwise.
     */
    static bool writeLinesToFile(const std::string& filePath, const std::vector<std::string>& lines);

    /**
     * Reads the entire content of a file as a single string.
     * @param filePath[const std::string&] - Path to the file.
     * @return [std::string] - Content of the file.
     */
    static std::string readEntireFile(const std::string& filePath);

    /**
     * Appends a single line to the end of a file.
     * @param filePath[const std::string&] - Path to the file.
     * @param line[const std::string&] - Line to append.
     * @return [bool] - True if append was successful, false otherwise.
     */
    static bool appendLineToFile(const std::string& filePath, const std::string& line);

    /**
     * Counts the number of lines in a file.
     * @param filePath[const std::string&] - Path to the file.
     * @return [size_t] - Number of lines in the file.
     */
    static size_t countLinesInFile(const std::string& filePath);

    /**
     * Copies a file from source path to destination path.
     * @param sourcePath[const std::string&] - Path to the source file.
     * @param destPath[const std::string&] - Path to the destination file.
     * @return [bool] - True if copy was successful, false otherwise.
     */
    static bool copyFile(const std::string& sourcePath, const std::string& destPath);

    /**
     * Deletes the specified file.
     * @param filePath[const std::string&] - Path to the file.
     * @return [bool] - True if deletion was successful, false otherwise.
     */
    static bool deleteFile(const std::string& filePath);
} // namespace files
