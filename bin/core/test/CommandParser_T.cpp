/**
 * @file CommandParser_T.cpp
 * @author Antonius Torode
 * @date 06/15/2025
 * @brief Unit tests for command_parser utilities.
 *
 * Tests include parsing of boolean, integer, double, and string options,
 * along with exception handling for missing or invalid inputs.
 */

#include <gtest/gtest.h>

#include "CommandParser.hpp"
#include "MIAException.hpp"

namespace command_parser
{
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
        EXPECT_THROW({parseIntOption(argc, argv, "-c", "--count", value, true);}, 
                     error::MIAException);
    }


    /**
     * @brief Test exception thrown on invalid integer value.
     */
    TEST(CommandParserTest, ParseIntOptionInvalidValue) 
    {
        char* argv[] = { (char*)"prog", (char*)"--count", (char*)"abc" };
        int argc = 3;
        int value = 0;
        EXPECT_THROW({parseIntOption(argc, argv, "-c", "--count", value);}, 
                     error::MIAException);
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
        EXPECT_THROW({parseDoubleOption(argc, argv, "-t", "--threshold", value);}, 
                     error::MIAException);
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
        EXPECT_THROW({parseStringOption(argc, argv, "-o", "--output", out, true);}, 
                     error::MIAException);
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


    /**
     * @brief Test exception thrown on duplicated boolean flag.
     */
    TEST(CommandParserTest, ParseBoolFlagDuplicateThrows)
    {
        char* argv[] = { (char*)"prog", (char*)"--verbose", (char*)"--verbose" };
        int argc = 3;
        bool out = false;
        EXPECT_THROW({parseBoolFlag(argc, argv, "-v", "--verbose", out);},
                     error::MIAException);
    }


    /**
     * @brief Test the short and long forms of one boolean option count as a duplicate.
     */
    TEST(CommandParserTest, ParseBoolFlagMixedFormsDuplicateThrows)
    {
        char* argv[] = { (char*)"prog", (char*)"-v", (char*)"--verbose" };
        int argc = 3;
        bool out = false;
        EXPECT_THROW({parseBoolFlag(argc, argv, "-v", "--verbose", out);},
                     error::MIAException);
    }


    /**
     * @brief Test exception thrown on duplicated integer option.
     */
    TEST(CommandParserTest, ParseIntOptionDuplicateThrows)
    {
        char* argv[] = { (char*)"prog", (char*)"--count", (char*)"5", (char*)"--count", (char*)"9" };
        int argc = 5;
        int value = 0;
        EXPECT_THROW({parseIntOption(argc, argv, "-c", "--count", value);},
                     error::MIAException);
    }


    /**
     * @brief Test exception thrown when the second occurrence of an integer option has no value.
     */
    TEST(CommandParserTest, ParseIntOptionDuplicateAtEndThrows)
    {
        char* argv[] = { (char*)"prog", (char*)"--count", (char*)"5", (char*)"--count" };
        int argc = 4;
        int value = 0;
        EXPECT_THROW({parseIntOption(argc, argv, "-c", "--count", value);},
                     error::MIAException);
    }


    /**
     * @brief Test exception thrown on duplicated unsigned integer option.
     */
    TEST(CommandParserTest, ParseUnsignedIntOptionDuplicateThrows)
    {
        char* argv[] = { (char*)"prog", (char*)"--level", (char*)"1", (char*)"--level", (char*)"2" };
        int argc = 5;
        unsigned int value = 0;
        EXPECT_THROW({parseUnsignedIntOption(argc, argv, "-l", "--level", value);},
                     error::MIAException);
    }


    /**
     * @brief Test exception thrown on duplicated double option.
     */
    TEST(CommandParserTest, ParseDoubleOptionDuplicateThrows)
    {
        char* argv[] = { (char*)"prog", (char*)"--limit", (char*)"1.5", (char*)"--limit", (char*)"2.5" };
        int argc = 5;
        double value = 0.0;
        EXPECT_THROW({parseDoubleOption(argc, argv, "-l", "--limit", value);},
                     error::MIAException);
    }


    /**
     * @brief Test exception thrown on duplicated string option.
     */
    TEST(CommandParserTest, ParseStringOptionDuplicateThrows)
    {
        char* argv[] = { (char*)"prog", (char*)"--output", (char*)"a.txt", (char*)"--output", (char*)"b.txt" };
        int argc = 5;
        std::string out;
        EXPECT_THROW({parseStringOption(argc, argv, "-o", "--output", out);},
                     error::MIAException);
    }


    /**
     * @brief Test the duplicate exception carries the Duplicate_Argument error code.
     */
    TEST(CommandParserTest, ParseDoubleOptionDuplicateErrorCode)
    {
        char* argv[] = { (char*)"prog", (char*)"--limit", (char*)"1.5", (char*)"--limit", (char*)"2.5" };
        int argc = 5;
        double value = 0.0;
        try
        {
            parseDoubleOption(argc, argv, "-l", "--limit", value);
            FAIL() << "Expected MIAException not thrown";
        }
        catch (const error::MIAException& ex)
        {
            EXPECT_EQ(ex.getCode(), error::ErrorCode::Duplicate_Argument);
        }
    }
} // namespace command_parser
