/**
 * @file Ankh.hpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief This defines the logic for the Ankh piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "MathTypes.hpp"

namespace check
{
    /**
     * This defines the logic for a Ankh piece.
     */
    class AnkhLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for an Ankh piece.
        AnkhLogic() : BasePieceLogic(PieceType::Ankh) {};

        /**
         * This returns the legal moves for an Ankh piece. An Ankh moves two
         * squares diagonally (either direction) and can jump pieces. An Ankh can 
         * capture an opposing piece but cannot move onto a square occupied by a piece 
         * of the same color.
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class AnkhLogic
} // namespace check
