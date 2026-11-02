/**
 * @file Templar.hpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief This defines the logic for the Templar piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "MathTypes.hpp"

namespace check
{
    /**
     * This defines the logic for a Templar piece.
     */
    class TemplarLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a Templar piece.
        TemplarLogic() : BasePieceLogic(PieceType::Templar) {};

        /**
         * This returns the legal moves for a Templar piece. A Templar moves two
         * squares horizontally or vertically and can jump pieces. A Templar can 
         * capture an opposing piece but cannot move onto a square occupied by a piece 
         * of the same color.
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class TemplarLogic
} // namespace check
