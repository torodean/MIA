/**
 * @file CommandParser_T.cpp
 * @brief Unit tests for command_parser utilities.
 * @author Antonius Torode
 * @date 06/15/2025
 *
 * Tests include parsing of boolean, integer, double, and string options,
 * along with exception handling for missing or invalid inputs.
 */

#include "CommandParser.hpp"
#include "MIAException.hpp"
#include "gtest/gtest.h"

using namespace command_parser;
using error::MIAException;
using error::ErrorCode;

/**
 * @brief Test successful parsing of a boolean flag using long option.
 */
TEST(CommandParserTest, ParseBoolFlagLongOption) 
{
    char* argv[] = { (char*)"prog", (char*)"--verbose" };
    int argc = 2;
    bool out = false;
    parseBoolFlag(argc, argv, "-v", "--verbose", out);
    EXPECT_TRUE(out);
}

/**
 * @brief Test successful parsing of a boolean flag using short option.
 */
TEST(CommandParserTest, ParseBoolFlagShortOption) 
{
    char* argv[] = { (char*)"prog", (char*)"-v" };
    int argc = 2;
    bool out = false;
    parseBoolFlag(argc, argv, "-v", "--verbose", out);
    EXPECT_TRUE(out);
}

/**
 * @brief Test successful parsing of integer value.
 */
TEST(CommandParserTest, ParseIntOptionValid) 
{
    char* argv[] = { (char*)"prog", (char*)"--count", (char*)"10" };
    int argc = 3;
    int value = 0;
    parseIntOption(argc, argv, "-c", "--count", value);
    EXPECT_EQ(value, 10);
}

/**
 * @brief Test exception thrown on missing required integer option.
 */
TEST(CommandParserTest, ParseIntOptionMissingRequired) 
{
    char* argv[] = { (char*)"prog" };
    int argc = 1;
    int value = 0;
    EXPECT_THROW({
        parseIntOption(argc, argv, "-c", "--count", value, true);
    }, MIAException);
}

/**
 * @brief Test exception thrown on invalid integer value.
 */
TEST(CommandParserTest, ParseIntOptionInvalidValue) 
{
    char* argv[] = { (char*)"prog", (char*)"--count", (char*)"abc" };
    int argc = 3;
    int value = 0;
    EXPECT_THROW({
        parseIntOption(argc, argv, "-c", "--count", value);
    }, MIAException);
}

/**
 * @brief Test successful parsing of double value.
 */
TEST(CommandParserTest, ParseDoubleOptionValid) 
{
    char* argv[] = { (char*)"prog", (char*)"--threshold", (char*)"0.75" };
    int argc = 3;
    double value = 0.0;
    parseDoubleOption(argc, argv, "-t", "--threshold", value);
    EXPECT_DOUBLE_EQ(value, 0.75);
}

/**
 * @brief Test exception thrown on invalid double value.
 */
TEST(CommandParserTest, ParseDoubleOptionInvalidValue) 
{
    char* argv[] = { (char*)"prog", (char*)"--threshold", (char*)"bad" };
    int argc = 3;
    double value = 0.0;
    EXPECT_THROW({
        parseDoubleOption(argc, argv, "-t", "--threshold", value);
    }, MIAException);
}

/**
 * @brief Test successful parsing of string option.
 */
TEST(CommandParserTest, ParseStringOptionValid) 
{
    char* argv[] = { (char*)"prog", (char*)"--output", (char*)"file.txt" };
    int argc = 3;
    std::string out;
    parseStringOption(argc, argv, "-o", "--output", out);
    EXPECT_EQ(out, "file.txt");
}

/**
 * @brief Test exception thrown when required string option is missing.
 */
TEST(CommandParserTest, ParseStringOptionMissingRequired) 
{
    char* argv[] = { (char*)"prog" };
    int argc = 1;
    std::string out;
    EXPECT_THROW({
        parseStringOption(argc, argv, "-o", "--output", out, true);
    }, MIAException);
}

/**
 * @brief Test no shortArg option.
 */
TEST(CommandParserTest, ParseOptionMissingShortArg) 
{
    char* argv[] = { (char*)"prog", (char*)"--output", (char*)"file.txt" };
    int argc = 3;
    std::string out;
    parseStringOption(argc, argv, "", "--output", out);
    EXPECT_EQ(out, "file.txt");
}

