/**
 * @file King.hpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief This defines the logic for the king piece.
 */
#pragma once

#include <vector>
#include <cstdint>

#include "Piece.hpp"
#include "MathTypes.hpp"
#include "BoardState.hpp"

namespace check
{
    /**
     * This defines the logic for a king piece.
     */
    class KingLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a king piece.
        KingLogic() : BasePieceLogic(PieceType::King) {};

        /**
         * This returns the legal moves for a king piece. A king can move one
         * square horizontally, vertically, or diagonally, provided that the
         * destination is on the board and is not occupied by a piece of the
         * same color. A king can capture an opposing piece. Castling is a
         * special king move that is also available.
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class King

    /**
     * Determines whether the specified player's king is in check.
     * @param state The current state of the board.
     * @param color The color of the king in question.
     * @return true if the king is in check, false otherwise.
     */
    bool isKingInCheck(const BoardState& state, Color color);
} // namespace check
