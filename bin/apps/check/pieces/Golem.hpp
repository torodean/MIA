/**
 * @file Golem.hpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief This defines the logic for the Golem piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "MathTypes.hpp"

namespace check
{
    /**
     * This defines the logic for a Golem piece.
     */
    class GolemLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a Golem piece.
        GolemLogic() : BasePieceLogic(PieceType::Golem) {};

        /**
         * This returns the legal moves for a Golem piece. A Golem can move up to
         * three squares horizontally or vertically, provided that its path
         * is not blocked by another piece. A Golem can capture an opposing piece
         * but cannot move onto a square occupied by a piece of the same color.
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class Golem
} // namespace check
