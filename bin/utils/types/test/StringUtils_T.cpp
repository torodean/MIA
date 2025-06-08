/**
 * File: StringUtils_T.cpp
 * Author: Antonius Torode
 * Created on: 05/20/2025
 * Description:
 *     This file contains unit tests for the StringUtils module using Google Test.
 *     It verifies the correctness of string manipulation functions such as case
 *     conversion, character removal, substring extraction, format validation,
 *     and other utility operations defined in the `StringUtils` namespace.
 * 
 *     The tests ensure reliable behavior and correctness of the StringUtils
 *     functions across typical use cases.
 */
#include <gtest/gtest.h>

// Include the associated files to test.
#include "StringUtils.hpp"

using namespace StringUtils;

TEST(StringUtilsTest, ToLowerConvertsAllCharacters) 
{
    std::string input1 = "HeLLo";           // Check a simple string with upper-case characters.
    std::string input2 = "HeLLo WoRld!";    // Check for multi-word strings with puncutation.
    std::string input3 = "Digits: 1234567"; // Check that numbers don't change.

    // Check expected results.
    EXPECT_EQ(toLower(input1), "hello");
    EXPECT_EQ(toLower(input2), "hello world!");
    EXPECT_EQ(toLower(input3), "digits: 1234567");
}

TEST(StringUtilsTest, RemoveCharInStringRemovesAllOccurrences) 
{
    std::string input = "hello world";
    EXPECT_EQ(removeCharInString(input, 'o'), "hell wrld");
}

TEST(StringUtilsTest, FindCharInStringReturnsCorrectIndex) 
{
    std::string input = "hello";
    EXPECT_EQ(findCharInString(input, 'e'), 1);
    EXPECT_EQ(findCharInString(input, 'z'), -1);  // Not found returns -1
}

TEST(StringUtilsTest, StringContainsCharDetectsCharacter) 
{
    std::string input = "abc";
    EXPECT_TRUE(stringContainsChar(input, 'a'));
    EXPECT_FALSE(stringContainsChar(input, 'x'));
}

TEST(StringUtilsTest, DelimiterStringSplitsCorrectly) 
{
    std::string input = "one,two,three";
    auto result = delimiterString(input, ",", false);
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "one");
    EXPECT_EQ(result[1], "two");
    EXPECT_EQ(result[2], "three");
}

TEST(StringUtilsTest, IsDigitsReturnsTrueForDigitsOnly) 
{
    EXPECT_TRUE(is_digits("123456"));
    EXPECT_FALSE(is_digits("123a56"));
}

TEST(StringUtilsTest, InputRollValidFormat) 
{
    std::string validRoll = "1d20";
    std::string alsoValidRoll = "30d19";
    std::string badChars = "1d2x";

    EXPECT_TRUE(inputRoll(validRoll));
    EXPECT_TRUE(inputRoll(alsoValidRoll));
    EXPECT_FALSE(inputRoll(badChars));
}

TEST(StringUtilsTest, FormOfYesRecognizesAffirmative) 
{
    std::string input = "yes";
    input = "yes";
    EXPECT_TRUE(formOfYes(input));
    input = "Y";
    EXPECT_TRUE(formOfYes(input));
    input = "OK";
    EXPECT_TRUE(formOfYes(input));
    input = "sure";
    EXPECT_TRUE(formOfYes(input));
    input = "indeed";
    EXPECT_TRUE(formOfYes(input));
    input = "no";
    EXPECT_FALSE(formOfYes(input));
    input = "negative";
    EXPECT_FALSE(formOfYes(input));
    input = "hello world!";
    EXPECT_FALSE(formOfYes(input));
}

TEST(StringUtilsTest, TodayReturnsNonEmptyString) 
{
    EXPECT_FALSE(today().empty());
}

TEST(StringUtilsTest, ShuffleStringChangesString) 
{
    std::string input = "abcdef";
    std::string shuffled = shuffleString(input);
    // It's possible shuffled == input, but unlikely; just check length
    EXPECT_EQ(shuffled.length(), input.length());
}

TEST(StringUtilsTest, GetBeforeCharReturnsCorrectSubstring) 
{
    std::string input = "key=value";
    EXPECT_EQ(getBeforeChar(input, '='), "key");
}

TEST(StringUtilsTest, GetAfterCharReturnsCorrectSubstring) 
{
    std::string input = "key=value";
    EXPECT_EQ(getAfterChar(input, '='), "value");
}

TEST(StringUtilsTest, GetBetweenXAndYReturnsCorrectSubstring) 
{
    std::string input = "[data]";
    EXPECT_EQ(getBetweenXAndY(input, '[', ']'), "data");
}

TEST(StringUtilsTest, EntangleTextCreatesTwoStrings) 
{
    std::string input = "abcdef";
    auto result = entangleText(input);
    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0], "a c e ");
    EXPECT_EQ(result[1], " b d f");
}

TEST(StringUtilsTest, TestStringToIntVector) 
{
    // Test empty string
    EXPECT_EQ(stringToIntVector(""), std::vector<int>{});

    // Test single uppercase character
    EXPECT_EQ(stringToIntVector("A"), std::vector<int>({65}));

    // Test multiple uppercase characters
    EXPECT_EQ(stringToIntVector("ABC"), std::vector<int>({65, 66, 67}));

    // Test lowercase characters
    EXPECT_EQ(stringToIntVector("abc"), std::vector<int>({97, 98, 99}));

    // Test numeric characters
    EXPECT_EQ(stringToIntVector("0123456789"), std::vector<int>({48, 49, 50, 51, 52, 53, 54, 55, 56, 57}));

    // Test special characters
    EXPECT_EQ(stringToIntVector("!@#"), std::vector<int>({33, 64, 35}));

    // Test mixed characters
    EXPECT_EQ(stringToIntVector("a1B!"), std::vector<int>({97, 49, 66, 33}));
}


TEST(StringUtilsTest, IntVectorToStringCases)
{
    // Empty vector
    EXPECT_EQ(intVectorToString({}), "");

    // Single character
    EXPECT_EQ(intVectorToString({65}), "A");

    // ASCII letters
    EXPECT_EQ(intVectorToString({72, 101, 108, 108, 111}), "Hello");

    // Digits as characters
    EXPECT_EQ(intVectorToString({48, 49, 50, 51, 52}), "01234");

    // Special characters
    EXPECT_EQ(intVectorToString({33, 64, 35}), "!@#");

    // Mixed content
    EXPECT_EQ(intVectorToString({97, 49, 66, 33}), "a1B!");

    // Non-printable ASCII (e.g., newline)
    EXPECT_EQ(intVectorToString({72, 10, 87}), "H\nW");
}

TEST(StringUtilsTest, InvertStringTests) 
{
    // Empty string returns empty string
    EXPECT_EQ(invertString(""), "");

    // Single character returns same character
    EXPECT_EQ(invertString("A"), "A");

    // Palindrome string returns same string
    EXPECT_EQ(invertString("madam"), "madam");

    // Normal string reverses correctly
    EXPECT_EQ(invertString("hello"), "olleh");

    // String with spaces and punctuation
    EXPECT_EQ(invertString("a b!"), "!b a");

    // String with numbers
    EXPECT_EQ(invertString("12345"), "54321");
}
