/**
 * @file ProgressValue.cpp
 * @author Antonius Torode
 * @date 10/12/2026
 * @brief Implements a container class for managing a value of a progress marker.
 */

// Include associated header file.
#include "ProgressValue.hpp"

#include "ProgressMarker.hpp"

namespace progress
{
    ProgressValue::ProgressValue(int32_t qty) : 
        value(qty) 
    {}


    int32_t ProgressValue::get() const 
    {
        return value;
    }


    void ProgressValue::set(int32_t val) 
    {
        value = val; 
    }


    void ProgressValue::add(int32_t val) 
    {
        value += val; 
    }


    void ProgressValue::subtract(int32_t val) 
    {
        if (val > value)
            value = 0; 
        value -= val;
    }
} // namespace progress marker
