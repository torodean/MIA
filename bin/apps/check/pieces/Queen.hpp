/**
 * @file Queen.hpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief This defines the logic for the queen piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "MathTypes.hpp"

namespace check
{
    /**
     * This defines the logic for a queen piece.
     */
    class QueenLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a queen piece.
        QueenLogic() : BasePieceLogic(PieceType::Queen) {};

        /**
         * This returns the legal moves for a queen piece. A queen can move any
         * number of squares horizontally, vertically, or diagonally, provided
         * that its path is not blocked by another piece. A queen can capture an
         * opposing piece but cannot move onto a square occupied by a piece of
         * the same color.
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class Queen
} // namespace check
