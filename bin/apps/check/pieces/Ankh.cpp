/**
 * @file Ankh.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the logic for the Ankh piece.
 */

#include <cstdint>

// Include the associated header file.
#include "Ankh.hpp"
#include "BoardState.hpp"
#include "MoveGeneration.hpp"

namespace check
{
    std::vector<math::Coordinate<uint8_t>> AnkhLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        // Define the jumps a Ankh can make.
        const std::vector<math::Coordinate<int8_t>> steps =
        {
            { 2,  2},
            { 2, -2},
            {-2,  2},
            {-2, -2},
        };

        // Ankhs jump other pieces in straight lines and moves two squares.
        return generateStepMoves(currentPos, steps, boardState, color);
    }
} // namespace check
