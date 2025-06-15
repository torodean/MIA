/**
 * @file Logger_T.cpp
 * @author Antonius Torode
 * Creation Date: 06/15/2025
 * Description: Google tests for logger::Logger and free functions in Logger.hpp.
 */

#include "Logger.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>  // For std::remove
#include <filesystem>


// Helper to read entire contents of a file into a string
std::string readFileContents(const std::string& filename)
{
    std::ifstream in(filename);
    return std::string((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());
}

// Remove test log file if it exists
void cleanupFile(const std::string& filename)
{
    std::remove(filename.c_str());
}


TEST(LoggerFreeFunctions, LogToFile_WritesMessage)
{
    const std::string testMessage = "Test message for specific log";
const std::string testFile = std::filesystem::absolute("test_log.log").string();
    cleanupFile(testFile);

    logger::logToFile(testMessage, testFile, false);

    std::string contents = readFileContents(testFile);
    EXPECT_NE(contents.find(testMessage), std::string::npos);

    cleanupFile(testFile);
}

TEST(LoggerFreeFunctions, LogMethodCallToFile_FormatsCorrectly)
{
    const std::string methodName = "TestMethod";
    const std::string params = "param1=10, param2=hello";
    const std::string testFile = std::filesystem::absolute("method_call.log").string();
    cleanupFile(testFile);

    logger::logMethodCallToFile(methodName, testFile, params, false);

    std::string contents = readFileContents(testFile);
    EXPECT_NE(contents.find(methodName), std::string::npos);
    EXPECT_NE(contents.find(params), std::string::npos);

    cleanupFile(testFile);
}

TEST(LoggerClass, ConstructorWithFilename_UsesGivenFile)
{
    const std::string testFile = std::filesystem::absolute("custom_log.log").string();
    cleanupFile(testFile);

    logger::Logger log(testFile);
    EXPECT_EQ(log.getLogFile(), testFile);

    log.log("Logging to custom file");
    std::string contents = readFileContents(testFile);
    EXPECT_NE(contents.find("Logging to custom file"), std::string::npos);

    cleanupFile(testFile);
}

TEST(LoggerClass, SetLogFile_ChangesLogFile)
{
    const std::string file1 = std::filesystem::absolute("file1.log").string();
    const std::string file2 = std::filesystem::absolute("file2.log").string();
    cleanupFile(file1);
    cleanupFile(file2);

    logger::Logger log(file1);
    log.log("Message in file1");

    log.setLogFile(file2);
    EXPECT_EQ(log.getLogFile(), file2);

    log.log("Message in file2");

    std::string contents1 = readFileContents(file1);
    std::string contents2 = readFileContents(file2);

    EXPECT_NE(contents1.find("Message in file1"), std::string::npos);
    EXPECT_NE(contents2.find("Message in file2"), std::string::npos);

    cleanupFile(file1);
    cleanupFile(file2);
}

TEST(LoggerClass, LogMethodCall_LogsMethodNameAndParams)
{
    const std::string testFile = std::filesystem::absolute("method_log.log").string();
    cleanupFile(testFile);

    logger::Logger log(testFile);
    const std::string methodName = "SampleMethod";
    const std::string params = "x=42, y=hello";

    log.logMethodCall(methodName, params, false);

    std::string contents = readFileContents(testFile);
    EXPECT_NE(contents.find(methodName), std::string::npos);
    EXPECT_NE(contents.find(params), std::string::npos);

    cleanupFile(testFile);
}

