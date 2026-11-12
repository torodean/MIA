/**
 * @file MoveState.hpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief This defines the state of a single move (what piece was moved,
 *        where it moved, and where it moved from).
 */
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "MathTypes.hpp"
#include "Piece.hpp"

namespace check
{
    /**
     * This stores the rook move that accompanies a king's castling move.
     */
    struct CastlingMetaData
    {
        /// The starting location of the castling rook.
        math::Coordinate<uint8_t> rookStartingPos;
        /// The ending location of the castling rook.
        math::Coordinate<uint8_t> rookEndingPos;
    };

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

        /// The rook move of a castle, present only when wasCastling is true.
        std::optional<CastlingMetaData> castlingData{std::nullopt};
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

    /**
     * This converts a MoveState's move coords to a string. This is used for
     * saving this object's move as a string and then decoding it later with
     * loadFromSaveLine (such as during file saves).
     *
     * @note This will only convert the startingPos and endingPos of the MoveState.
     *       The intent is that the move playback can reconstruct a board state,
     *       but only the coorindates of the MoveState are needed for that process. 
     *
     * @param moveState The MoveState to stringify.
     * @return The move coordinates of MoveState as a string.
     */
    std::string createSaveLine(const MoveState& moveState);

    /**
     * This extracts the moves from a string created by createSaveLine().
     *
     * @note The returned MoveState's bool values are not set, because those
     *       are typically determined later and also not needed for move-playback.
     *
     * @paramm input The input as a string.
     * @return The Move constructed from the input string.
     * @throws MIAException if the input string is not correctly serialized.
     */
    MoveState loadFromSaveLine(const std::string& input);
} // namespace check
