/**
 * @file MathTypes.hpp
 * @author Antonius Torode
 * @date 06/18/2025
 * @brief This file provides a collection of general types used for various mathematical purposes.
 */
#pragma once

namespace math
{
    /**
     * @brief Represents a 2D coordinate with named integer components.
     * @tparam Type The type for each component of the coordinate.
     */
    template<typename Type>
    struct Coordinate 
    {
        // Default constructor.
        Coordinate() = default;
    
        /// Main constructor which takes and sets two integer coordinate values.
        Coordinate(Type x_val, Type y_val) : x(x_val), y(y_val) {}

        Type x; ///< Storage for the X coordinate value.
        Type y; ///< Storage for the Y coordinate value.
    };
} // namespace math
