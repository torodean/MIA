/**
 * @file FileUtils_T.cpp
 * @author Antonius Torode
 * @date 05/21/2025
 * @brief Unit tests for FileUtils methods using Google Test.
 */

#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdio>  // for remove()

#include <gtest/gtest.h>

// Include the header we are testing methods for.
#include "FileUtils.hpp"
#include "TestingUtils.hpp"


namespace files
{
    /**
     * @brief TODO
     */
    class FileUtils_T : public ::testing::Test 
    {
    protected:
        std::string testFile;
        const std::vector<std::string> fileLines = {"Line 1", "Line 2", "Line 3"};

        void SetUp() override 
        {
            // Create a test file with known content.
            testFile = testing::createTempTestFile(fileLines);
        }

        void TearDown() override 
        {
            // Cleanup test files.
            testing::removeFile(testFile);
        }
    }; // class FileUtils_T


    /**
     * @brief Tests that getRandomLineOfFile returns a line from the file.
     */
    TEST_F(FileUtils_T, GetRandomLineOfFileReturnsLineFromFile) 
    {
        std::vector<std::string> lines = {"Line1", "Line2", "Line3", "Line4"};
        std::string filename = testing::createTempTestFile(lines);

        std::string fileNameParam = filename;
        std::string line = getRandomLineOfFile(fileNameParam);

        // The line returned should be one of the lines in the file.
        EXPECT_NE(std::find(lines.begin(), lines.end(), line), lines.end());

        testing::removeFile(filename);
    }


    /**
     * @brief TODO
     */
    TEST_F(FileUtils_T, GetRandomLineOfFileEmptyFileReturnsError) 
    {
        std::string filename = testing::createTempTestFile({});

        std::string fileNameParam = filename;
        std::string line = getRandomLineOfFile(fileNameParam);

        EXPECT_EQ(line, "ERROR");

        testing::removeFile(filename);
    }


    /**
     * @brief TODO
     */
    TEST_F(FileUtils_T, PrintRandomLinesFromFilePrintsCorrectNumberOfLines) 
    {
        std::vector<std::string> lines = {"Alpha", "Bravo", "Charlie", "Delta"};
        std::string filename = testing::createTempTestFile(lines);

        // Redirect cout.
        std::stringstream buffer;
        std::streambuf* oldCoutBuf = std::cout.rdbuf(buffer.rdbuf());

        printRandomLinesFromFile(filename, 3);

        // Restore cout.
        std::cout.rdbuf(oldCoutBuf);

        std::string output = buffer.str();

        // Check that output has at least 3 lines printed (3 random lines + 3 newlines).
        std::istringstream iss(output);
        std::string line;
        int lineCount = 0;
        while (std::getline(iss, line))
        {
            ++lineCount;
            // Each line should be one of the original lines.
            EXPECT_NE(std::find(lines.begin(), lines.end(), line), lines.end());
        }
        EXPECT_EQ(lineCount, 3);

        testing::removeFile(filename);
    }


    /**
     * @brief Tests that fileExists returns true for existing file.
     */
    TEST_F(FileUtils_T, FileExistsTrue)
    {
        EXPECT_TRUE(fileExists(testFile));
    }


    /**
     * @brief Tests that fileExists returns false for non-existent file.
     */
    TEST_F(FileUtils_T, FileExistsFalse)
    {
        EXPECT_FALSE(fileExists("nonexistentfile.txt"));
    }


    /**
     * @brief Tests that readAllLines reads all of the file lines correctly.
     */
    TEST_F(FileUtils_T, ReadAllLines)
    {
        std::vector<std::string> lines = readAllLines(testFile);
        ASSERT_EQ(lines.size(), 3);
        EXPECT_EQ(lines[0], "Line 1");
        EXPECT_EQ(lines[1], "Line 2");
        EXPECT_EQ(lines[2], "Line 3");
    }


    /**
     * @brief Tests that writeLinesToFile overwrites the file and writes the lines.
     */
    TEST_F(FileUtils_T, WriteLinesToFileAndReadBack)
    {
        std::vector<std::string> linesToWrite = {"A", "B", "C"};
        writeLinesToFile(testFile, linesToWrite);

        std::vector<std::string> linesRead = readAllLines(testFile);
        ASSERT_EQ(linesRead.size(), 3);
        EXPECT_EQ(linesRead[0], "A");
        EXPECT_EQ(linesRead[1], "B");
        EXPECT_EQ(linesRead[2], "C");
    }


    /**
     * @brief Tests that all lines appear in readEntireFile's output.
     */
    TEST_F(FileUtils_T, ReadEntireFile)
    {
        std::string content = readEntireFile(testFile);
        EXPECT_NE(content.find("Line 1"), std::string::npos);
        EXPECT_NE(content.find("Line 2"), std::string::npos);
        EXPECT_NE(content.find("Line 3"), std::string::npos);
    }


    /**
     * @brief Tests that appendLineToFile correctly appens a line to a file.
     */
    TEST_F(FileUtils_T, AppendLineToFile)
    {
        appendLineToFile(testFile, "Line 4");

        std::vector<std::string> lines = readAllLines(testFile);
        ASSERT_EQ(lines.size(), 4);
        EXPECT_EQ(lines.back(), "Line 4");
    }


    /**
     * @brief Tests that countLinesInFile returns the correct number of lines for a test file.
     */
    TEST_F(FileUtils_T, CountLinesInFile)
    {
        size_t lineCount = countLinesInFile(testFile);
        EXPECT_EQ(lineCount, 3);
    }


    /**
     * @brief Tests that copyFile works as it should by copying the file and its contents.
     */
    TEST_F(FileUtils_T, CopyFileSuccess)
    {
        const std::string copyFileName = "copiedTestfile.txt";
        bool copied = copyFile(testFile, copyFileName);
        EXPECT_TRUE(copied);
        EXPECT_TRUE(fileExists(copyFileName));

        std::vector<std::string> original = readAllLines(testFile);
        std::vector<std::string> copiedLines = readAllLines(copyFileName);
        EXPECT_EQ(original, copiedLines);
        testing::removeFile(copyFileName);
    }


    /**
     * @brief Tests that copyFile returns correctly for non-existent files.
     */
    TEST_F(FileUtils_T, CopyFileFailure)
    {
        const std::string copyFileName = "copiedTestfile.txt";
        bool copied = copyFile("nonexistentfile.txt", copyFileName);
        EXPECT_FALSE(copied);
        EXPECT_FALSE(fileExists(copyFileName));
    }


    /**
     * @brief Tests that deleteFile correctly can delete a file.
     */
    TEST_F(FileUtils_T, DeleteFileSuccess)
    {
        EXPECT_TRUE(fileExists(testFile));
        bool deleted = deleteFile(testFile);
        EXPECT_TRUE(deleted);
        EXPECT_FALSE(fileExists(testFile));
    }


    /**
     * @brief Tests that deleteFile returns false when the file to delete does not exist.
     */
    TEST_F(FileUtils_T, DeleteFileFailure)
    {
        bool deleted = deleteFile("nonexistentfile.txt");
        EXPECT_FALSE(deleted);
    }
} // namespace files
