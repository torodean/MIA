/**
 * @file Golem.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the logic for the Golem piece.
 */

#include <cstdint>

// Include the associated header file.
#include "Golem.hpp"
#include "BoardState.hpp"
#include "MoveGeneration.hpp"

namespace check
{
    std::vector<math::Coordinate<uint8_t>> GolemLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        // Golems move in straight lines up to three squares.
        return generateStraightMoves(currentPos, boardState, color, 3);
    }
} // namespace check
