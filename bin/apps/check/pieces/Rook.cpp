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
        std::vector<math::Coordinate<uint8_t>> legalMoves;

        // Define the four directions in which a rook can move.
        const int8_t directions[4][2] =
        {
            { 0,  1}, // Up.
            { 0, -1}, // Down.
            { 1,  0}, // Right.
            {-1,  0}  // Left.
        };

        for (const auto& direction : directions)
        {
            int16_t x = currentPos.x;
            int16_t y = currentPos.y;

            // Continue in this direction until reaching the board edge or a piece.
            while (true)
            {
                x += direction[0];
                y += direction[1];

                // Stop if the destination is outside the board.
                if (x < 0 || x >= boardState.getXSize() ||
                    y < 0 || y >= boardState.getYSize())
                {
                    break;
                }

                uint8_t xPos = static_cast<uint8_t>(x);
                uint8_t yPos = static_cast<uint8_t>(y);

                // An occupied square blocks further movement.
                if (boardState.isSquareOccupied({xPos, yPos}))
                {
                    if (boardState.containsOpponentPiece({xPos, yPos}, color))
                    { // An opposing piece can be captured, but not a friendly piece.
                        legalMoves.push_back({xPos, yPos});
                    }

                    break;
                }

                // The square is unoccupied and can be moved to.
                legalMoves.push_back({xPos, yPos});
            }
        }

        return legalMoves;
    }
} // namespace check
