/**
 * @file MIAException_T.cpp
 * @author Antonius Torode
 * @brief Unit tests for the MIAException class using Google Test.
 * Creation date: 2025-06-15
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

TEST(MIAExceptionTest, ConstructsWithCodeAndMessage) 
{
    MIAException ex(ErrorCode::Linux_Only_Feature, "Provided value is null");
    EXPECT_EQ(ex.getCode(), ErrorCode::Linux_Only_Feature);
    EXPECT_NE(std::string(ex.what()).find("Linux"), std::string::npos);
    EXPECT_NE(std::string(ex.what()).find("Provided value is null"), std::string::npos);
}

TEST(MIAExceptionTest, ConstructsWithCodeOnly) 
{
    MIAException ex(ErrorCode::File_Not_Found);
    EXPECT_EQ(ex.getCode(), ErrorCode::File_Not_Found);
    EXPECT_EQ(std::string(ex.what()), "File not found.");
}

TEST(MIAExceptionTest, WhatReturnsCachedMessage) 
{
    MIAException ex(ErrorCode::Catastrophic_Failure, "Test message");
    const char* first = ex.what();
    const char* second = ex.what();
    EXPECT_EQ(first, second);  // Should be the same pointer (cached)
}

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

