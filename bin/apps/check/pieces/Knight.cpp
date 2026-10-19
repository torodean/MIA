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
        std::vector<math::Coordinate<uint8_t>> legalMoves;

        // Define the eight possible moves for a knight.
        const int8_t moves[8][2] = {
            { 1,  2},
            { 1, -2},
            {-1,  2},
            {-1, -2},
            { 2,  1},
            { 2, -1},
            {-2,  1},
            {-2, -1}
        };

        // Check each possible destination.
        for (const auto& move : moves)
        {
            int16_t x = static_cast<int16_t>(currentPos.x) + move[0];
            int16_t y = static_cast<int16_t>(currentPos.y) + move[1];

            // Skip destinations outside the board.
            if (x < 0 || x >= boardState.getXSize() ||
                y < 0 || y >= boardState.getYSize())
            {
                continue;
            }

            uint8_t xPos = static_cast<uint8_t>(x);
            uint8_t yPos = static_cast<uint8_t>(y);

            if (boardState.isSquareOccupied(xPos, yPos))
            { // A knight can jump over pieces, but cannot capture a friendly piece.
                if (boardState.containsOpponentPiece(xPos, yPos, color))
                {
                    legalMoves.push_back({xPos, yPos});
                }

                continue;
            }

            // The destination is unoccupied and can be moved to.
            legalMoves.push_back({xPos, yPos});
        }

        return legalMoves;
    }
} // namespace check
