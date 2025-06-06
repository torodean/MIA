/**
 * File: VectorUtils.hpp
 * Author: Antonius Torode
 * Created on: 03/01/2021
 * Description: 
 *     This file provides a collection of utility functions for vector parsing,
 *     processing, and formatting. It provides helper functions for dealing with
 *     vectors and other useful vector-related features.
 */
#pragma once

#include <vector>

namespace VectorUtils
{
    /**
     * Compares two vectors for equality regardless of element order.
     * 
     * This function returns true if both vectors contain the same elements 
     * with the same multiplicities, but the order of elements does not matter.
     * The input vectors are copied and sorted internally.
     * 
     * @tparam T The type of elements contained in the vectors. Must be 
     *           comparable with operator<.
     * @param a[std::vector<T>] - First vector to compare (passed by value).
     * @param b[std::vector<T>] - Second vector to compare (passed by value).
     * @return [bool] - true if both vectors contain the same elements in any order.
     *     false otherwise.
     */
    template <typename T>
    bool unorderedVectorEquals(std::vector<T> a, std::vector<T> b)
    {
        if (a.size() != b.size()) 
            return false;
            
        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());
        return a == b;
    }
} // namespace types
