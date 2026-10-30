/**
 * @file Queen.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the logic for the queen piece.
 */

#include <cstdint>

// Include the associated header file.
#include "Queen.hpp"
#include "BoardState.hpp"

namespace check
{
    std::vector<math::Coordinate<uint8_t>> QueenLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        // Queens combine the straight moves of a rook with the diagonal moves of a bishop.
        std::vector<math::Coordinate<uint8_t>> legalMoves =
            generateStraightMoves(currentPos, boardState, color);

        std::vector<math::Coordinate<uint8_t>> diagonalMoves =
            generateDiagonalMoves(currentPos, boardState, color);

        legalMoves.insert(legalMoves.end(), diagonalMoves.begin(), diagonalMoves.end());

        return legalMoves;
    }
} // namespace check
