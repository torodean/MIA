/**
 * @file MIAException_T.cpp
 * @author Antonius Torode
 * @brief Unit tests for the MIAException class using Google Test.
 * @date 2025-06-15
 *
 * This test suite validates the behavior of the MIAException class, including:
 * - Construction with error codes and detail strings
 * - Correct message formatting from getErrorDescription
 * - Proper integration with std::exception::what()
 * - Macro MIA_THROW generates contextual error messages
 */

#include <gtest/gtest.h>
#include "MIAException.hpp"

using namespace error;

/**
 * @test MIAExceptionTest.ConstructsWithCodeAndMessage
 * @brief Verifies that MIAException correctly stores an error code and appends
 *        a custom message when constructed with both parameters.
 *
 * The test confirms that getCode() returns the provided ErrorCode and that the
 * formatted message returned by what() contains both the default error text
 * associated with the code and the supplied custom message.
 */
TEST(MIAExceptionTest, ConstructsWithCodeAndMessage) 
{
    MIAException ex(ErrorCode::Linux_Only_Feature, "Provided value is null");
    EXPECT_EQ(ex.getCode(), ErrorCode::Linux_Only_Feature);
    EXPECT_NE(std::string(ex.what()).find("Linux"), std::string::npos);
    EXPECT_NE(std::string(ex.what()).find("Provided value is null"), std::string::npos);
}

/**
 * @test MIAExceptionTest.ConstructsWithCodeOnly
 * @brief Verifies that MIAException constructed with only an ErrorCode uses the
 *        default error message associated with that code.
 *
 * The test confirms that getCode() returns the provided code and that what()
 * returns the expected default message without additional text.
 */
TEST(MIAExceptionTest, ConstructsWithCodeOnly) 
{
    MIAException ex(ErrorCode::File_Not_Found);
    EXPECT_EQ(ex.getCode(), ErrorCode::File_Not_Found);
    EXPECT_EQ(std::string(ex.what()), "File not found.");
}

/**
 * @test MIAExceptionTest.WhatReturnsCachedMessage
 * @brief Verifies that MIAException::what() returns a cached message buffer.
 *
 * The test calls what() multiple times and confirms that the returned pointer
 * remains the same, indicating that the formatted error message is stored and
 * reused rather than recomputed on each call.
 */
TEST(MIAExceptionTest, WhatReturnsCachedMessage) 
{
    MIAException ex(ErrorCode::Catastrophic_Failure, "Test message");
    const char* first = ex.what();
    const char* second = ex.what();
    EXPECT_EQ(first, second);  // Should be the same pointer (cached)
}

/**
 * @test MIAExceptionTest.MIAThrowMacroAddsContext
 * @brief Verifies that the MIA_THROW macro throws MIAException and augments the
 *        error message with contextual information.
 *
 * The test confirms that the thrown exception contains the base error message,
 * the user-supplied message, and the function context where the macro was
 * invoked (resolved to TestBody in GoogleTest).
 */
TEST(MIAExceptionTest, MIAThrowMacroAddsContext) 
{
    try 
    {
        MIA_THROW(ErrorCode::Catastrophic_Failure, "Invalid state");
        FAIL() << "Expected exception not thrown";
    } 
    catch (const MIAException& ex) 
    {
        std::string msg = ex.what();
        EXPECT_NE(msg.find("catastrophic failure"), std::string::npos);
        EXPECT_NE(msg.find("Invalid state"), std::string::npos);
        
        // Google test resolves this to a method named TestBody.
        // See the output of std::cout << __func__ << std::endl;        
        EXPECT_NE(msg.find("in TestBody"), std::string::npos);
    }
}

