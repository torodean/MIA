/**
 * @file MathTypes.hpp
 * @author Antonius Torode
 * @date 06/18/2025
 * @brief This file provides a collection of general types used for various mathematical purposes.
 */
#pragma once

#include <ostream>

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
        
        /**
         * @brief Compares two coordinates for equality.
         * @param input The coordinate to compare against.
         * @return true if both coordinates have equal X and Y components.
         */
        bool operator==(const Coordinate<Type>& input)
        {
            return x == input.x && y == input.y;
        }
    };
    
    /**
     * @brief Outputs a coordinate as a string in the format (x, y).
     * @tparam Type The type for each component of the coordinate.
     * @param os The output stream.
     * @param coord The coordinate to output.
     * @return A reference to the output stream.
     */
    template<typename Type>
    std::ostream& operator<<(std::ostream& os, const Coordinate<Type>& coord)
    {
        os << "(" << coord.x << ", " << coord.y << ")";
        return os;
    }
} // namespace math
