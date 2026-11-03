/**
 * @file Pedestal.hpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief This defines the logic for the Pedestal piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "PieceType.hpp"
#include "MathTypes.hpp"

namespace check
{
    /**
     * This defines the logic for a Pedestal piece.
     */
    class PedestalLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a Pedestal piece.
        PedestalLogic() : BasePieceLogic(PieceType::Pedestal) {};

        /**
         * This returns the legal moves for a Pedestal piece. A Pedestal can move any
         * horizontally or vertically, but it must move as far as possible in the chosen
         * direction. A Pedestal can capture an opposing piece but cannot move onto a 
         * square occupied by a piece of the same color.
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;

        /**
         * This returns all squares attacked by a pedestal. A pedestal's attacks change
         * based on where the other pieces are. This method is primarily used to determine
         * things like checks, moving through checks, and other things related to the
         * follow-up moves. Therefore, the exact places it can attack in the moment are
         * less relevant than where it could attack after the next move. For 'after the
         * next move, this is essentially anywhere that a rook would attack.
         *
         * TODO - I need to make sure I didn't miss somewhere that the above statement
         *        might not be true for.
         *
         * @param currentPos The current coordinate of the pedestal.
         * @param boardState The current state of the board.
         * @param color The color of the pedestal, which determines its attack direction.
         */
        std::vector<math::Coordinate<uint8_t>> getAttackedSquares(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class PedestalLogic
} // namespace check
