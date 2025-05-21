/**
 * File: FileUtils.hpp
 * Author: Antonius Torode
 * Created on: 03/07/2021
 * Description: Provides utility functions for reading, writing, and processing files.
 */

namespace files
{
    /**
     * Returns the random line of a specified text file.
     * @param fileName[std::string] Input file to parse.
     * @return [std::string] Returns a random line of the file.
     */
    static std::string getRandomLineOfFile(std::string& fileName);

    /**
     * Takes a file as an input and prints an input number of random lines from it.
     * @param useDefaultPath[bool] - true if using the default MIA inputFilePath from the MIAConfig.MIA file.
     * @param numberOfLines[int] - The number of lines to return
     */
    static void printRandomLinesFromFile(bool useDefaultPath, int numberOfLines);

} // namespace files
