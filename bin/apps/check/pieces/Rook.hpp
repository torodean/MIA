/**
 * @file Rook.hpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief This defines the logic for the rook piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "MathTypes.hpp"

namespace check
{
    /**
     * This defines the logic for a rook piece.
     */
    class RookLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a rook piece.
        RookLogic() : BasePieceLogic(PieceType::Rook) {};

        /**
         * This returns the legal moves for a rook piece. A rook can move any
         * number of squares horizontally or vertically, provided that its path
         * is not blocked by another piece. A rook can capture an opposing piece
         * but cannot move onto a square occupied by a piece of the same color.
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class Rook
} // namespace check
