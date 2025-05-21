#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <cstdio>  // for remove()

// Include the header we are testing methods for.
#include "FileUtils.hpp"


namespace files
{
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
} // namespace files
