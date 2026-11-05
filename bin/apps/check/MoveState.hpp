/**
 * @file MoveState.hpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief This defines the state of a single move (what piece was moved,
 *        where it moved, and where it moved from).
 */
#pragma once

#include <string>
#include <vector>

#include "MathTypes.hpp"
#include "Piece.hpp"

namespace check
{
    /**
     * This defines the state for a single move.
     */
    struct MoveState
    {
        /// The piece that was moved.
        Piece piece;
        /// The starting location of a piece.
        math::Coordinate<uint8_t> startingPos;
        /// The ending location of a piece.
        math::Coordinate<uint8_t> endingPos;
        /// Stores if the last move was en passant.
        bool wasEnPassant{false};
        /// Stores if the last move was castling.
        bool wasCastling{false};
        /// Stores if the last move made a capture of another piece.
        bool wasCapture{false};

        /*
         * Stores whether the last move triggers a piece promotion. When this occurs,
         * the piece that is on the square will be updated, the piece in this move
         * still remains as the original piece (e.g., pawn, spade).
         */
        bool triggersPromo{false};
    }; // struct MoveState

    /**
     * Converts a single MoveState into the display text used for the ui move
     * history boxes. The text is "<glyph>: <start> -> <end>" for a normal move
     * and "<glyph>: <start> -x <end>" for a capture, where <start> and <end>
     * are square labels made of the file letter and rank number used by the
     * board's labeled border (e.g., "e2"), and <glyph> is the moved piece's
     * glyph art. The glyph is looked up in pieceArts by the moved piece's
     * type, and is left out along with the colon when no glyph art backs the
     * piece (e.g., when image art is used).
     *
     * @param move The move state to convert.
     * @param pieceArts The configured piece arts to look the glyph up in.
     * @return The display string for the move.
     */
    std::string moveStateToString(const MoveState& move,
                                  const std::vector<PieceArt>& pieceArts);
} // namespace check
