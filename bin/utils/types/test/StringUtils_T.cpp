/**
 * @file StringUtils_T.cpp
 * @author Antonius Torode
 * @date 05/20/2025
 * 
 * This file contains unit tests for the StringUtils module using Google Test.
 * It verifies the correctness of string manipulation functions such as case
 * conversion, character removal, substring extraction, format validation,
 * and other utility operations defined in the `StringUtils` namespace.
 * 
 * The tests ensure reliable behavior and correctness of the StringUtils
 * functions across typical use cases.
 */

#include <gtest/gtest.h>

// Include the associated files to test.
#include "StringUtils.hpp"
#include "MIAException.hpp"

namespace string_utils
{
    /**
     * Tests for the toLower(..) method.
     */
    TEST(StringUtils_T, ToLowerConvertsAllCharacters) 
    {
        EXPECT_EQ(toLower("HeLLo"), "hello") 
            << "Check a simple string with upper-case characters.";

        EXPECT_EQ(toLower("HeLLo WoRld!"), "hello world!") 
            << "Check multi-word strings with punctuation.";

        EXPECT_EQ(toLower("Digits: 1234567"), "digits: 1234567") 
            << "Check that numbers don't change.";

        EXPECT_EQ(toLower("SymBoLs: !@#$%^&*"), "symbols: !@#$%^&*") 
            << "Check that symbols don't change.";

        EXPECT_EQ(toLower(""), "") 
            << "Check an empty string.";
    }


    /**
     * Tests for the toUpper(..) method.
     */
    TEST(StringUtils_T, ToUpperConvertsAllCharacters) 
    {
        EXPECT_EQ(toUpper("HeLLo"), "HELLO") 
            << "Check a simple string with upper-case characters.";

        EXPECT_EQ(toUpper("HeLLo WoRld!"), "HELLO WORLD!") 
            << "Check multi-word strings with punctuation.";

        EXPECT_EQ(toUpper("Digits: 1234567"), "DIGITS: 1234567") 
            << "Check that numbers don't change.";

        EXPECT_EQ(toUpper("SymBoLs: !@#$%^&*"), "SYMBOLS: !@#$%^&*") 
            << "Check that symbols don't change.";

        EXPECT_EQ(toUpper(""), "") 
            << "Check an empty string.";
    }


    /**
     * Tests for the removeCharInString(..) nethod.
     */
    TEST(StringUtils_T, RemoveCharInStringRemovesAllOccurrences) 
    {
        EXPECT_EQ(removeCharInString("hello world", 'o'), "hell wrld")
	        << "Check removing a character from a simple string.";

        EXPECT_EQ(removeCharInString("hell0 w0rld", '0'), "hell wrld")
	        << "Check removing numbers from a string.";

        EXPECT_EQ(removeCharInString("he!!o wor!d", '!'), "heo word")
	        << "Check removing symbols from a string.";

        EXPECT_EQ(removeCharInString("fish", '!'), "fish")
	        << "Check a string with no matching characters.";

        EXPECT_EQ(removeCharInString("", '!'), "")
	        << "Check an empty string.";
    }


    /**
     * Tests for the findCharInString(..) method.
     */
    TEST(StringUtils_T, FindCharInStringReturnsCorrectIndex) 
    {
        EXPECT_EQ(findCharInString("hello world", 'e'), 1)
	        << "Check finding a character in a string.";

        EXPECT_EQ(findCharInString("hello world", 'o'), 4)
	        << "Check finding a character with multiple occurrences.";

        EXPECT_EQ(findCharInString("hell0 w0rld", '0'), 4)
	        << "Check finding a number in a string.";

        EXPECT_EQ(findCharInString("he!!o wor!d", '!'), 2)
	        << "Check finding a symbol in a string.";

        EXPECT_EQ(findCharInString("hello world", 'z'), -1)
	        << "Check that a character not found returns -1.";

        EXPECT_EQ(findCharInString("", 'z'), -1)
	        << "Check that searching an empty string returns -1.";
    }


    /**
     * Tests for the stringContainsChar(..) method.
     */
    TEST(StringUtils_T, StringContainsCharDetectsCharacter) 
    {
        EXPECT_TRUE(stringContainsChar("hello world", 'e'))
	        << "Check finding a character in a string.";

        EXPECT_TRUE(stringContainsChar("hello world", 'l'))
	        << "Check finding another character in a string.";

        EXPECT_TRUE(stringContainsChar("hell0 w0rld", '0'))
	        << "Check finding a number in a string.";

        EXPECT_TRUE(stringContainsChar("he!!o wor!d", '!'))
	        << "Check finding a symbol in a string.";

        EXPECT_FALSE(stringContainsChar("hello world", 'x'))
	        << "Check that a character not found returns false.";

        EXPECT_FALSE(stringContainsChar("", 'x'))
	        << "Check that searching an empty string returns false.";
    }


    /**
     * Tests for the delimiterString(..) method.
     */
    TEST(StringUtils_T, DelimiterStringSplitsCorrectly) 
    {
        EXPECT_EQ(delimiterString("one,two,three", ",", false),
            (std::vector<std::string>{"one", "two", "three"}))
            << "Check behavior using a comma delimiter.";

        EXPECT_EQ(delimiterString("one0two0three", "0", false),
            (std::vector<std::string>{"one", "two", "three"}))
            << "Check behavior using a number delimiter.";

        EXPECT_EQ(delimiterString("one!two!three", "!", false),
            (std::vector<std::string>{"one", "two", "three"}))
            << "Check behavior using a symbol delimiter.";

        EXPECT_EQ(delimiterString("one!!two", "!", false),
            (std::vector<std::string>{"one", "", "two"}))
            << "Check behavior using a malformed string with consecutive delimiters.";

        EXPECT_EQ(delimiterString("one", "!", false),
            (std::vector<std::string>{"one"}))
            << "Check behavior when the delimiter is not present.";

        EXPECT_EQ(delimiterString("", "!", false),
            (std::vector<std::string>{""}))
            << "Check behavior using an empty string.";

        EXPECT_EQ(delimiterString("!", "!", false),
            (std::vector<std::string>{"", ""}))
            << "Check behavior using a string containing only the delimiter.";

        EXPECT_EQ(delimiterString("one!", "!", false),
            (std::vector<std::string>{"one", ""}))
            << "Check behavior using a string ending with the delimiter.";

        EXPECT_EQ(delimiterString("onefishtwofishredfishbluefish", "fish", false),
            (std::vector<std::string>{"one", "two", "red", "blue", ""}))
            << "Check behavior using a multi-char string delimiter.";
    }    


    /**
     * Tests for the delimiterString(..) method (char input overload).
     */
    TEST(StringUtils_T, DelimiterStringSplitsCorrectlyCharOverload) 
    {        
        EXPECT_EQ(delimiterString("one,two,three", ',', false),
            (std::vector<std::string>{"one", "two", "three"}))
            << "Check behavior using a comma delimiter.";

        EXPECT_EQ(delimiterString("one0two0three", '0', false),
            (std::vector<std::string>{"one", "two", "three"}))
            << "Check behavior using a number delimiter.";

        EXPECT_EQ(delimiterString("one!two!three", '!', false),
            (std::vector<std::string>{"one", "two", "three"}))
            << "Check behavior using a symbol delimiter.";

        EXPECT_EQ(delimiterString("one!!two", '!', false),
            (std::vector<std::string>{"one", "", "two"}))
            << "Check behavior using a malformed string with consecutive delimiters.";

        EXPECT_EQ(delimiterString("one", '!', false),
            (std::vector<std::string>{"one"}))
            << "Check behavior when the delimiter is not present.";

        EXPECT_EQ(delimiterString("", '!', false),
            (std::vector<std::string>{""}))
            << "Check behavior using an empty string.";

        EXPECT_EQ(delimiterString("!", '!', false),
            (std::vector<std::string>{"", ""}))
            << "Check behavior using a string containing only the delimiter.";

        EXPECT_EQ(delimiterString("one!", '!', false),
            (std::vector<std::string>{"one", ""}))
            << "Check behavior using a string ending with the delimiter.";
    }


    /**
     * Tests for the isDigits(..) method.
     */
    TEST(StringUtils_T, IsDigitsReturnsTrueForDigitsOnly) 
    {
        EXPECT_TRUE(isDigits("123456"))
            << "Check a string containing only digits.";

        EXPECT_FALSE(isDigits("123a56"))
            << "Check a string containing letters.";

        EXPECT_FALSE(isDigits("123!56"))
            << "Check a string containing symbols.";

        EXPECT_FALSE(isDigits(""))
            << "Check an empty string.";
    }


    /**
     * Tests for the isHexDigits(..) method.
     */
    TEST(StringUtils_T, IsHexDigitsReturnsTrueForDigitsOnly) 
    {
        EXPECT_TRUE(isHexDigits("123456"))
            << "Check a string containing only digits.";
            
        EXPECT_TRUE(isHexDigits("ABCDE"))
            << "Check a string containing only hex-letters.";
            
        EXPECT_TRUE(isHexDigits("FFF"))
            << "Check a string containing only F's.";
            
        EXPECT_FALSE(isHexDigits("ZZZ"))
            << "Check a string containing only non-hex-letters";
            
        EXPECT_TRUE(isHexDigits("123456ABC"))
            << "Check a string containing digits and hex-letters.";

        EXPECT_FALSE(isHexDigits("12Az56"))
            << "Check a string containing non-hex letters letters.";

        EXPECT_FALSE(isHexDigits("1A3!56"))
            << "Check a string containing symbols.";

        EXPECT_FALSE(isHexDigits(""))
            << "Check an empty string.";
    }


    /**
     * Tests for the inputRoll(..) method.
     */
    TEST(StringUtils_T, InputRollValidFormat) 
    {
        EXPECT_TRUE(inputRoll("1d20"))
            << "Check a valid dice roll format.";

        EXPECT_TRUE(inputRoll("30d19"))
            << "Check a valid dice roll format with multi-digit values.";

        EXPECT_FALSE(inputRoll("1d2x"))
            << "Check an invalid dice roll containing a letter.";

        EXPECT_FALSE(inputRoll("Potato"))
            << "Check an invalid dice roll containing no dice notation.";

        EXPECT_FALSE(inputRoll(""))
            << "Check an empty string.";
    }


    /**
     * Tests for the formOfYes(..) method.
     */
    TEST(StringUtils_T, FormOfYesRecognizesAffirmative) 
    {
        // Check affirmative cases.
        EXPECT_TRUE(formOfYes("yes"));
        EXPECT_TRUE(formOfYes("Y"));
        EXPECT_TRUE(formOfYes("y"));
        EXPECT_TRUE(formOfYes("ok"));
        EXPECT_TRUE(formOfYes("sure"));
        EXPECT_TRUE(formOfYes("indeed"));
        EXPECT_TRUE(formOfYes("IndEeD"));
        EXPECT_TRUE(formOfYes("affirmative"));
        EXPECT_TRUE(formOfYes("indubitably"));

        // Check negative cases.
        EXPECT_FALSE(formOfYes(""));
        EXPECT_FALSE(formOfYes("no"));
        EXPECT_FALSE(formOfYes("negative"));
        EXPECT_FALSE(formOfYes("hello world!"));
    }


    /**
     * Tests for the today() method.
     */
    TEST(StringUtils_T, TodayReturnsNonEmptyString) 
    {
        EXPECT_FALSE(today().empty());
    }


    /**
     * Tests for the shuffleString(..) method.
     */
    TEST(StringUtils_T, ShuffleStringChangesString) 
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
    TEST(StringUtils_T, GetBeforeCharReturnsCorrectSubstring) 
    {
        EXPECT_EQ(getBeforeChar("key=value", '='), "key")
            << "Check substring before a delimiter character.";

        EXPECT_EQ(getBeforeChar("key0value", '0'), "key")
            << "Check substring before a numeric character.";

        EXPECT_EQ(getBeforeChar("keyEvalue", 'E'), "key")
            << "Check substring before an uppercase character.";

        EXPECT_EQ(getBeforeChar("keyEvElue", 'E'), "key")
            << "Check substring before the first occurrence of a character.";
    }


    /**
     * Tests for the getAfterChar(..) method.
     */
    TEST(StringUtils_T, GetAfterCharReturnsCorrectSubstring) 
    {
        EXPECT_EQ(getAfterChar("key=value", '='), "value")
            << "Check substring after a delimiter character.";

        EXPECT_EQ(getAfterChar("key0value", '0'), "value")
            << "Check substring after a numeric character.";

        EXPECT_EQ(getAfterChar("keyEvalue", 'E'), "value")
            << "Check substring after an uppercase character.";

        EXPECT_EQ(getAfterChar("keyEvElue", 'E'), "vElue")
            << "Check substring after the first occurrence of a character.";
    }


    /**
     * Tests for the getBetweenXAndY(..) method.
     */
    TEST(StringUtils_T, GetBetweenXAndYReturnsCorrectSubstring) 
    {
        EXPECT_EQ(getBetweenXAndY("[data]", '[', ']'), "data");
        EXPECT_EQ(getBetweenXAndY("AdataB", 'A', 'B'), "data");

        // Missing marker characters.
        EXPECT_EQ(getBetweenXAndY("data", '[', ']'), "");
        EXPECT_EQ(getBetweenXAndY("[data", '[', ']'), "");
        EXPECT_EQ(getBetweenXAndY("data]", '[', ']'), "");
        EXPECT_EQ(getBetweenXAndY("", '[', ']'), "");

        // Duplicate marker characters.    
        EXPECT_EQ(getBetweenXAndY("0data0", '0', '0'), "data");
        EXPECT_EQ(getBetweenXAndY("AdataA", 'A', 'A'), "data");
    }


    /**
     * Tests for the entangleText(..) method.
     */
    TEST(StringUtils_T, EntangleTextCreatesTwoStrings) 
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
    TEST(StringUtils_T, TestStringToIntVector) 
    {
        EXPECT_EQ(stringToIntVector(""), std::vector<int>{})
            << "Check an empty string.";

        EXPECT_EQ(stringToIntVector("A"), std::vector<int>({65}))
            << "Check a single uppercase character.";

        EXPECT_EQ(stringToIntVector("ABC"), std::vector<int>({65, 66, 67}))
            << "Check multiple uppercase characters.";

        EXPECT_EQ(stringToIntVector("abc"), std::vector<int>({97, 98, 99}))
            << "Check lowercase characters.";

        EXPECT_EQ(stringToIntVector("0123456789"), std::vector<int>({48, 49, 50, 51, 52, 53, 54, 55, 56, 57}))
            << "Check numeric characters.";

        EXPECT_EQ(stringToIntVector("!@#"), std::vector<int>({33, 64, 35}))
            << "Check special characters.";

        EXPECT_EQ(stringToIntVector("a1B!"), std::vector<int>({97, 49, 66, 33}))
            << "Check mixed characters.";
    }


    /**
     * Tests for the intVectorToString(..) method.
     */
    TEST(StringUtils_T, IntVectorToStringCases)
    {
        EXPECT_EQ(intVectorToString({}), "")
	        << "Check an empty vector.";

        EXPECT_EQ(intVectorToString({65}), "A")
	        << "Check a single character.";

        EXPECT_EQ(intVectorToString({72, 101, 108, 108, 111}), "Hello")
	        << "Check ASCII letters.";

        EXPECT_EQ(intVectorToString({48, 49, 50, 51, 52}), "01234")
	        << "Check digits as characters.";

        EXPECT_EQ(intVectorToString({33, 64, 35}), "!@#")
	        << "Check special characters.";

        EXPECT_EQ(intVectorToString({97, 49, 66, 33}), "a1B!")
	        << "Check mixed content.";

        EXPECT_EQ(intVectorToString({72, 10, 87}), "H\nW")
	        << "Check non-printable ASCII (e.g., newline).";
    }


    /**
     * Tests for the invertString(..) method.
     */
    TEST(StringUtils_T, InvertStringTests) 
    {
        EXPECT_EQ(invertString(""), "")
	        << "Check an empty string returns an empty string.";

        EXPECT_EQ(invertString("A"), "A")
	        << "Check a single character returns the same character.";

        EXPECT_EQ(invertString("madam"), "madam")
	        << "Check a palindrome string returns the same string.";

        EXPECT_EQ(invertString("hello"), "olleh")
	        << "Check a normal string reverses correctly.";

        EXPECT_EQ(invertString("a b!"), "!b a")
	        << "Check a string with spaces and punctuation.";

        EXPECT_EQ(invertString("12345"), "54321")
	        << "Check a string with numbers.";
    }


    /**
     * Tests for the centerText(..) method.
     */
    TEST(StringUtils_T, CenterText)
    {
        EXPECT_EQ(centerText("Test", 10), "   Test   ");
        EXPECT_EQ(centerText("Test", 9), "  Test   ");
        EXPECT_EQ(centerText("Test", 8), "  Test  ");
        EXPECT_EQ(centerText("Test", 4), "Test");
        EXPECT_EQ(centerText("Test", 3), "Test");
        EXPECT_EQ(centerText("", 5), "     ");
    }


    /**
     * Tests for the resizeString(..) method.
     */
    TEST(StringUtils_T, resizeString)
    {
        EXPECT_EQ(resizeString("12345", "123"), "  123");
        EXPECT_EQ(resizeString("123", "12345"), "  123");
        EXPECT_EQ(resizeString("aaaa", "bbbb"), "bbbb");
        EXPECT_EQ(resizeString("12345", "123", '0'), "00123");
    }


    /**
     * @brief Tests the zeroString method.
     */
    TEST(StringUtils_T, zeroString)
    {
        ASSERT_EQ(zeroString(5), "00000");
        ASSERT_EQ(zeroString(8), "00000000");
        ASSERT_EQ(zeroString(0), "");

        // Check a large string.
        std::string zStr = zeroString(5678);
        ASSERT_EQ(zStr.size(), 5678);
    }    


	/**
     * Covers the stringToUint8 method: valid inputs at the range boundaries, 
	 * and every failure mode it should reject.
     */
    TEST(StringUtils_T, StringToUint8)
    {
        // Valid values parse to the exact uint8_t value.
        EXPECT_EQ(stringToUint8("0"), static_cast<uint8_t>(0));
        EXPECT_EQ(stringToUint8("1"), static_cast<uint8_t>(1));
        EXPECT_EQ(stringToUint8("42"), static_cast<uint8_t>(42));
        EXPECT_EQ(stringToUint8("255"), static_cast<uint8_t>(255));

        // Values below zero are rejected instead of wrapping.
        EXPECT_THROW(stringToUint8("-1"), error::MIAException);

        // Values above 255 are rejected instead of truncating.
        EXPECT_THROW(stringToUint8("256"), error::MIAException);
        EXPECT_THROW(stringToUint8("300"), error::MIAException);

        // Non-numeric strings are rejected.
        EXPECT_THROW(stringToUint8("abc"), error::MIAException);
        EXPECT_THROW(stringToUint8(""), error::MIAException);

        /*
		 * Partially numeric strings are rejected, not silently truncated
		 * to their leading digits by std::stoi.
		 */
        EXPECT_THROW(stringToUint8("12abc"), error::MIAException);
        EXPECT_THROW(stringToUint8("1 2"), error::MIAException);
    }
} // namespace string_utils
