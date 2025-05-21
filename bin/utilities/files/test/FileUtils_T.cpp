#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdio>  // for remove()

// Include the header we are testing methods for.
#include "FileUtils.hpp"


namespace files
{
    class FileUtils_T : public ::testing::Test 
    {
    protected:
        const std::string testFile = "testfile.txt";
        const std::string copyFileName = "copyfile.txt";
    
        void SetUp() override 
        {
            // Create a test file with known content
            std::ofstream file(testFile);
            file << "Line 1\nLine 2\nLine 3\n";
            file.close();
        }
    
        void TearDown() override 
        {
            // Cleanup test files
            if (fs::exists(testFile)) fs::remove(testFile);
            if (fs::exists(copyFileName)) fs::remove(copyFileName);
        }
    }; // class FileUtils_T

    // Helper: Create a temp file with given lines
    static std::string createTempFile(const std::vector<std::string>& lines) 
    {
        std::string filename = "temp_test_file.txt";
        std::ofstream ofs(filename);
        for (const auto& line : lines) 
        {
            ofs << line << "\n";
        }
        ofs.close();
        return filename;
    }
    
    // Helper: Remove temp file
    static void removeTempFile(const std::string& filename) 
    {
        std::remove(filename.c_str());
    }
    
    TEST(FileUtilsTest, GetRandomLineOfFileReturnsLineFromFile) 
    {
        std::vector<std::string> lines = {"Line1", "Line2", "Line3", "Line4"};
        std::string filename = createTempFile(lines);
        
        std::string fileNameParam = filename;
        std::string line = getRandomLineOfFile(fileNameParam);
        
        // The line returned should be one of the lines in the file
        EXPECT_NE(std::find(lines.begin(), lines.end(), line), lines.end());
        
        removeTempFile(filename);
    }
    
    TEST(FileUtilsTest, GetRandomLineOfFileEmptyFileReturnsError) 
    {
        std::string filename = createTempFile({});
        
        std::string fileNameParam = filename;
        std::string line = FileUtils::getRandomLineOfFile(fileNameParam);
        
        EXPECT_EQ(line, "ERROR");
        
        removeTempFile(filename);
    }
    
    TEST(FileUtilsTest, PrintRandomLinesFromFilePrintsCorrectNumberOfLines) 
    {
        std::vector<std::string> lines = {"Alpha", "Bravo", "Charlie", "Delta"};
        std::string filename = createTempFile(lines);
    
        // Redirect cout
        std::stringstream buffer;
        std::streambuf* oldCoutBuf = std::cout.rdbuf(buffer.rdbuf());
    
        FileUtils::printRandomLinesFromFile(filename, 3);
    
        // Restore cout
        std::cout.rdbuf(oldCoutBuf);
    
        std::string output = buffer.str();
    
        // Check that output has at least 3 lines printed (3 random lines + 3 newlines)
        std::istringstream iss(output);
        std::string line;
        int lineCount = 0;
        while (std::getline(iss, line))
        {
            ++lineCount;
            // Each line should be one of the original lines
            EXPECT_NE(std::find(lines.begin(), lines.end(), line), lines.end());
        }
        EXPECT_EQ(lineCount, 3);
    
        removeTempFile(filename);
    }

    TEST_F(FileUtilsTest, FileExistsTrue)
    {
        EXPECT_TRUE(fileExists(testFile));
    }
    
    TEST_F(FileUtilsTest, FileExistsFalse)
    {
        EXPECT_FALSE(fileExists("nonexistentfile.txt"));
    }
    
    TEST_F(FileUtilsTest, ReadAllLines)
    {
        std::vector<std::string> lines = readAllLines(testFile);
        ASSERT_EQ(lines.size(), 3);
        EXPECT_EQ(lines[0], "Line 1");
        EXPECT_EQ(lines[1], "Line 2");
        EXPECT_EQ(lines[2], "Line 3");
    }
    
    TEST_F(FileUtilsTest, WriteLinesToFileAndReadBack)
    {
        std::vector<std::string> linesToWrite = {"A", "B", "C"};
        writeLinesToFile(testFile, linesToWrite);
    
        std::vector<std::string> linesRead = readAllLines(testFile);
        ASSERT_EQ(linesRead.size(), 3);
        EXPECT_EQ(linesRead[0], "A");
        EXPECT_EQ(linesRead[1], "B");
        EXPECT_EQ(linesRead[2], "C");
    }
    
    TEST_F(FileUtilsTest, ReadEntireFile)
    {
        std::string content = readEntireFile(testFile);
        EXPECT_NE(content.find("Line 1"), std::string::npos);
        EXPECT_NE(content.find("Line 2"), std::string::npos);
        EXPECT_NE(content.find("Line 3"), std::string::npos);
    }
    
    TEST_F(FileUtilsTest, AppendLineToFile)
    {
        appendLineToFile(testFile, "Line 4");
    
        std::vector<std::string> lines = readAllLines(testFile);
        ASSERT_EQ(lines.size(), 4);
        EXPECT_EQ(lines.back(), "Line 4");
    }
    
    TEST_F(FileUtilsTest, CountLinesInFile)
    {
        size_t lineCount = countLinesInFile(testFile);
        EXPECT_EQ(lineCount, 3);
    }
    
    TEST_F(FileUtilsTest, CopyFileSuccess)
    {
        bool copied = copyFile(testFile, copyFileName);
        EXPECT_TRUE(copied);
        EXPECT_TRUE(fileExists(copyFileName));
    
        std::vector<std::string> original = readAllLines(testFile);
        std::vector<std::string> copiedLines = readAllLines(copyFileName);
        EXPECT_EQ(original, copiedLines);
    }
    
    TEST_F(FileUtilsTest, CopyFileFailure)
    {
        bool copied = copyFile("nonexistentfile.txt", copyFileName);
        EXPECT_FALSE(copied);
        EXPECT_FALSE(fileExists(copyFileName));
    }
    
    TEST_F(FileUtilsTest, DeleteFileSuccess)
    {
        EXPECT_TRUE(fileExists(testFile));
        bool deleted = deleteFile(testFile);
        EXPECT_TRUE(deleted);
        EXPECT_FALSE(fileExists(testFile));
    }
    
    TEST_F(FileUtilsTest, DeleteFileFailure)
    {
        bool deleted = deleteFile("nonexistentfile.txt");
        EXPECT_FALSE(deleted);
    }
} // namespace files
