/**
 * @file Rook.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the logic for the rook piece.
 */

#include <cstdint>

// Include the associated header file.
#include "Rook.hpp"
#include "BoardState.hpp"

namespace check
{
    std::vector<math::Coordinate<uint8_t>> RookLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        // Rooks move in straight lines.
        return generateStraightMoves(currentPos, boardState, color);
    }
} // namespace check
