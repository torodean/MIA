/**
 * @file Shard.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the logic for the Shard piece.
 */

#include <cstdint>

// Include the associated header file.
#include "Shard.hpp"
#include "BoardState.hpp"
#include "MoveGeneration.hpp"

namespace check
{
    std::vector<math::Coordinate<uint8_t>> ShardLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        // Shards move along diagonals up to three squares.
        return generateDiagonalMoves(currentPos, boardState, color, 3);
    }
} // namespace check
