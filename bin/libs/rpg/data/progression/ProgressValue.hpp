/**
 * @file ProgressValue.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * @brief A container class for managing a value of a progress marker.
 */
#pragma once

#include <cstdint>

#include "ProgressMarker.hpp"

namespace progress
{
    /**
     * A class to hold the value of a progress marker.
     */
    class ProgressValue
    {
    public:
        /// Default constructor.
        ProgressValue() = default;
        
        /// The main constructor.
        ProgressValue(int32_t qty);
        
        /// Getter for the value.
        int32_t get() const;
        
        /// Adjusters for the value.
        void set(int32_t val);
        void add(int32_t val);
        void subtract(int32_t val);
            
    private:        
        int32_t value{0};  ///< The value of this progress marker.
    };
} // namespace progress marker
