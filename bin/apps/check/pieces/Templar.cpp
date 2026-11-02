/**
 * @file Templar.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the logic for the Templar piece.
 */

#include <cstdint>

// Include the associated header file.
#include "Templar.hpp"
#include "BoardState.hpp"

namespace check
{
    std::vector<math::Coordinate<uint8_t>> TemplarLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        // Define the jumps a Templar can make.
        const std::vector<math::Coordinate<int8_t>> steps =
        {
            { 0,  2},
            { 0, -2},
            { 2,  0},
            {-2,  0},
        };

        // Templars jump other pieces in straight lines and moves two squares.
        return generateStepMoves(currentPos, steps, boardState, color);
    }
} // namespace check
