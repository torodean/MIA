/**
 * @file MathTypes.hpp
 * @author Antonius Torode
 * @date 06/18/2025
 * Description: This file provides a collection of general types used for various
 *     mathematical purposes.
 */
#pragma once

namespace math
{
    /**
     * @brief Represents a 2D coordinate with named integer components.
     */
    struct Coordinate 
    {
        Coordinate(int x_val = 0, int y_val = 0) : x(x_val), y(y_val) {}
        /// Storage for the coordinate values.
        int x, y;
    };
} // namespace math
