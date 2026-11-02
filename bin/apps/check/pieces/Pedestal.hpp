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
    }; // class Pedestal
} // namespace check
