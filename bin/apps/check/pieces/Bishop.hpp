/**
 * @file Bishop.hpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief This defines the logic for the bishop piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "MathTypes.hpp"

namespace check
{
    /**
     * This defines the logic for a bishop piece.
     */
    class BishopLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a bishop piece.
        BishopLogic() : BasePieceLogic(PieceType::Bishop) {};

        /**
         * This returns the legal moves for a bishop piece. A bishop can move
         * any number of squares diagonally, provided that its path is not
         * blocked by another piece. A bishop can capture an opposing piece
         * but cannot move onto a square occupied by a piece of the same color.
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class Bishop
} // namespace check
