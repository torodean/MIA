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
        std::vector<math::Coordinate<uint8_t>> legalMoves;

        // Define the eight directions in which a queen can move.
        const int8_t directions[8][2] = 
        {
            { 0,  1}, // Up.
            { 0, -1}, // Down.
            { 1,  0}, // Right.
            {-1,  0}, // Left.
            { 1,  1}, // Up and right.
            {-1,  1}, // Up and left.
            { 1, -1}, // Down and right.
            {-1, -1}  // Down and left.
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
                { // An opposing piece can be captured, but not a friendly piece.  
                    if (boardState.containsOpponentPiece({xPos, yPos}, color))
                    {
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
