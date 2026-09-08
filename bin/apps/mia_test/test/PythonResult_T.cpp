/**
 * @file PythonResult_T.cpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Tests for the PythonResult value type.
 *
 * These tests need no Python interpreter; PythonResult is a plain value type.
 */

#include <string>

#include <gtest/gtest.h>

// Associated header to test.
#include "PythonResult.hpp"


/**
 * @brief Verifies the default constructor produces a valid void result.
 */
TEST(PythonResultConstruction, DefaultIsVoid)
{
    PythonResult result;

    EXPECT_TRUE(result.isValid())
        << "A default constructed result should be valid.";
    EXPECT_EQ(result.getType(), PythonResult::Type::Void)
        << "A default constructed result should be Void.";
}

/**
 * @brief Verifies the integer constructor stores the value and type.
 */
TEST(PythonResultConstruction, IntegerValue)
{
    PythonResult result(42L);

    EXPECT_TRUE(result.isValid())
        << "An integer result should be valid.";
    EXPECT_EQ(result.getType(), PythonResult::Type::Integer)
        << "The integer constructor should set the Integer type.";
    EXPECT_EQ(result.asInt(), 42)
        << "asInt should return the stored value.";
}

/**
 * @brief Verifies the double constructor stores the value and type.
 */
TEST(PythonResultConstruction, DoubleValue)
{
    PythonResult result(2.5);

    EXPECT_TRUE(result.isValid())
        << "A double result should be valid.";
    EXPECT_EQ(result.getType(), PythonResult::Type::Double)
        << "The double constructor should set the Double type.";
    EXPECT_DOUBLE_EQ(result.asDouble(), 2.5)
        << "asDouble should return the stored value.";
}

/**
 * @brief Verifies both string constructors produce a String result.
 */
TEST(PythonResultConstruction, StringValue)
{
    PythonResult fromStd("hello");
    PythonResult fromLiteral("world");

    EXPECT_EQ(fromStd.getType(), PythonResult::Type::String)
        << "A std::string argument should build a String result.";
    EXPECT_EQ(fromStd.asString(), "hello")
        << "asString should return the stored value.";
    EXPECT_EQ(fromLiteral.getType(), PythonResult::Type::String)
        << "A const char* literal should build a String result.";
    EXPECT_EQ(fromLiteral.asString(), "world")
        << "asString should return the stored value.";
}

/**
 * @brief Verifies the error factory produces an invalid result with a message.
 */
TEST(PythonResultError, ErrorFactory)
{
    PythonResult result = PythonResult::error("something failed");

    EXPECT_FALSE(result.isValid())
        << "An error result should not be valid.";
    EXPECT_EQ(result.getError(), "something failed")
        << "getError should return the message given to error().";
}

/**
 * @brief Verifies a valid result has an empty error message.
 */
TEST(PythonResultError, ValidResultHasEmptyError)
{
    PythonResult result(7L);

    EXPECT_EQ(result.getError(), "")
        << "A valid result should carry no error message.";
}

/**
 * @brief Verifies typed getters fall back safely on the wrong type.
 */
TEST(PythonResultError, GettersOnWrongType)
{
    PythonResult integer(5L);

    EXPECT_EQ(integer.asDouble(), 0.0)
        << "asDouble on an Integer result should return 0.0.";
    EXPECT_EQ(integer.asString(), "")
        << "asString on an Integer result should return an empty string.";
}
