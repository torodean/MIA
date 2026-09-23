/**
 * @file Logger_T.cpp
 * @author Antonius Torode
 * @date 06/15/2025
 * Description: Google tests for logger::Logger and free functions in Logger.hpp.
 */

#include "Logger.hpp"
#include "Paths.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
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

// Extract the first line containing the given substring, without the trailing newline.
std::string findLine(const std::string& contents, const std::string& substring)
{
    std::istringstream stream(contents);
    std::string line;
    while (std::getline(stream, line))
    {
        if (line.find(substring) != std::string::npos)
            return line;
    }
    return "";
}

/**
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
 * @brief Verifies that log with specified tags works.
 *
 * The test logs a message with optional tags and makes sure the tags appear in the
 * log message correctly.
 */
TEST(LoggerClass, LogWithTags)
{
    const std::string logFile = std::filesystem::absolute("logWithTags.log").string();
    cleanupFile(logFile);

    logger::Logger log(logFile);
    std::vector<std::string> tags1 = {"tag1"};
    log.log("Message with one tag.", tags1);

    std::vector<std::string> tags2 = {"tag1", "tag2"};
    log.log("Message with two tags.", tags2);

    std::string contents = readFileContents(logFile);

    EXPECT_NE(contents.find("[tag1]: Message with one tag."), std::string::npos);
    EXPECT_NE(contents.find("[tag1, tag2]: Message with two tags"), std::string::npos);

    cleanupFile(logFile);
}

/**
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

/**
 * @brief Verifies that the logToFile tags overload writes the tagged format.
 *
 * The test logs messages with the tags free function, once with tags and once with
 * an empty tag list, and checks the full log lines. The timestamp is a fixed 19
 * character string, so the expected lines are exact; this pins the spacing between
 * the timestamp, the tag, and the message.
 */
TEST(LoggerFreeFunctions, LogToFile_WithTags)
{
    const std::string logFile = std::filesystem::absolute("logToFileTags.log").string();
    cleanupFile(logFile);

    logger::logToFile("tagged message", logFile, {"tag1", "tag2"});
    logger::logToFile("untagged message", logFile, {});

    std::string contents = readFileContents(logFile);

    const std::string taggedLine = findLine(contents, "tagged message");
    ASSERT_FALSE(taggedLine.empty());
    EXPECT_EQ(taggedLine, taggedLine.substr(0, 19) + " [tag1, tag2]: tagged message");

    const std::string untaggedLine = findLine(contents, "untagged message");
    ASSERT_FALSE(untaggedLine.empty());
    EXPECT_EQ(untaggedLine, untaggedLine.substr(0, 19) + ": untagged message");

    cleanupFile(logFile);
}

/**
 * @brief Verifies that the logToDefaultFile tags overload writes to the default log.
 *
 * The test logs a clearly-labeled test message through the default file free function
 * and confirms the tagged line appears in the default log file. This intentionally
 * appends to the production log; the UNIT_TEST tag and message text make it clear to
 * anyone reading the log that the entry came from a unit test.
 */
TEST(LoggerFreeFunctions, LogToDefaultFile_WithTags)
{
    const std::string defaultLogFile = paths::getDefaultLogDirToUse() + "/" + logger::DEFAULT_LOG_FILE;

    const std::vector<std::string> tags = {"UNIT_TEST"};
    logger::logToDefaultFile("unit test - logToDefaultFile tags overload", tags);

    std::string contents = readFileContents(defaultLogFile);
    EXPECT_NE(contents.find("[UNIT_TEST]: unit test - logToDefaultFile tags overload"), std::string::npos)
        << "The test message was not found in " << defaultLogFile;
}

/**
 * @brief Verifies that log with an application name set works.
 *
 * The test logs a message with the application name set, once without tags and once
 * with tags, and checks the full log lines. The timestamp is a fixed 19 character
 * string, so the expected lines are exact; this pins the spacing between the
 * timestamp, the app name tag, and the message.
 */
TEST(LoggerClass, LogWithAppNameAndTags)
{
    const std::string logFile = std::filesystem::absolute("logWithAppName.log").string();
    cleanupFile(logFile);

    logger::Logger log(logFile);
    log.setApplicationName("testApp");
    log.log("test log message");

    // An empty tags vector with an app name set still emits the app name tag.
    log.log("app name only", {});

    std::vector<std::string> tags = {"tag1", "tag2"};
    log.log("app name and tags", tags);

    std::string contents = readFileContents(logFile);

    const std::string appNameLine = findLine(contents, "test log message");
    ASSERT_FALSE(appNameLine.empty());
    EXPECT_EQ(appNameLine, appNameLine.substr(0, 19) + " [testApp]: test log message");

    const std::string appNameOnlyLine = findLine(contents, "app name only");
    ASSERT_FALSE(appNameOnlyLine.empty());
    EXPECT_EQ(appNameOnlyLine, appNameOnlyLine.substr(0, 19) + " [testApp]: app name only");

    const std::string tagLine = findLine(contents, "app name and tags");
    ASSERT_FALSE(tagLine.empty());
    EXPECT_EQ(tagLine, tagLine.substr(0, 19) + " [testApp, tag1, tag2]: app name and tags");

    cleanupFile(logFile);
}

