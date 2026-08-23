/**
 * @file VectorUtils.hpp
 * @author Antonius Torode
 * @date 03/01/2021
 * Description: 
 *     This file provides a collection of utility functions for vector parsing,
 *     processing, and formatting. It provides helper functions for dealing with
 *     vectors and other useful vector-related features.
 */
#pragma once

#include <vector>
#include <sstream>
#include <string>

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
     * @param a First vector to compare (passed by value).
     * @param b Second vector to compare (passed by value).
     * @return true if both vectors contain the same elements in any order.
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
    
    /**
     * Converse a vector of values to a string. Each element of the vector is
     * separated by a comma delimiter.
     * 
     * @tparam T The type of elements contained in the vector. Must be a type
     *           which has an operator<< defined.
     * @param values The input vector to convert to a string.
     * @return A string representation of the vector.
     */
    template <typename T>
    std::string vectorToString(const std::vector<T>& values)
    {
        std::ostringstream result;

        for (size_t i=0; i<values.size(); ++i)
        {
            if (i > 0)
                result << ", ";

            result << values[i];
        }

        return result.str();
    }
} // namespace types
