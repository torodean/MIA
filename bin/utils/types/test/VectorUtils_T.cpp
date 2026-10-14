/**
 * @file VectorUtils_T.cpp
 * @author Antonius Torode
 * @date 06/05/2025
 * 
 * Unit tests for the VectorUtils library. This file contains tests
 * to verify the correctness of vector utility functions.
 * Additional tests for new utility functions should be added here.
 */

#include <vector>

#include <gtest/gtest.h>

// Include the associated file to be tested.
#include "VectorUtils.hpp"

namespace vector_utils
{
    /**
     * @brief Tests that the unorderedVectorEquals method returns true for vectors
     *        that have the same order.
     */
    TEST(VectorUtils_T, unorderedVectorEquals_sameOrderVectors) 
    {
        std::vector<int> a = {1, 2, 3, 4};
        std::vector<int> b = {1, 2, 3, 4};
        EXPECT_TRUE(unorderedVectorEquals(a, b));
    }


    /**
     * @brief Tests that the unorderedVectorEquals method returns true for vectors
     *        that have a different order but otherwise contain the same elements.
     */
    TEST(VectorUtils_T, unorderedVectorEquals_differentOrderVectors) 
    {
        std::vector<int> a = {1, 3, 2, 4};
        std::vector<int> b = {4, 2, 1, 3};
        EXPECT_TRUE(unorderedVectorEquals(a, b));
    }


    /**
     * @brief Tests that the unorderedVectorEquals returns false for vectors that
     *        are different.
     */
    TEST(VectorUtils_T, unorderedVectorEquals_differentVectorElements) 
    {
        std::vector<int> a = {1, 2, 3};
        std::vector<int> b = {1, 2, 4};
        EXPECT_FALSE(unorderedVectorEquals(a, b));
    }


    /**
     * @brief Tests that the unorderedVectorEquals returns false for vectors that
     *        are different sizes.
     */
    TEST(VectorUtils_T, unorderedVectorEquals_differentVectorSizes) 
    {
        std::vector<int> a = {1, 2, 3};
        std::vector<int> b = {1, 2, 3, 4};
        EXPECT_FALSE(unorderedVectorEquals(a, b));
    }


    /**
     * @brief Tests that the unorderedVectorEquals works with other types (strings).
     */
    TEST(VectorUtils_T, unorderedVectorEquals_WorksWithStrings) 
    {
        std::vector<std::string> a = {"apple", "banana", "cherry"};
        std::vector<std::string> b = {"banana", "cherry", "apple"};
        EXPECT_TRUE(unorderedVectorEquals(a, b));
    }


    /**
     * @brief Tests that empty vectors also show as equal.
     */
    TEST(VectorUtils_T, unorderedVectorEquals_WorksWithEmptyVectors) 
    {
        std::vector<int> a;
        std::vector<int> b;
        EXPECT_TRUE(unorderedVectorEquals(a, b));
    }
} // namespace vector_utils
