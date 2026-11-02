/**
 * @file Spade.cpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief Implements the logic for the Spade piece.
 */

#include <cstdint>
#include <cmath>

// Include the associated header file.
#include "Spade.hpp"
#include "BoardState.hpp"
#include "MoveState.hpp"

namespace check
{
    std::vector<math::Coordinate<uint8_t>> SpadeLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        // Defines forward as +1 or -1 depending on color.
        int8_t forward = color == Color::White ? 1 : -1;

        std::vector<math::Coordinate<uint8_t>> legalMoves;
        
        // Add all moves one or two forward.
        math::Coordinate<uint8_t> destination = currentPos;
        destination.y = static_cast<uint8_t>(currentPos.y + forward);
        if (boardState.isSquareOnBoard(destination))
        { // The square ahead of it is on the board.
            if (!boardState.isSquareOccupied(destination))
            { // The square ahead is not occupied.
                legalMoves.push_back(destination);
                
                // Checking 2 squares ahead is only needed if the first wasn't occupied. 
                destination.y = static_cast<uint8_t>(currentPos.y + 2*forward);
                if (boardState.isSquareOnBoard(destination) && 
                    !boardState.isSquareOccupied(destination))
                    legalMoves.push_back(destination);
            }
        }

        // Check if any captures are available.
        if (currentPos.x + 1 < boardState.getXSize() && // Keep within board bounds.
            boardState.containsOpponentPiece({static_cast<uint8_t>(currentPos.x + 1), 
                                              static_cast<uint8_t>(currentPos.y + forward)}, color))
        { // Forward and to the right.

            legalMoves.push_back({static_cast<uint8_t>(currentPos.x + 1), 
                                  static_cast<uint8_t>(currentPos.y + forward)});
        }
        if (currentPos.x - 1 >= 0 && // Keep within board bounds.
            boardState.containsOpponentPiece({static_cast<uint8_t>(currentPos.x - 1), 
                                             static_cast<uint8_t>(currentPos.y + forward)}, color))
        { // Forward and to the left.
            legalMoves.push_back({static_cast<uint8_t>(currentPos.x - 1), 
                                  static_cast<uint8_t>(currentPos.y + forward)});
        }

        // Handle en passant.
        const std::optional<MoveState>& lastMove = boardState.getLastMove();
        if (lastMove != std::nullopt && 
            (lastMove->piece.type == PieceType::Spade || lastMove->piece.type == PieceType::Pawn) )
        { // Last move was a Spade or pawn so it may enable en passant.
            /*
             * A Positive direction indicates the last moved piece was to our right, otherwise,
             * it's to our left. Also, the absolute value of this must be exactly 1 to signal
             * that the last piece moved was one column to the right or left.
             */
            int16_t direction = static_cast<int16_t>(lastMove->endingPos.x) -
                                static_cast<int16_t>(currentPos.x);
            if (lastMove->endingPos.y == currentPos.y && // The last Spade move ended next to this one. 
                (direction == -1 || direction == 1) &&   // The last piece was in an adjacent column.
                std::abs(lastMove->endingPos.y - lastMove->startingPos.y) == 2) // The Spade moved two squares.
            { // Indicates en passant is possible.
                legalMoves.push_back({static_cast<uint8_t>(currentPos.x + direction), 
                                      static_cast<uint8_t>(currentPos.y + forward)});
            }
        }

        return legalMoves;
    }


    std::vector<math::Coordinate<uint8_t>> SpadeLogic::getAttackedSquares(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        std::vector<math::Coordinate<uint8_t>> attackedSquares;

        // Define the Spade's forward direction based on its color.
        int16_t forward = color == Color::White ? 1 : -1;

        // Check both diagonal attack squares.
        for (int16_t dx : {-1, 1})
        {
            int16_t x = static_cast<int16_t>(currentPos.x) + dx;
            int16_t y = static_cast<int16_t>(currentPos.y) + forward;

            // Skip squares outside the board.
            if (x < 0 || x >= boardState.getXSize() || y < 0 || y >= boardState.getYSize())
            {
                continue;
            }

            attackedSquares.push_back({static_cast<uint8_t>(x), static_cast<uint8_t>(y)});
        }

        return attackedSquares;
    }
} // namespace check
