/**
 * @file CommandOption_T.cpp
 * @brief Unit tests for the CommandOption class using Google Test.
 * @author Antonius Torode
 * @date 06/15/2025
 *
 * Tests include help string formatting, valid parsing of each supported type,
 * and proper exception throwing on type mismatches.
 */

#include "CommandOption.hpp"
#include "gtest/gtest.h"

// Mocks for command_parser functions
namespace command_parser 
{
    void parseBoolFlag(int, char**, const std::string&, const std::string&, bool& out) 
    {
        out = true;
    }

    void parseIntOption(int, char**, const std::string&, const std::string&, int& out, bool) 
    {
        out = 42;
    }

    void parseDoubleOption(int, char**, const std::string&, const std::string&, double& out, bool) 
    {
        out = 3.14;
    }

    void parseStringOption(int, char**, const std::string&, const std::string&, std::string& out, bool) 
    {
        out = "parsed_value";
    }
}

using error::MIAException;
using error::ErrorCode;

/**
 * @brief Test help string formatting with short and long options.
 */
TEST(CommandOptionTest, HelpStringFormatting) 
{
    CommandOption opt("-f", "--file", "Specifies the file name.", CommandOption::stringOption);
    std::string help = opt.getHelp();
    EXPECT_NE(help.find("-f, --file"), std::string::npos);
    EXPECT_NE(help.find("Specifies the file name."), std::string::npos);
}

/**
 * @brief Test correct parsing dispatch for bool option.
 */
TEST(CommandOptionTest, GetBoolOptionSuccess) 
{
    CommandOption opt("-v", "--verbose", "Enable verbose mode.", CommandOption::boolOption);
    bool value = false;
    char* argv[] = { (char*)"prog", (char*)"--verbose" };
    opt.getOptionVal(2, argv, value);
    EXPECT_TRUE(value);
}

/**
 * @brief Test correct parsing dispatch for int option.
 */
TEST(CommandOptionTest, GetIntOptionSuccess) 
{
    CommandOption opt("-n", "--number", "Provide a number.", CommandOption::intOption);
    int value = 0;
    char* argv[] = { (char*)"prog", (char*)"--number", (char*)"42" };
    opt.getOptionVal(3, argv, value);
    EXPECT_EQ(value, 42);
}

/**
 * @brief Test correct parsing dispatch for double option.
 */
TEST(CommandOptionTest, GetDoubleOptionSuccess) 
{
    CommandOption opt("-d", "--double", "Provide a double.", CommandOption::doubleOption);
    double value = 0.0;
    char* argv[] = { (char*)"prog", (char*)"--double", (char*)"3.14" };
    opt.getOptionVal(3, argv, value);
    EXPECT_DOUBLE_EQ(value, 3.14);
}

/**
 * @brief Test correct parsing dispatch for string option.
 */
TEST(CommandOptionTest, GetStringOptionSuccess) 
{
    CommandOption opt("-s", "--string", "Provide a string.", CommandOption::stringOption);
    std::string value;
    char* argv[] = { (char*)"prog", (char*)"--string", (char*)"value" };
    opt.getOptionVal(3, argv, value);
    EXPECT_EQ(value, "parsed_value");
}

/**
 * @brief Test type mismatch throws MIAException.
 */
TEST(CommandOptionTest, ThrowsOnTypeMismatch) 
{
    CommandOption opt("-x", "--wrong", "Wrong type used.", CommandOption::intOption);
    try 
    {
        bool wrongType;
        char* argv[] = { (char*)"prog", (char*)"--wrong", (char*)"true" };
        opt.getOptionVal(3, argv, wrongType);
        FAIL() << "Expected MIAException not thrown";
    } 
    catch (const MIAException& ex) 
    {
        std::string msg = ex.what();
        EXPECT_NE(msg.find("Expected bool"), std::string::npos);
        EXPECT_EQ(ex.getCode(), ErrorCode::Invalid_Type_Requested);
    }
}

/**
 * @brief Test unsupported template type throws MIAException.
 */
TEST(CommandOptionTest, ThrowsOnUnsupportedTemplateType) 
{
    CommandOption opt("-u", "--unsupported", "Unsupported type test", CommandOption::unknownOption);
    struct Dummy {};
    try 
    {
        Dummy d;
        char* argv[] = { (char*)"prog", (char*)"--unsupported" };
        opt.getOptionVal(2, argv, d); // unsupported type
        FAIL() << "Expected MIAException not thrown";
    } 
    catch (const MIAException& ex) 
    {
        EXPECT_EQ(ex.getCode(), ErrorCode::Invalid_Type_Requested);
        EXPECT_NE(std::string(ex.what()).find("Unsupported template type"), std::string::npos);
    }
}

