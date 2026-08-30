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

namespace basic_utils
{
    /**
     * @test BasicUtilitiesTest.FindCharInString
     * @brief Tests the findCharInString function to locate characters in strings.
     */
    TEST(BasicUtilitiesTest, FindCharInString)
    {
        EXPECT_EQ(findCharInString("hello", 'e'), 1);
        EXPECT_EQ(findCharInString("hello", 'x'), -1);
        EXPECT_EQ(findCharInString("", 'a'), -1);
    }

    /**
     * @test BasicUtilitiesTest.StripFunctions
     * @brief Tests whitespace trimming functions: lstrip, rstrip, and strip.
     */
    TEST(BasicUtilitiesTest, StripFunctions)
    {
        EXPECT_EQ(lstrip("  abc"), "abc");
        EXPECT_EQ(rstrip("abc  "), "abc");
        EXPECT_EQ(strip("  abc  "), "abc");
        EXPECT_EQ(strip(" \t\nabc\n\t "), "abc");
    }

    /**
     * @test BasicUtilitiesTest.GetCurrentDateTime
     * @brief Tests that getCurrentDateTime returns a properly formatted timestamp.
     */
    TEST(BasicUtilitiesTest, GetCurrentDateTime)
    {
        auto dt = getCurrentDateTime();
        // Format should be "YYYY-MM-DD HH:MM:SS" (19 characters)
        EXPECT_EQ(dt.size(), 19);
        EXPECT_EQ(dt[4], '-');
        EXPECT_EQ(dt[7], '-');
        EXPECT_EQ(dt[10], ' ');
        EXPECT_EQ(dt[13], ':');
        EXPECT_EQ(dt[16], ':');
    }

    /**
     * @test BasicUtilitiesTest.EnsureDirectoryExists
     * @brief Tests ensureDirectoryExists function including creation and existence checks.
     */
    TEST(BasicUtilitiesTest, EnsureDirectoryExists)
    {
        std::string testDir = "test_dir_gtest";

        // Cleanup any previous test artifacts
        if (std::filesystem::exists(testDir)) std::filesystem::remove_all(testDir);

        // Should create directory if missing
        EXPECT_TRUE(ensureDirectoryExists(testDir, true));
        EXPECT_TRUE(std::filesystem::exists(testDir));
        EXPECT_TRUE(std::filesystem::is_directory(testDir));

        // Should return true if directory already exists
        EXPECT_TRUE(ensureDirectoryExists(testDir, false));

        // Cleanup after test
        std::filesystem::remove_all(testDir);
    }

    /**
     * @test BasicUtilitiesTest.EnsureFileExists
     * @brief Tests ensureFileExists function including file creation and edge cases.
     */
    TEST(BasicUtilitiesTest, EnsureFileExists)
    {
        std::string testFile = "test_file_gtest.txt";

        // Remove file if exists before test
        if (std::filesystem::exists(testFile)) std::filesystem::remove(testFile);

        // Should create the file if missing
        EXPECT_TRUE(ensureFileExists(testFile, true));
        EXPECT_TRUE(std::filesystem::exists(testFile));
        EXPECT_TRUE(std::filesystem::is_regular_file(testFile));

        // Should return true if file already exists
        EXPECT_TRUE(ensureFileExists(testFile, false));

        // Create directory with same name to simulate failure scenario
        std::filesystem::remove(testFile);
        std::filesystem::create_directory(testFile);

        // Should return false since path is a directory, not a file
        EXPECT_FALSE(ensureFileExists(testFile, false));

        // Cleanup after test
        std::filesystem::remove(testFile);
    }
} // namespace basic_utils
