/**
 * @file Logger_T.cpp
 * @author Antonius Torode
 * @date 06/15/2025
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

/**
 * @test LoggerFreeFunctions.LogToFile_WritesMessage
 * @brief Verifies that logger::logToFile writes the provided message to the specified file.
 *
 * The test removes any existing test file, calls logger::logToFile with a
 * known message, then reads the file contents and confirms that the message
 * appears in the file.
 */
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

/**
 * @test LoggerFreeFunctions.LogMethodCallToFile_FormatsCorrectly
 * @brief Verifies that logger::logMethodCallToFile logs a method call entry containing
 *        the method name and parameter string.
 *
 * The test writes a formatted method call to a log file and confirms that both the
 * method name and parameter string appear in the resulting log output.
 */
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

/**
 * @test LoggerClass.ConstructorWithFilename_UsesGivenFile
 * @brief Verifies that constructing logger::Logger with a filename sets the active log file.
 *
 * The test constructs a Logger with a specific file path, checks that getLogFile()
 * returns that path, writes a log entry, and confirms the message is written to the
 * expected file.
 */
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

/**
 * @test LoggerClass.SetLogFile_ChangesLogFile
 * @brief Verifies that setLogFile changes the destination file used for logging.
 *
 * The test logs a message to the initial file, switches the log file using
 * setLogFile(), logs another message, and verifies each message appears in the
 * correct file.
 */
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

/**
 * @test LoggerClass.LogMethodCall_LogsMethodNameAndParams
 * @brief Verifies that Logger::logMethodCall records both the method name and parameters.
 *
 * The test writes a method call entry using Logger::logMethodCall and confirms that
 * the resulting log entry contains the specified method name and parameter string.
 */
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

