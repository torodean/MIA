/**
 * @file ProgressValue.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A container class for managing a value of a progress marker.
 */
#pragma once

#include "ProgressMarker.hpp"

namespace progress
{
    /**
     * A struct to hold the value of a progress marker.
     */
    struct ProgressValue
    {
        /// Default constructor.
        ProgressValue() = default;
        
        /// The main constructor.
        ProgressValue(uint32_t qty) : value(qty) {}
        
        /// Getter for the value.
        unsigned int getValue() const { return value; }
        
        /// Adjusters for the value.
        void set(unsigned int val) { value = val; }
        void add(unsigned int val) { value += val; }
            
    private:        
        unsigned int value{0};  ///< The value of this progress marker.
    };
} // namespace progress marker
