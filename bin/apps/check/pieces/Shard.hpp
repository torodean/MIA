/**
 * @file Shard.hpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief This defines the logic for the Shard piece.
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
     * This defines the logic for a Shard piece.
     */
    class ShardLogic : public BasePieceLogic
    {
    public:
        /// The main constructor for a Shard piece.
        ShardLogic() : BasePieceLogic(PieceType::Shard) {};

        /**
         * This returns the legal moves for a Shard piece. A Shard can move
         * up to three squares diagonally, provided that its path is not
         * blocked by another piece. A Shard can capture an opposing piece
         * but cannot move onto a square occupied by a piece of the same color.
         */
        std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) override;
    }; // class ShardLogic
} // namespace check
