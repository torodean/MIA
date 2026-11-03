/**
 * @file MoveGeneration.cpp
 * @author Antonius Torode
 * @date 11/03/2026
 * @brief Implements the features in MoveGeneration.hpp
 */

// Include the associated header file.
#include "MoveGeneration.hpp"
#include "BoardState.hpp"

namespace check
{
    namespace
    {
        /**
         * Checks each given direction starting from currentPos, gathering the legal
         * moves of a piece that moves in straight lines until blocked. Empty squares
         * are added as legal moves. A direction stops at the board edge or at the
         * first occupied square, which is added as a capture when it holds an
         * opposing piece and is simply blocked when it holds a friendly piece.
         *
         * @param currentPos The position of the piece requesting the moves.
         * @param directions The direction coordinates to check, each stored as {x, y}.
         * @param boardState The current state of the board.
         * @param color The color of the piece requesting the moves.
         * @param maximumDistance The maximum distance in each direction to move. Zero 
         *        represents no maximum move distance.
         * @return The squares this piece can legally move to.
         */
        std::vector<math::Coordinate<uint8_t>> movesInDirections(
            const math::Coordinate<uint8_t>& currentPos,
            const std::vector<math::Coordinate<int8_t>>& directions,
            const BoardState& boardState,
            Color color,
            uint8_t maximumDistance = 0)
        {
            std::vector<math::Coordinate<uint8_t>> legalMoves;

            for (const auto& direction : directions)
            {
                // Tracks the total distance moved to stay less than the maximum.
                uint8_t distanceMoved = 0;

                math::Coordinate<uint8_t> destination = currentPos;

                // Continue in this direction until reaching the board edge or a piece.
                while (true)
                {
                    destination.x += direction.x;
                    destination.y += direction.y;
                    distanceMoved++;

                    /*
                     * Stop if the destination is outside the board. Stepping past an
                     * edge wraps the coordinate around, which is never on the board.
                     */
                    if (!boardState.isSquareOnBoard(destination))
                    {
                        break;
                    }

                    // An occupied square blocks further movement.
                    if (boardState.isSquareOccupied(destination))
                    { // An opposing piece can be captured, but not a friendly piece.
                        if (boardState.containsOpponentPiece(destination, color))
                        {
                            legalMoves.push_back(destination);
                        }

                        break;
                    }

                    // The square is unoccupied and can be moved to.
                    legalMoves.push_back(destination);

                    if (maximumDistance == 0)
                    { // No max case.
                        continue;
                    }
                    else if (distanceMoved >= maximumDistance)
                    { // Already moved the maximum distance.
                        break;
                    }
                }
            }

            return legalMoves;
        }
    } // namespace


    std::vector<math::Coordinate<uint8_t>> generateStraightMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color,
        uint8_t maximumDistance)
    {
        // Define the four straight directions in which these pieces move.
        const std::vector<math::Coordinate<int8_t>> directions =
        {
            { 0,  1}, // Up.
            { 0, -1}, // Down.
            { 1,  0}, // Right.
            {-1,  0}  // Left.
        };

        return movesInDirections(currentPos, directions, boardState, color, maximumDistance);
    }


    std::vector<math::Coordinate<uint8_t>> generateDiagonalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color,
        uint8_t maximumDistance)
    {
        // Define the four diagonal directions in which these pieces move.
        const std::vector<math::Coordinate<int8_t>> directions =
        {
            { 1,  1}, // Up and right.
            {-1,  1}, // Up and left.
            { 1, -1}, // Down and right.
            {-1, -1}  // Down and left.
        };

        return movesInDirections(currentPos, directions, boardState, color, maximumDistance);
    }


    std::vector<math::Coordinate<uint8_t>> generateStepMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const std::vector<math::Coordinate<int8_t>>& directions,
        const BoardState& boardState,
        Color color)
    {
        std::vector<math::Coordinate<uint8_t>> legalMoves;

        for (const auto& direction : directions)
        {
            math::Coordinate<uint8_t> destination = currentPos;
            destination.x += direction.x;
            destination.y += direction.y;

            /*
             * Skip if the destination is outside the board. Stepping off an
             * edge wraps the coordinate around, which is never on the board.
             */
            if (!boardState.isSquareOnBoard(destination))
            {
                continue;
            }

            /*
             * These pieces move over whatever is in their way, so only the
             * destination matters. An opposing piece can be captured, but not
             * a friendly piece.
             */
            if (!boardState.isSquareOccupied(destination) ||
                boardState.containsOpponentPiece(destination, color))
            {
                legalMoves.push_back(destination);
            }
        }

        return legalMoves;
    }
} // namespace check
