/**
 * @file StringUtils_T.cpp
 * @author Antonius Torode
 * @date 05/20/2025
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

namespace string_utils
{
    /**
     * Tests for the toLower(..) method.
     */
    TEST(StringUtilsTest, ToLowerConvertsAllCharacters) 
    {
        std::string input1 = "HeLLo";             // Check a simple string with upper-case characters.
        std::string input2 = "HeLLo WoRld!";      // Check for multi-word strings with puncutation.
        std::string input3 = "Digits: 1234567";   // Check that numbers don't change.
        std::string input4 = "SymBoLs: !@#$%^&*"; // Check that symbols don't change.

        // Check expected results.
        EXPECT_EQ(toLower(input1), "hello");
        EXPECT_EQ(toLower(input2), "hello world!");
        EXPECT_EQ(toLower(input3), "digits: 1234567");
        EXPECT_EQ(toLower(input4), "symbols: !@#$%^&*");
    }

    /**
     * Tests for the toUpper(..) method.
     */
    TEST(StringUtilsTest, ToUpperConvertsAllCharacters) 
    {
        std::string input1 = "HeLLo";             // Check a simple string with upper-case characters.
        std::string input2 = "HeLLo WoRld!";      // Check for multi-word strings with puncutation.
        std::string input3 = "Digits: 1234567";   // Check that numbers don't change.
        std::string input4 = "SymBoLs: !@#$%^&*"; // Check that symbols don't change.

        // Check expected results.
        EXPECT_EQ(toUpper(input1), "HELLO");
        EXPECT_EQ(toUpper(input2), "HELLO WORLD!");
        EXPECT_EQ(toUpper(input3), "DIGITS: 1234567");
        EXPECT_EQ(toUpper(input4), "SYMBOLS: !@#$%^&*");
    }

    /**
     * Tests for the removeCharInString(..) nethod.
     */
    TEST(StringUtilsTest, RemoveCharInStringRemovesAllOccurrences) 
    {
        std::string input1 = "hello world"; // A string with just characters.
        std::string input2 = "hell0 w0rld"; // A string with some numbers in it.
        std::string input3 = "he!!o wor!d"; // A string with some symbols in it.
        
        // Check expected results.
        EXPECT_EQ(removeCharInString(input1, 'o'), "hell wrld");
        EXPECT_EQ(removeCharInString(input2, '0'), "hell wrld");
        EXPECT_EQ(removeCharInString(input3, '!'), "heo word");
    }

    /**
     * Tests for the findCharInString(..) method.
     */
    TEST(StringUtilsTest, FindCharInStringReturnsCorrectIndex) 
    {
        std::string input1 = "hello world"; // A string with just characters.
        std::string input2 = "hell0 w0rld"; // A string with some numbers in it.
        std::string input3 = "he!!o wor!d"; // A string with some symbols in it.
        
        // Check expected results.
        EXPECT_EQ(findCharInString(input1, 'e'), 1);
        EXPECT_EQ(findCharInString(input1, 'o'), 4);
        EXPECT_EQ(findCharInString(input2, '0'), 4);
        EXPECT_EQ(findCharInString(input3, '!'), 2);
        EXPECT_EQ(findCharInString(input1, 'z'), -1);  // Not found returns -1
    }

    /**
     * Tests for the stringContainsChar(..) method.
     */
    TEST(StringUtilsTest, StringContainsCharDetectsCharacter) 
    {
        std::string input1 = "hello world"; // A string with just characters.
        std::string input2 = "hell0 w0rld"; // A string with some numbers in it.
        std::string input3 = "he!!o wor!d"; // A string with some symbols in it.
        
        // Check expected results.
        EXPECT_TRUE(stringContainsChar(input1, 'e'));
        EXPECT_TRUE(stringContainsChar(input1, 'l'));
        EXPECT_TRUE(stringContainsChar(input2, '0'));
        EXPECT_TRUE(stringContainsChar(input3, '!'));
        EXPECT_FALSE(stringContainsChar(input1, 'x'));
    }

    /**
     * Tests for the delimiterString(..) method.
     */
    TEST(StringUtilsTest, DelimiterStringSplitsCorrectly) 
    {
        // Check behavior using a comma delimiter.
        std::string input1 = "one,two,three";
        std::vector<std::string> result1 = delimiterString(input1, ",", false);
        ASSERT_EQ(result1.size(), 3);
        EXPECT_EQ(result1[0], "one");
        EXPECT_EQ(result1[1], "two");
        EXPECT_EQ(result1[2], "three");
        
        // Check behavior using a number delimiter.
        std::string input2 = "one0two0three";
        std::vector<std::string> result2 = delimiterString(input2, "0", false);
        ASSERT_EQ(result2.size(), 3);
        EXPECT_EQ(result2[0], "one");
        EXPECT_EQ(result2[1], "two");
        EXPECT_EQ(result2[2], "three");
        
        // Check behavior using a symbol delimiter.
        std::string input3 = "one!two!three";
        std::vector<std::string> result3 = delimiterString(input3, "!", false);
        ASSERT_EQ(result3.size(), 3);
        EXPECT_EQ(result3[0], "one");
        EXPECT_EQ(result3[1], "two");
        EXPECT_EQ(result3[2], "three");
    }

    /**
     * Tests for the isDigits(..) method.
     */
    TEST(StringUtilsTest, IsDigitsReturnsTrueForDigitsOnly) 
    {
        EXPECT_TRUE(isDigits("123456"));
        EXPECT_FALSE(isDigits("123a56"));
        EXPECT_FALSE(isDigits("123!56"));
    }

    /**
     * Tests for the inputRoll(..) method.
     */
    TEST(StringUtilsTest, InputRollValidFormat) 
    {
        EXPECT_TRUE(inputRoll("1d20"));
        EXPECT_TRUE(inputRoll("30d19"));
        EXPECT_FALSE(inputRoll("1d2x"));
        EXPECT_FALSE(inputRoll("Potato"));
    }

    /**
     * Tests for the formOfYes(..) method.
     */
    TEST(StringUtilsTest, FormOfYesRecognizesAffirmative) 
    {
        // Check affirmative cases.
        EXPECT_TRUE(formOfYes("yes"));
        EXPECT_TRUE(formOfYes("Y"));
        EXPECT_TRUE(formOfYes("y"));
        EXPECT_TRUE(formOfYes("ok"));
        EXPECT_TRUE(formOfYes("sure"));
        EXPECT_TRUE(formOfYes("indeed"));
        EXPECT_TRUE(formOfYes("affirmative"));
        EXPECT_TRUE(formOfYes("indubitably"));
        
        // Check negative cases.
        EXPECT_FALSE(formOfYes("no"));
        EXPECT_FALSE(formOfYes("negative"));
        EXPECT_FALSE(formOfYes("hello world!"));
    }

    /**
     * Tests for the today() method.
     */
    TEST(StringUtilsTest, TodayReturnsNonEmptyString) 
    {
        EXPECT_FALSE(today().empty());
    }

    /**
     * Tests for the shuffleString(..) method.
     */
    TEST(StringUtilsTest, ShuffleStringChangesString) 
    {
        std::string input1 = "abcdef";
        std::string shuffled1 = shuffleString(input1);
        // It's possible shuffled == input, but unlikely; just check length here.
        EXPECT_EQ(shuffled1.length(), input1.length());
        
        // Make sure the shuffled string matches one of the possibilities.
        std::string input2 = "abc";
        std::string shuffled2 = shuffleString(input2);
        std::vector<std::string> possibleShuffles = 
            { "abc", "acb", "bac", "bca", "cab", "cba" };
        uint8_t matches = 0;
        for (size_t i=0; i<6; i++)
        {
            if (shuffled2 == possibleShuffles[i])
                matches++;
        }
        ASSERT_EQ(matches, 1)
            << "The shuffled string must only match one possible shuffle string.";
            
        // Make sure all possible shuffled strings get made after multiple calls.
        std::vector<uint8_t> counters;
        counters.resize(6);
        for (int i=0; i<1000; i++)
        { // 1000 iterations is more than enough to statistically always get all combinations.
            shuffled2 = shuffleString(input2);
            for (size_t j=0; j<6; j++)
            {
                if (shuffled2 == possibleShuffles[j])
                {
                    counters[j]++;
                    continue;
                }
            }
        }    
        for (size_t i=0; i<6; i++)
        { // Check each counter for the shuffle variations.
            ASSERT_TRUE(counters[i] > 0)
                << "All shuffled strings should appear after 1000 iterations.";
        }
    }

    /**
     * Tests for the getBeforeChar(..) method.
     */
    TEST(StringUtilsTest, GetBeforeCharReturnsCorrectSubstring) 
    {
        EXPECT_EQ(getBeforeChar("key=value", '='), "key");
        EXPECT_EQ(getBeforeChar("key0value", '0'), "key");
        EXPECT_EQ(getBeforeChar("keyEvalue", 'E'), "key");
        EXPECT_EQ(getBeforeChar("keyEvElue", 'E'), "key");
    }

    /**
     * Tests for the getAfterChar(..) method.
     */
    TEST(StringUtilsTest, GetAfterCharReturnsCorrectSubstring) 
    {
        EXPECT_EQ(getAfterChar("key=value", '='), "value");
        EXPECT_EQ(getAfterChar("key0value", '0'), "value");
        EXPECT_EQ(getAfterChar("keyEvalue", 'E'), "value");
        EXPECT_EQ(getAfterChar("keyEvElue", 'E'), "vElue");
    }

    /**
     * Tests for the getBetweenXAndY(..) method.
     */
    TEST(StringUtilsTest, GetBetweenXAndYReturnsCorrectSubstring) 
    {
        EXPECT_EQ(getBetweenXAndY("[data]", '[', ']'), "data");
        EXPECT_EQ(getBetweenXAndY("AdataB", 'A', 'B'), "data");
        
        // Missing marker characters.
        EXPECT_EQ(getBetweenXAndY("data", '[', ']'), "");
        EXPECT_EQ(getBetweenXAndY("[data", '[', ']'), "");
        EXPECT_EQ(getBetweenXAndY("data]", '[', ']'), "");
        
        // Duplicate marker characters.    
        EXPECT_EQ(getBetweenXAndY("0data0", '0', '0'), "data");
        EXPECT_EQ(getBetweenXAndY("AdataA", 'A', 'A'), "data");
    }

    /**
     * Tests for the entangleText(..) method.
     */
    TEST(StringUtilsTest, EntangleTextCreatesTwoStrings) 
    {
        std::string input = "abcdef";
        auto result = entangleText(input);
        ASSERT_EQ(result.size(), 2);
        EXPECT_EQ(result[0], "a c e ");
        EXPECT_EQ(result[1], " b d f");
    }

    /**
     * Tests for the stringToIntVector(..) method.
     */
    TEST(StringUtilsTest, TestStringToIntVector) 
    {
        // Test empty string.
        EXPECT_EQ(stringToIntVector(""), std::vector<int>{});

        // Test single uppercase character.
        EXPECT_EQ(stringToIntVector("A"), std::vector<int>({65}));

        // Test multiple uppercase characters.
        EXPECT_EQ(stringToIntVector("ABC"), std::vector<int>({65, 66, 67}));

        // Test lowercase characters.
        EXPECT_EQ(stringToIntVector("abc"), std::vector<int>({97, 98, 99}));

        // Test numeric characters.
        EXPECT_EQ(stringToIntVector("0123456789"), std::vector<int>({48, 49, 50, 51, 52, 53, 54, 55, 56, 57}));

        // Test special characters.
        EXPECT_EQ(stringToIntVector("!@#"), std::vector<int>({33, 64, 35}));

        // Test mixed characters.
        EXPECT_EQ(stringToIntVector("a1B!"), std::vector<int>({97, 49, 66, 33}));
    }

    /**
     * Tests for the intVectorToString(..) method.
     */
    TEST(StringUtilsTest, IntVectorToStringCases)
    {
        // Empty vector.
        EXPECT_EQ(intVectorToString({}), "");

        // Single character.
        EXPECT_EQ(intVectorToString({65}), "A");

        // ASCII letters.
        EXPECT_EQ(intVectorToString({72, 101, 108, 108, 111}), "Hello");

        // Digits as characters.
        EXPECT_EQ(intVectorToString({48, 49, 50, 51, 52}), "01234");

        // Special characters.
        EXPECT_EQ(intVectorToString({33, 64, 35}), "!@#");

        // Mixed content.
        EXPECT_EQ(intVectorToString({97, 49, 66, 33}), "a1B!");

        // Non-printable ASCII (e.g., newline).
        EXPECT_EQ(intVectorToString({72, 10, 87}), "H\nW");
    }

    /**
     * Tests for the invertString(..) method.
     */
    TEST(StringUtilsTest, InvertStringTests) 
    {
        // Empty string returns empty string.
        EXPECT_EQ(invertString(""), "");

        // Single character returns same character.
        EXPECT_EQ(invertString("A"), "A");

        // Palindrome string returns same string.
        EXPECT_EQ(invertString("madam"), "madam");

        // Normal string reverses correctly.
        EXPECT_EQ(invertString("hello"), "olleh");

        // String with spaces and punctuation.
        EXPECT_EQ(invertString("a b!"), "!b a");

        // String with numbers.
        EXPECT_EQ(invertString("12345"), "54321");
    }

    /**
     * Tests for the centerText(..) method.
     */
    TEST(StringUtilsTest, CenterText)
    {
        EXPECT_EQ(centerText("Test", 10), "   Test   ");
        EXPECT_EQ(centerText("Test", 9), "  Test   ");
        EXPECT_EQ(centerText("Test", 8), "  Test  ");
        EXPECT_EQ(centerText("Test", 4), "Test");
        EXPECT_EQ(centerText("Test", 3), "Test");
        EXPECT_EQ(centerText("", 5), "     ");
    }
} // namespace string_utils
