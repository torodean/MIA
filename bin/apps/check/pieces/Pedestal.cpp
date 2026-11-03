/**
 * @file Pedestal.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the logic for the Pedestal piece.
 */

#include <cstdint>

// Include the associated header file.
#include "Pedestal.hpp"
#include "BoardState.hpp"
#include "MoveGeneration.hpp"

namespace check
{
    std::vector<math::Coordinate<uint8_t>> PedestalLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        std::vector<math::Coordinate<uint8_t>> legalMoves;

        // Define the four straight directions in which these pieces move.
        const std::vector<math::Coordinate<int8_t>> directions =
        {
            { 0,  1}, // Up.
            { 0, -1}, // Down.
            { 1,  0}, // Right.
            {-1,  0}  // Left.
        };

        for (const auto& direction : directions)
        {
            math::Coordinate<uint8_t> destination = currentPos;

            // Continue in this direction until reaching the board edge or a piece.
            while (true)
            {
                math::Coordinate<uint8_t> prevDestination = destination;
                /*
                 * Similar to the rook's logic, except only add the last valid square in 
                 * each direction. It's either the edge of the board or an opponent's piece.
                 */
                destination.x += direction.x;
                destination.y += direction.y;

                /*
                 * Stop if the destination is outside the board. Stepping past an
                 * edge wraps the coordinate around, which is never on the board.
                 */
                if (!boardState.isSquareOnBoard(destination))
                {
                    legalMoves.push_back(prevDestination);
                    break;
                }

                // An occupied square blocks further movement.
                if (boardState.isSquareOccupied(destination))
                { // An opposing piece can be captured, but not a friendly piece.
                    if (boardState.containsOpponentPiece(destination, color))
                    { // This square contains an opponents piece.
                        legalMoves.push_back(destination);
                        break;
                    }
                    else
                    { // This square contains a friendly piece.
                        legalMoves.push_back(prevDestination);
                        break;
                    }

                    break;
                }
            }
        }

        return legalMoves;
    }


    std::vector<math::Coordinate<uint8_t>> PedestalLogic::getAttackedSquares(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        return generateStraightMoves(currentPos, boardState, color);
    }
} // namespace check
