/**
 * @file BasicUtilities_T.cpp
 * @brief Unit tests for BasicUtilities namespace functions using Google Test.
 * 
 * @author Antonius Torode
 * @date 06/15/2025
 */

#include "BasicUtilities.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

/**
 * @brief Tests the findCharInString function to locate characters in strings.
 */
TEST(BasicUtilitiesTest, FindCharInString)
{
    EXPECT_EQ(BasicUtilities::findCharInString("hello", 'e'), 1);
    EXPECT_EQ(BasicUtilities::findCharInString("hello", 'x'), -1);
    EXPECT_EQ(BasicUtilities::findCharInString("", 'a'), -1);
}

/**
 * @brief Tests whitespace trimming functions: lstrip, rstrip, and strip.
 */
TEST(BasicUtilitiesTest, StripFunctions)
{
    EXPECT_EQ(BasicUtilities::lstrip("  abc"), "abc");
    EXPECT_EQ(BasicUtilities::rstrip("abc  "), "abc");
    EXPECT_EQ(BasicUtilities::strip("  abc  "), "abc");
    EXPECT_EQ(BasicUtilities::strip(" \t\nabc\n\t "), "abc");
}

/**
 * @brief Tests that getCurrentDateTime returns a properly formatted timestamp.
 */
TEST(BasicUtilitiesTest, GetCurrentDateTime)
{
    auto dt = BasicUtilities::getCurrentDateTime();
    // Format should be "YYYY-MM-DD HH:MM:SS" (19 characters)
    EXPECT_EQ(dt.size(), 19);
    EXPECT_EQ(dt[4], '-');
    EXPECT_EQ(dt[7], '-');
    EXPECT_EQ(dt[10], ' ');
    EXPECT_EQ(dt[13], ':');
    EXPECT_EQ(dt[16], ':');
}

/**
 * @brief Tests ensureDirectoryExists function including creation and existence checks.
 */
TEST(BasicUtilitiesTest, EnsureDirectoryExists)
{
    std::string testDir = "test_dir_gtest";

    // Cleanup any previous test artifacts
    if (fs::exists(testDir)) fs::remove_all(testDir);

    // Should create directory if missing
    EXPECT_TRUE(BasicUtilities::ensureDirectoryExists(testDir, true));
    EXPECT_TRUE(fs::exists(testDir));
    EXPECT_TRUE(fs::is_directory(testDir));

    // Should return true if directory already exists
    EXPECT_TRUE(BasicUtilities::ensureDirectoryExists(testDir, false));

    // Cleanup after test
    fs::remove_all(testDir);
}

/**
 * @brief Tests ensureFileExists function including file creation and edge cases.
 */
TEST(BasicUtilitiesTest, EnsureFileExists)
{
    std::string testFile = "test_file_gtest.txt";

    // Remove file if exists before test
    if (fs::exists(testFile)) fs::remove(testFile);

    // Should create the file if missing
    EXPECT_TRUE(BasicUtilities::ensureFileExists(testFile, true));
    EXPECT_TRUE(fs::exists(testFile));
    EXPECT_TRUE(fs::is_regular_file(testFile));

    // Should return true if file already exists
    EXPECT_TRUE(BasicUtilities::ensureFileExists(testFile, false));

    // Create directory with same name to simulate failure scenario
    fs::remove(testFile);
    fs::create_directory(testFile);

    // Should return false since path is a directory, not a file
    EXPECT_FALSE(BasicUtilities::ensureFileExists(testFile, false));

    // Cleanup after test
    fs::remove(testFile);
}

/**
 * @brief Main entry point for Google Test.
 */
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

