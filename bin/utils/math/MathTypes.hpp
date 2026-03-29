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
        /// Main constructor which takes and sets two integer coordinate values.
        Coordinate(int x_val = 0, int y_val = 0) : x(x_val), y(y_val) {}
        
        int x; ///< Storage for the X coordinate value.
        int y; ///< Storage for the Y coordinate value.
    };
} // namespace math
