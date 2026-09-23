/**
 * @file MathUtils_T.cpp
 * @author Antonius Torode
 * @date 05/20/2025
 * Description: Unit tests for MathUtils.hpp using Google Test framework. 
 *     Tests cover the methods in MathUtils.
 */ 

#include <gtest/gtest.h>

// Include the assocuated header file for methods to test.
#include "MathUtils.hpp"

namespace math
{
    // Test randomInt within bounds.
    TEST(MathUtils, RandomIntWithinBounds) 
    {
        int min = 1, max = 10, seed = 42;
        for (int i = 0; i < 100; i++) 
        {
            int val = randomInt(min, max, seed, false);
            EXPECT_GE(val, min);
            EXPECT_LE(val, max);
        }
    }
    
    // Test randomInt returns consistent result with fixed seed and no time usage.
    TEST(MathUtils, RandomIntConsistentWithSeed) 
    {
        int seed = 123;
        int val1 = randomInt(1, 100, seed, false);
        int val2 = randomInt(1, 100, seed, false);
        EXPECT_EQ(val1, val2);
    }
    
    // Test roll function with simple valid inputs.
    TEST(MathUtils, RollSimpleDice) 
    {
        EXPECT_GE(roll("1d6"), 1);
        EXPECT_LE(roll("1d6"), 6);
    
        EXPECT_GE(roll("2d4"), 2);
        EXPECT_LE(roll("2d4"), 8);
    }
    
    /* TODO - this method doesn't handle this currently.
    // Test roll with invalid input should handle gracefully (depends on implementation, here just test no crash).
    TEST(MathUtils, RollInvalidInput) 
    {
        EXPECT_NO_THROW(roll("invalid"));
    }
    */
    
    // Test rolldXX returns values within expected range.
    TEST(MathUtils, RolldXXRange) 
    {
        int seed = 99;
        int diceSides[] = {4, 6, 8, 10, 20};
        for (int sides : diceSides) 
        {
            int val = rolldXX(sides, seed);
            EXPECT_GE(val, 1);
            EXPECT_LE(val, sides);
        }
    }

    /**
     * @brief Verifies saturatingAdd returns the normal sum for in-range inputs.
     */
    TEST(MathUtils, SaturatingAddNormal)
    {
        EXPECT_EQ(saturatingAdd(0, 0), 0);
        EXPECT_EQ(saturatingAdd(100, 20), 120);
        EXPECT_EQ(saturatingAdd(5, -10), -5);
        EXPECT_EQ(saturatingAdd(-5, -5), -10);
    }

    /**
     * @brief Verifies saturatingAdd caps positive overflow at INT_MAX.
     */
    TEST(MathUtils, SaturatingAddOverflow)
    {
        int max = std::numeric_limits<int>::max();
        EXPECT_EQ(saturatingAdd(max, 1), max);
        EXPECT_EQ(saturatingAdd(max - 5, 10), max);
        EXPECT_EQ(saturatingAdd(0, max), max);
    }

    /**
     * @brief Verifies saturatingAdd caps negative overflow at INT_MIN.
     */
    TEST(MathUtils, SaturatingAddUnderflow)
    {
        int min = std::numeric_limits<int>::lowest();
        EXPECT_EQ(saturatingAdd(min, -1), min);
        EXPECT_EQ(saturatingAdd(min + 5, -10), min);
        EXPECT_EQ(saturatingAdd(0, min), min);
    }
} // namespace math
