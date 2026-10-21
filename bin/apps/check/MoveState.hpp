/**
 * @file MoveState.hpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief This defines the state of a single move (what piece was moved,
 *        where it moved, and where it moved from).
 */
#pragma once

#include "MathTypes.hpp"
#include "Piece.hpp"

namespace check
{
    /**
     * This defines the state for a single move.
     */
    struct MoveState
    {
        /// The piece that was moved.
        Piece piece;
        /// The starting location of a piece.
        math::Coordinate<uint8_t> startingPos;
        /// The ending location of a piece.
        math::Coordinate<uint8_t> endingPos;
        /// Stores if the last move was en passant.
        bool wasEnPassant{false};

        /*
         * Stores whether the last move triggers a pawn promotion. When this occurs,
         * the piece that is on the square will be updated, the piece in this move
         * still remains as the pawn.
         */
        bool triggersPawnPromo{false};
    }; // class MoveState
} // namespace check
