/*
 * File: VectorUtils_T.cpp
 * Author: Antonius Torode
 * Created on: 06/05/2025
 * Description:
 *     Unit tests for the VectorUtils library. This file contains tests
 *     to verify the correctness of vector utility functions.
 *     Additional tests for new utility functions should be added here.
 */

#include <gtest/gtest.h>
#include "VectorUtils.hpp"
#include <vector>

using namespace VectorUtils;

TEST(VectorUtils_UnorderedVectorEquals, ReturnsTrueForSameVectorsSameOrder) 
{
    std::vector<int> a = {1, 2, 3, 4};
    std::vector<int> b = {1, 2, 3, 4};
    EXPECT_TRUE(unorderedVectorEquals(a, b));
}

TEST(VectorUtils_UnorderedVectorEquals, ReturnsTrueForSameVectorsDifferentOrder) 
{
    std::vector<int> a = {1, 3, 2, 4};
    std::vector<int> b = {4, 2, 1, 3};
    EXPECT_TRUE(unorderedVectorEquals(a, b));
}

TEST(VectorUtils_UnorderedVectorEquals, ReturnsFalseForDifferentVectors) 
{
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {1, 2, 4};
    EXPECT_FALSE(unorderedVectorEquals(a, b));
}

TEST(VectorUtils_UnorderedVectorEquals, ReturnsFalseForDifferentSizes) 
{
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {1, 2, 3, 4};
    EXPECT_FALSE(unorderedVectorEquals(a, b));
}

TEST(VectorUtils_UnorderedVectorEquals, WorksWithStrings) 
{
    std::vector<std::string> a = {"apple", "banana", "cherry"};
    std::vector<std::string> b = {"banana", "cherry", "apple"};
    EXPECT_TRUE(unorderedVectorEquals(a, b));
}

TEST(VectorUtils_UnorderedVectorEquals, WorksWithEmptyVectors) 
{
    std::vector<int> a;
    std::vector<int> b;
    EXPECT_TRUE(unorderedVectorEquals(a, b));
}
