/**
 * @file Pawn.hpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief This defines the logic for the pawn piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "MathTypes.hpp"

namespace check
{
    /**
     * This defines the logic for a pawn piece.
     */
    class PawnLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a pawn piece.
        PawnLogic() : BasePieceLogic(PieceType::Pawn) {};

        /**
         * This returns the legal moves for a pawn piece. For a pawn, the 
         * piece can move one square forward (positive direction for white,
         * and negative for black). A pawn can capture a piece if it's
         * diagonal to it. If a pawn is on the second row (the usual starting
         * position), it can instead move up to two squares forward. En passant
         * allows a pawn to take another pawn diagonally if a pawn moves two squares
         * past it (ending up next to this pawn).
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class Pawn
} // namespace check
