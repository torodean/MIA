/**
 * @file Knight.hpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief This defines the logic for the knight piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "MathTypes.hpp"

namespace check
{
    /**
     * This defines the logic for a knight piece.
     */
    class KnightLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a knight piece.
        KnightLogic() : BasePieceLogic(PieceType::Knight) {};

        /**
         * This returns the legal moves for a knight piece. A knight moves in
         * an L-shape: two squares horizontally and one vertically, or two
         * squares vertically and one horizontally. A knight can jump over
         * other pieces. It can capture an opposing piece but cannot move onto
         * a square occupied by a piece of the same color.
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class Knight
} // namespace check
