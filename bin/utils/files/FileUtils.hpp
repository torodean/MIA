/**
 * @file FileUtils.hpp
 * @author Antonius Torode
 * @date 03/07/2021
 * @brief Provides utility functions for reading, writing, and processing files.
 */
#pragma once

#include <vector>
#include <string>

namespace files
{
	/**
	 * @brief Enum representation for different file types.
	 */
	enum FileType
	{
		Unknown, ///< An unknown file type.
		Mp3,     ///< An mp3 audio file.
		Wav      ///< A wav audio file.
	};

	/**
	 * @brief Converts a string to a file type.
	 * @param input The input string to convert.
	 */
	FileType stringToFileType(const std::string& input);

	/**
	 * @brief Stores metadata for a file.
	 */
	struct FileMetaData
	{
		std::string fullFilePath;
		std::string fileName;
		FileType type{Unknown};
	};

	/**
	 * @brief Constructs a FileMetaData object from a file.
	 * @param fileName The full path to the file to construct metadata from.
	 * @param verboseMode Enables verboseMode output (default = false).
	 * @return The constructed meta data of the file.
	 */
	FileMetaData getFileMetaData(const std::string& fileName,
	                             bool verboseMode = false);

    /**
     * @brief Returns the random line of a specified text file.
     * @param fileName Input file to parse.
     * @return Returns a random line of the file.
     */
    std::string getRandomLineOfFile(std::string& fileName);

    /**
     * @brief Takes a file as an input and prints an input number of random lines from it.
     * @param filePath The input file path to use.
     * @param numberOfLines The number of lines to return.
     * @throws MIAException if the file is not found or accessible.
     */
    void printRandomLinesFromFile(std::string filePath, int numberOfLines);

    /**
     * @brief Checks if a file exists at the specified path.
     * @param filePath Path to the file.
     * @return True if the file exists, false otherwise.
     */
    bool fileExists(const std::string& filePath);

    /**
     * @brief Reads all lines from a file into a vector of strings.
     * @param filePath Path to the file.
     * @return Vector containing all lines from the file.
     */
    std::vector<std::string> readAllLines(const std::string& filePath);

    /**
     * @brief Writes the given lines to a file, overwriting any existing content.
     * @param filePath Path to the file.
     * @param lines Lines to write.
     */
    void writeLinesToFile(const std::string& filePath, const std::vector<std::string>& lines);

    /**
     * @brief Reads the entire content of a file as a single string.
     * @param filePath Path to the file.
     * @return Content of the file.
     */
    std::string readEntireFile(const std::string& filePath);

    /**
     * @brief Appends a single line to the end of a file.
     * @param filePath Path to the file.
     * @param line Line to append.
     */
    void appendLineToFile(const std::string& filePath, const std::string& line);

    /**
     * @brief Counts the number of lines in a file.
     * @param filePath Path to the file.
     * @return Number of lines in the file.
     */
    size_t countLinesInFile(const std::string& filePath);

    /**
     * @brief Copies a file from source path to destination path.
     * @param sourcePath Path to the source file.
     * @param destPath Path to the destination file.
     * @return True if copy was successful, false otherwise.
     */
    bool copyFile(const std::string& sourcePath, const std::string& destPath);

    /**
     * @brief Deletes the specified file.
     * @param filePath Path to the file.
     * @return True if deletion was successful, false otherwise.
     */
    bool deleteFile(const std::string& filePath);
} // namespace files
