/**
 * @file Bishop.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the logic for the bishop piece.
 */

#include <cstdint>

// Include the associated header file.
#include "Bishop.hpp"
#include "BoardState.hpp"

namespace check
{
    std::vector<math::Coordinate<uint8_t>> BishopLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        // Bishops move along diagonals.
        return generateDiagonalMoves(currentPos, boardState, color);
    }
} // namespace check
