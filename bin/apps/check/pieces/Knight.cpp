/**
 * @file Knight.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the logic for the knight piece.
 */

#include <cstdint>

// Include the associated header file.
#include "Knight.hpp"
#include "BoardState.hpp"

namespace check
{
    std::vector<math::Coordinate<uint8_t>> KnightLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        // Define the eight jumps a knight can make.
        const std::vector<math::Coordinate<int8_t>> steps =
        {
            { 1,  2},
            { 1, -2},
            {-1,  2},
            {-1, -2},
            { 2,  1},
            { 2, -1},
            {-2,  1},
            {-2, -1}
        };

        // Knights can jump over pieces, but cannot capture a friendly piece.
        return generateStepMoves(currentPos, steps, boardState, color);
    }
} // namespace check
