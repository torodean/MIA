/**
 * @file D3CMath_T.cpp
 * @author Antonius Torode
 * @date 05/20/2025
 * @brief Unit tests for the D3CMath methods and features.
 */

#include <gtest/gtest.h>

// Include the appropriate header with methods to test.
#include "D3CMath.hpp"
#include "MIAException.hpp"

namespace math
{
    /**
     * @brief Tests for the sumOfDigits method.
     */
    TEST(D3CMath_T, sumOfDigits)
    {
        ASSERT_EQ(sumOfDigits("1234"), 10);
        ASSERT_EQ(sumOfDigits("01234"), 10);
        ASSERT_EQ(sumOfDigits(""), 0);
    }


    /**
     * @brief Tests the numberOfFactors method.
     */
    TEST(D3CMath_T, numberOfFactors)
    {
        ASSERT_EQ(numberOfFactors(10), 4);
        ASSERT_EQ(numberOfFactors(0), 0);
        ASSERT_EQ(numberOfFactors(1), 1);
    }


    /**
     * @brief Tests the isTriangleNumber method.
     */
    TEST(D3CMath_T, isTriangleNumber)
    {
        ASSERT_TRUE(isTriangleNumber(1));
        ASSERT_TRUE(isTriangleNumber(6));
        ASSERT_TRUE(isTriangleNumber(210));
        ASSERT_FALSE(isTriangleNumber(7));
        ASSERT_FALSE(isTriangleNumber(1234567));
    }


    /**
     * @brief Tests the numOfDigitsInt method.
     */
    TEST(D3CMath_T, numOfDigitsInt)
    {
        ASSERT_EQ(numOfDigitsInt(1234), 4);
        ASSERT_EQ(numOfDigitsInt(543678), 6);
        ASSERT_EQ(numOfDigitsInt(-404), 3);

        // Special cases.
        ASSERT_EQ(numOfDigitsInt(0), 0);
        ASSERT_EQ(numOfDigitsInt(00000), 0);
    }


    /**
     * @brief Tests the getLargerInt method.
     */
    TEST(D3CMath_T, getLargerInt)
    {
        ASSERT_EQ(getLargerInt(5, 7), 7);
        ASSERT_EQ(getLargerInt(123, 56789), 56789);
        ASSERT_EQ(getLargerInt(-12, 12), 12);
        ASSERT_EQ(getLargerInt(0, 0), 0);
        ASSERT_EQ(getLargerInt(-123, -142), -123);
    }


    /**
     * @brief Tests the stringToVector method.
     */
    TEST(D3CMath_T, stringToVector)
    {
        // TODO
    }


    /**
     * @brief Tests the vectorToString method.
     */
    TEST(D3CMath_T, vectorToString)
    {
        // TODO
    }


    /**
     * @brief Tests the addTwoStrings method.
     */
    TEST(D3CMath_T, addTwoStrings)
    {
        std::string sum = addTwoStrings("15", "32");
        ASSERT_EQ(sum, "47");

        sum = addTwoStrings("0", "42");
        ASSERT_EQ(sum, "42");

        sum = addTwoStrings("42", "0");
        ASSERT_EQ(sum, "42");

        // TODO - this is currently not supported by this method.
        //sum = addTwoStrings("15", "-20");
        //ASSERT_EQ(sum, "-5");

        sum = addTwoStrings("0", "0");
        ASSERT_EQ(sum, "0");

        sum = addTwoStrings("-12", "-42");
        ASSERT_EQ(sum, "-54");
    }


    /**
     * @brief Tests the addNegativeSign method.
     */
    TEST(D3CMath_T, addNegativeSign)
    {
        ASSERT_EQ(addNegativeSign("123"), "-123");
        ASSERT_EQ(addNegativeSign("-123"), "-123");
        ASSERT_EQ(addNegativeSign("0"), "0");
        ASSERT_EQ(addNegativeSign("00000"), "00000");
        ASSERT_THROW(addNegativeSign(""), error::MIAException);
    }


    /**
     * @brief Tests the subtractTwoStrings method.
     */
    TEST(D3CMath_T, subtractTwoStrings)
    {
        // Normal larger - smaller subtraction.
        std::string diff = subtractTwoStrings("123456", "2564");
        ASSERT_EQ(diff, "120892");

        // smaller - larger subtraction with strings of the same length.
        diff = subtractTwoStrings("15", "32");
        ASSERT_EQ(diff, "-17");        

        // larger - smaller subtraction with strings of the same length.
        diff = subtractTwoStrings("32", "12");
        ASSERT_EQ(diff, "20");

        // Subtraction where the first is zero and second is positive.
        diff = subtractTwoStrings("0", "42");
        ASSERT_EQ(diff, "-42");

        // Subtraction where the first is zero and second is negative.
        diff = subtractTwoStrings("0", "-42");
        ASSERT_EQ(diff, "42");

        // Subtraction where the first is positve and second is zero.
        diff = subtractTwoStrings("42", "0");
        ASSERT_EQ(diff, "42");

        // Subtraction where the first is negative and second is zero.
        diff = subtractTwoStrings("-42", "0");
        ASSERT_EQ(diff, "-42");

        // Positive minus a negative with same size strings.
        diff = subtractTwoStrings("15", "-20");
        ASSERT_EQ(diff, "35");

        // Negative minus a positive with same size strings.
        diff = subtractTwoStrings("-12", "42");
        ASSERT_EQ(diff, "-54");

        // Negative minus a smaller negative with different size strings.
        diff = subtractTwoStrings("-12", "-442");
        ASSERT_EQ(diff, "430");

        // Negative minus a smaller negative number with same size strings.
        diff = subtractTwoStrings("-20", "-12");
        ASSERT_EQ(diff, "-8");       

        // Negative minus a larger negative number with same size strings.
        diff = subtractTwoStrings("-12", "-20");
        ASSERT_EQ(diff, "8");

        // Reverse the negative-negative subtraction.
        diff = subtractTwoStrings("-442", "-12");
        ASSERT_EQ(diff, "-430");

        // Subtraction with two zero inputs.
        diff = subtractTwoStrings("0", "0");
        ASSERT_EQ(diff, "0");

        // Two negative numbers where the first is greater.
        diff = subtractTwoStrings("-12", "-42");
        ASSERT_EQ(diff, "30");

        // Borrowing from the next digit.
        diff = subtractTwoStrings("100", "1");
        ASSERT_EQ(diff, "99");

        // Borrowing across multiple consecutive zeros.
        diff = subtractTwoStrings("1000", "1");
        ASSERT_EQ(diff, "999");

        // Borrowing across zeros with a multi-digit subtrahend.
        diff = subtractTwoStrings("10000", "101");
        ASSERT_EQ(diff, "9899");

        // Subtracting numbers with alternating digits.
        diff = subtractTwoStrings("1005", "998");
        ASSERT_EQ(diff, "7");

        // No borrowing required.
        diff = subtractTwoStrings("9876", "1234");
        ASSERT_EQ(diff, "8642");

        // Positive minus positive with same absolute value strings.
        diff = subtractTwoStrings("42", "42");
        ASSERT_EQ(diff, "0");

        // Negative minus negative with same absolute value strings.
        diff = subtractTwoStrings("-42", "-42");
        ASSERT_EQ(diff, "0");

        // Positive minus negative with same absolute value strings.
        diff = subtractTwoStrings("42", "-42");
        ASSERT_EQ(diff, "84");

        // Negative minus positive with same absolute value strings.
        diff = subtractTwoStrings("-42", "42");
        ASSERT_EQ(diff, "-84");

        // Try with long numbers.
        diff = subtractTwoStrings("1000000000000000000000000000000", "1");
        ASSERT_EQ(diff, "999999999999999999999999999999");

        // Try with long numbers.
        diff = subtractTwoStrings("1000000000000000000000000000000","999999999999999999999999999999");
        ASSERT_EQ(diff, "1");
    }


    /**
     * @brief Tests the multiplyTwoStrings method.
     */
    TEST(D3CMath_T, multiplyTwoStrings)
    {
        std::string product;

        // Multiplication with zero as one of the inputs.
        product = multiplyTwoStrings("0", "0");
        ASSERT_EQ(product, "0");
        product = multiplyTwoStrings("0", "42");
        ASSERT_EQ(product, "0");
        product = multiplyTwoStrings("42", "0");
        ASSERT_EQ(product, "0");

        // Multiplications with positive values.
        product = multiplyTwoStrings("7", "6");
        ASSERT_EQ(product, "42");
        product = multiplyTwoStrings("70", "6");
        ASSERT_EQ(product, "420");
        product = multiplyTwoStrings("7", "60");
        ASSERT_EQ(product, "420");

        // Multiplications with one positive and one negative value.
        product = multiplyTwoStrings("-7", "6");
        ASSERT_EQ(product, "-42");
        product = multiplyTwoStrings("7", "-6");
        ASSERT_EQ(product, "-42");
        product = multiplyTwoStrings("70", "-6");
        ASSERT_EQ(product, "-420");
        product = multiplyTwoStrings("-70", "6");
        ASSERT_EQ(product, "-420");
        product = multiplyTwoStrings("-7", "60");
        ASSERT_EQ(product, "-420");
        product = multiplyTwoStrings("7", "-60");
        ASSERT_EQ(product, "-420");

        // Multiplications with negative values.
        product = multiplyTwoStrings("-7", "-6");
        ASSERT_EQ(product, "42");
        product = multiplyTwoStrings("-70", "-6");
        ASSERT_EQ(product, "420");
        product = multiplyTwoStrings("-7", "-60");
        ASSERT_EQ(product, "420");

        // Multiplication with large values.
        product = multiplyTwoStrings("982137409812734091287340912387", "1279863498127643987126349812734");
        ASSERT_EQ(product, "1257001820964949313913310643837770272853987552233914150936058");
        product = multiplyTwoStrings("-982137409812734091287340912387", "1279863498127643987126349812734");
        ASSERT_EQ(product, "-1257001820964949313913310643837770272853987552233914150936058");
        product = multiplyTwoStrings("-982137409812734091287340912387", "-1279863498127643987126349812734");
        ASSERT_EQ(product, "1257001820964949313913310643837770272853987552233914150936058");
    }


    /**
     * @brief Tests the largestProductOfXConsecutiveNumbers method.
     */
    TEST(D3CMath_T, largestProductOfXConsecutiveNumbers)
    {
        // Basic cases.
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("12345", 2), 20);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("12345", 3), 60);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("12345", 4), 120);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("12345", 5), 120);

        // Single-digit products.
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("12345", 1), 5);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("55555", 1), 5);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("00000", 1), 0);

        // Sequences containing zeros.
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("10203", 2), 0);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("1230456", 3), 120);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("00000", 3), 0);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("90909", 2), 0);

        // Largest product at the beginning.
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("987123", 2), 72);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("987123", 3), 504);

        // Largest product at the end.
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("123987", 2), 72);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("123987", 3), 504);

        // Repeated digits.
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("77777", 3), 343);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("99999", 4), 6561);

        // Product of the entire string.
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("1234", 4), 24);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("9876", 4), 3024);

        // Boundary conditions for x.
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("58321", 1), 8);
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("123456", 6), 720);

        // Invalid values of x.
        EXPECT_EQ(largestProductOfXConsecutiveNumbers("12345", 0), 0);
        EXPECT_THROW(largestProductOfXConsecutiveNumbers("12345", 6), error::MIAException);

        // Empty string.
        EXPECT_THROW(largestProductOfXConsecutiveNumbers("", 1), error::MIAException);
        EXPECT_THROW(largestProductOfXConsecutiveNumbers("", 0), error::MIAException);

        // Large numeric sequence.
        EXPECT_EQ(largestProductOfXConsecutiveNumbers(
            "73167176531330624919225119674426574742355349194934", 4), 1512);
    }


    /**
     * @brief Tests the productCollatzSequence method.
     */
    TEST(D3CMath_T, productCollatzSequence)
    {
        // TODO
    }


    /**
     * @brief Tests the latticePathsOfSquare method.
     */
    TEST(D3CMath_T, latticePathsOfSquare)
    {
        // TODO
    }


    /**
     * @brief Tests the solveQuadraticFormula method.
     */
    TEST(D3CMath_T, solveQuadraticFormula)
    {
        // TODO
    }


    /**
     * @brief Tests the isPalindrome method.
     */
    TEST(D3CMath_T, isPalindrome)
    {
        // TODO
    }
} //namespace math

