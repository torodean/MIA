/**
 * @file Spade.hpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief This defines the logic for the Spade piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "MathTypes.hpp"

namespace check
{
    /**
     * This defines the logic for a Spade piece.
     */
    class SpadeLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a Spade piece.
        SpadeLogic() : BasePieceLogic(PieceType::Spade) {};

        /**
         * This returns the legal moves for a Spade piece. For a Spade, the 
         * piece can move one or two squares forward (positive direction for white,
         * and negative for black). A Spade can capture a piece if it's diagonal to 
         * it. En passant allows a Spade to take another Pawn or Spade diagonally if 
         * that piece moves two squares past it (ending up next to this Spade).
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;        

        /**
         * This returns all squares attacked by a Spade. A Spade attacks the two
         * diagonally adjacent squares in its forward direction, regardless of
         * whether those squares are occupied.
         * @param currentPos The current coordinate of the Spade.
         * @param boardState The current state of the board.
         * @param color The color of the Spade, which determines its attack direction.
         */
        std::vector<math::Coordinate<uint8_t>> getAttackedSquares(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class SpadeLogic
} // namespace check
