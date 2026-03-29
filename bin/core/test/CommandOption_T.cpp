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

/**
 * @brief Mock implementations for command-line parsing helpers.
 *
 * These functions replace the real command_parser implementations during
 * unit testing to isolate CommandOption behavior. Each stub simulates a
 * successful parse operation by assigning deterministic values to the
 * output parameter without performing any real command-line parsing.
 */
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
 * @test CommandOptionTest.HelpStringFormatting
 * @brief Verifies that CommandOption generates a correctly formatted help string.
 *
 * The test constructs an option with both short and long forms and checks
 * that the resulting help string includes the combined option identifiers
 * and the provided description text.
 */
TEST(CommandOptionTest, HelpStringFormatting) 
{
    CommandOption opt("-f", "--file", "Specifies the file name.", CommandOption::STRING_OPTION);
    std::string help = opt.getHelp();
    EXPECT_NE(help.find("-f, --file"), std::string::npos);
    EXPECT_NE(help.find("Specifies the file name."), std::string::npos);
}

/**
 * @test CommandOptionTest.GetBoolOptionSuccess
 * @brief Verifies that CommandOption correctly dispatches boolean options
 *        to the boolean parsing routine.
 */
TEST(CommandOptionTest, GetBoolOptionSuccess) 
{
    CommandOption opt("-v", "--verbose", "Enable verbose mode.", CommandOption::BOOL_OPTION);
    bool value = false;
    char* argv[] = { (char*)"prog", (char*)"--verbose" };
    opt.getOptionVal(2, argv, value);
    EXPECT_TRUE(value);
}

/**
 * @test CommandOptionTest.GetIntOptionSuccess
 * @brief Verifies that integer command-line options are parsed correctly.
 */
TEST(CommandOptionTest, GetIntOptionSuccess) 
{
    CommandOption opt("-n", "--number", "Provide a number.", CommandOption::INT_OPTION);
    int value = 0;
    char* argv[] = { (char*)"prog", (char*)"--number", (char*)"42" };
    opt.getOptionVal(3, argv, value);
    EXPECT_EQ(value, 42);
}

/**
 * @test CommandOptionTest.GetDoubleOptionSuccess
 * @brief Verifies that double command-line options are parsed correctly.
 */
TEST(CommandOptionTest, GetDoubleOptionSuccess) 
{
    CommandOption opt("-d", "--double", "Provide a double.", CommandOption::DOUBLE_OPTION);
    double value = 0.0;
    char* argv[] = { (char*)"prog", (char*)"--double", (char*)"3.14" };
    opt.getOptionVal(3, argv, value);
    EXPECT_DOUBLE_EQ(value, 3.14);
}

/**
 * @test CommandOptionTest.GetStringOptionSuccess
 * @brief Verifies that string command-line options are parsed correctly.
 */
TEST(CommandOptionTest, GetStringOptionSuccess) 
{
    CommandOption opt("-s", "--string", "Provide a string.", CommandOption::STRING_OPTION);
    std::string value;
    char* argv[] = { (char*)"prog", (char*)"--string", (char*)"value" };
    opt.getOptionVal(3, argv, value);
    EXPECT_EQ(value, "parsed_value");
}

/**
 * @test CommandOptionTest.ThrowsOnTypeMismatch
 * @brief Verifies that requesting an option value with an incompatible type
 *        results in an exception.
 *
 * The test attempts to retrieve an integer option using a boolean variable.
 * The operation should throw MIAException with the error code
 * Invalid_Type_Requested and a message indicating the expected type.
 */
TEST(CommandOptionTest, ThrowsOnTypeMismatch) 
{
    CommandOption opt("-x", "--wrong", "Wrong type used.", CommandOption::INT_OPTION);
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
 * @test CommandOptionTest.ThrowsOnUnsupportedTemplateType
 * @brief Verifies that unsupported template types passed to getOptionVal()
 *        trigger an exception.
 *
 * The test provides a custom type that is not supported by the CommandOption
 * parsing interface. The method is expected to throw MIAException with the
 * Invalid_Type_Requested error code and an explanatory message.
 */
TEST(CommandOptionTest, ThrowsOnUnsupportedTemplateType) 
{
    CommandOption opt("-u", "--unsupported", "Unsupported type test", CommandOption::UNKNOWN_OPTION);
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

