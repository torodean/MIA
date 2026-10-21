/**
 * @file MathTypes_T.cpp
 * @author Antonius Torode
 * @date 10/21/2026
 * @brief Unit tests covering the features and methods in MathTypes.hpp
 */ 


#include <gtest/gtest.h>

// Include the assocuated header file for methods to test.
#include "MathTypes.hpp"

namespace math
{
    /**
     * @brief Tests that the Coordinate operator== functions correctly.
     */
    TEST(MathTypes_T, coordinateEqualityOperator)
    {
        // Check integer coordinates.
        Coordinate<int> coord1(5, 6);
        Coordinate<int> coord2(5, 6);
        Coordinate<int> coord3(5, 8);
        Coordinate<int> coord4(7, 6);
        ASSERT_EQ(coord1, coord2);
        ASSERT_NE(coord1, coord3);
        ASSERT_NE(coord1, coord4);
        
        // Check integer coordinates.
        Coordinate<uint16_t> coord5(5, 6);
        Coordinate<uint16_t> coord6(5, 6);
        Coordinate<uint16_t> coord7(5, 8);
        Coordinate<uint16_t> coord8(7, 6);
        ASSERT_EQ(coord5, coord6);
        ASSERT_NE(coord5, coord7);
        ASSERT_NE(coord5, coord8);
    }
} // namespace math
