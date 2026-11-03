/**
 * @file MoveGeneration.hpp
 * @author Antonius Torode
 * @date 11/03/2026
 * @brief This defines the shared move generation helpers used by the piece
 *        logic implementations to create their possible moves.
 */
#pragma once

#include <cstdint>
#include <vector>

#include "MathTypes.hpp"
#include "Piece.hpp"

namespace check
{
    // Predeclare some classes so they can be referenced.
    class BoardState;

    /**
     * This generates the legal moves for pieces that move in straight lines
     * (e.g., rook, queen). Each of the four straight directions is checked square
     * by square starting from currentPos. Empty squares are added as legal moves.
     * A direction stops at the board edge or at the first occupied square, which
     * is added as a capture when it holds an opposing piece and is simply blocked
     * when it holds a friendly piece.
     *
     * @param currentPos The position of the piece requesting the moves.
     * @param boardState The current state of the board.
     * @param color The color of the piece requesting the moves.
     * @param maximumDistance The maximum distance in each direction to move. Zero 
     *        represents no maximum move distance.
     * @return The squares this piece can legally move to.
     */
    std::vector<math::Coordinate<uint8_t>> generateStraightMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color,
        uint8_t maximumDistance = 0);

    /**
     * This generates the legal moves for pieces that move along diagonals
     * (e.g., bishop, queen). This behaves the same as generateStraightMoves, but
     * checks the four diagonal directions instead of the straight ones.
     *
     * @param currentPos The position of the piece requesting the moves.
     * @param boardState The current state of the board.
     * @param color The color of the piece requesting the moves.
     * @param maximumDistance The maximum distance in each direction to move. Zero 
     *        represents no maximum move distance.
     * @return The squares this piece can legally move to.
     */
    std::vector<math::Coordinate<uint8_t>> generateDiagonalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color,
        uint8_t maximumDistance = 0);

    /**
     * This generates the legal moves for pieces that move a fixed number of
     * squares at a time (e.g., knight, king). Each direction is checked only
     * once, unlike the pieces handled by generateStraightMoves. These pieces
     * move over whatever is in their way, so an occupied destination is legal
     * only when it holds an opposing piece, and a friendly piece simply blocks
     * that square.
     *
     * @param currentPos The position of the piece requesting the moves.
     * @param directions The fixed offsets to check, each stored as {x, y}.
     * @param boardState The current state of the board.
     * @param color The color of the piece requesting the moves.
     * @return The squares this piece can legally move to.
     */
    std::vector<math::Coordinate<uint8_t>> generateStepMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const std::vector<math::Coordinate<int8_t>>& directions,
        const BoardState& boardState,
        Color color);
} // namespace check
