/**
 * @file MathUtils_T.cpp
 * @author Antonius Torode
 * @date 05/20/2025
 * @brief: Unit tests covering the methods in MathUtils.hpp
 */ 

#include <limits>
#include <vector>

#include <gtest/gtest.h>

// Include the assocuated header file for methods to test.
#include "MathUtils.hpp"
#include "MIAException.hpp"

namespace math
{
    // Test randomInt within bounds.
    TEST(MathUtils, RandomIntWithinBounds) 
    {
        int min = 1, max = 100, seed = 42;
        for (int i = 0; i < 1000; i++) 
        {
            int val = randomInt(min, max, seed, false);
            EXPECT_GE(val, min);
            EXPECT_LE(val, max);
        }
        for (int i = 0; i < 1000; i++) 
        {
            int val = randomInt(min, max, seed, true);
            EXPECT_GE(val, min);
            EXPECT_LE(val, max);
        }
    }
    
    
    // Test randomInt returns consistent result with fixed seed and no time usage.
    TEST(MathUtils, RandomIntConsistentWithSeed) 
    {
        int seed = 123;
        int val1 = randomInt(1, 100000, seed, false);
        int val2 = randomInt(1, 100000, seed, false);
        EXPECT_EQ(val1, val2);
    }
    
    
    /*
     * Tests that random values are not duplicated when using time.
     */
    TEST(MathUtils, RandomIntNotDuplicated) 
    {
        int seed = 123;
        int val1 = randomInt(1, 100000, seed, true);
        int val2 = randomInt(1, 100000, seed, true);
        EXPECT_NE(val1, val2);
    }
    
    
    /*
     * Test that randomChance() works as expected;
     */
    TEST(MathUtils, randomChance)
    {
        // Border/edge cases.
        EXPECT_TRUE(randomChance(1.0));
        EXPECT_FALSE(randomChance(0.0));
        
        // Error cases.
        EXPECT_THROW(randomChance(-0.1), error::MIAException);
        EXPECT_THROW(randomChance(1.1), error::MIAException);
        
        /*
         * Normal use cases.
         * Since these are inherently variable/random, we can run a thousand times
         * and check that the values are within an expected range. There's always
         * a random chance these tests could fail, but the range is large enough that
         * it would be a fluke if they did.
         *
         * This tracks the number of 'true' values returned over ten thousand iterations
         * for each chance that is a multiple of 10%. It then checks that each one appears
         * some number of times that is close to the expected value.
         *
         * The sigma value is given by sqrt(p*(1-p)/n). The variance these tests
         * allow is then 4 sigma (should be sufficient) in either direction.
         */
        int iterations = 10000;
        std::vector<int> chances = {0,0,0,0,0,0,0,0,0};
        for (int i=0; i<iterations; i++)
        {
            for (size_t j=1; j<10; j++)
                if (randomChance(static_cast<double>(j)/10.0))
                    chances[j]++; // Increment if a true was returned.
        }
        
        for (size_t j=1; j<10; j++)
        { // Check the expected results.
            double sigma = std::sqrt( ( (j/10.0)*( 1.0-(j/10.0) ) ) / static_cast<double>(iterations) );
            int allowedVariance = std::round(4.0*sigma*static_cast<double>(iterations));
            EXPECT_GE(chances[j], j*static_cast<double>(iterations)/10.0 - allowedVariance);
            EXPECT_LE(chances[j], j*static_cast<double>(iterations)/10.0 + allowedVariance);
        }
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
