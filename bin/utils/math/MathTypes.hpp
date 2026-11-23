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
     * @brief Represents a 2D coordinate with template components.
     * @tparam Type The type for each component of the coordinate.
     */
    template<typename Type>
    struct Coordinate 
    {
        // Default constructor.
        Coordinate() = default;

        /// Main constructor which takes and sets two coordinate values.
        Coordinate(Type xVal, Type yVal) : x(xVal), y(yVal) {}

        Type x; ///< Storage for the X coordinate value.
        Type y; ///< Storage for the Y coordinate value.

        /**
         * @brief Compares two coordinates for equality.
         * @param input The coordinate to compare against.
         * @return true if both coordinates have equal X and Y components.
         */
        bool operator==(const Coordinate<Type>& input) const
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
        os << "(" << std::to_string(coord.x) << ", " << std::to_string(coord.y) << ")";
        return os;
    }    
    
    /**
     * @brief Represents a 3D coordinate with template components.
     * @tparam Type The type for each component of the coordinate.
     */
    template<typename Type>
    struct Coordinate3
    {
        // Default constructor.
        Coordinate3() = default;

        /// Main constructor which takes and sets three coordinate values.
        Coordinate3(Type xVal, Type yVal, Type zVal) : x(xVal), y(yVal), z(zVal) {}

        Type x; ///< Storage for the X coordinate value.
        Type y; ///< Storage for the Y coordinate value.
        Type z; ///< Storage for the Z coordinate value.

        /**
         * @brief Compares three coordinates for equality.
         * @param input The coordinate to compare against.
         * @return true if both coordinates have equal X, Y, and Z components.
         */
        bool operator==(const Coordinate3<Type>& input) const
        {
            return x == input.x && y == input.y && z == input.z;
        }
    };

    /**
     * @brief Outputs a coordinate3 as a string in the format (x, y, z).
     * @tparam Type The type for each component of the coordinate.
     * @param os The output stream.
     * @param coord The coordinate3 to output.
     * @return A reference to the output stream.
     */
    template<typename Type>
    std::ostream& operator<<(std::ostream& os, const Coordinate3<Type>& coord)
    {
        os << "(" << std::to_string(coord.x)
           << ", " << std::to_string(coord.y)
           << ", " << std::to_string(coord.z) << ")";
        return os;
    }
} // namespace math
