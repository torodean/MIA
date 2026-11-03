/**
 * @file PieceType.hpp
 * @author Antonius Torode
 * @date 11/03/2026
 * @brief This defines the enumeration of all defined piece types, along with
 *        the conversions between the piece types and their string
 *        representations.
 */
#pragma once

#include <cstdint>
#include <string>

namespace check
{
    /**
     * Represents the various defined pieces. Each of these pieces should have
     * a class representing the logic and functionality of the piece, while
     * this enum is simply an indicator for those types.
     *
     * For now, this has all of the normal chess pieces defined, new pieces
     * can be added later after these are working and coded.
     */
    enum class PieceType : uint8_t
    {
        None,       ///< Represents no piece.
        King,       ///< The King. (Keep this entry first for iterations)
        Queen,      ///< The Queen.
        Pawn,       ///< The Pawn.
        Bishop,     ///< The Bishop.
        Knight,     ///< The Knight.
        Rook,       ///< The Rook.
        Templar,    ///< The Templar (custom knight-rook like piece).
        Ankh,       ///< The Ankh (custom knight-bishop like piece).
        Shard,      ///< The Shard (custom bishop-like piece).
        Golem,      ///< The Golem (custom rook-like piece).
        Spade,      ///< The Spade (custom pawn-like piece).
        Pedestal,   ///< The Pedestal (custom rook-like piece).
        Count       ///< Number of defined piece types. (Keep last!)
    }; // enum PieceType

    /**
     * Increments the piece type to the next enumerator.
     * @param type The piece type to increment.
     * @return A reference to the incremented piece type.
     * @note This returns a maximum value of PieceType::Count.
     */
    PieceType& operator++(PieceType& type);

    /**
     * Converts a piece type into its string representation.
     * @note This produces all-lowercase output.
     * @param type The piece type to convert.
     * @return The string representation of the piece type, matching the
     *         enumerator name (e.g. PieceType::Knight becomes "knight").
     *         PieceType::None becomes "none".
     */
    std::string pieceTypeToString(PieceType type);

    /**
     * Converts a string into a piece type. Matching is case insensitive,
     * so "knight", "Knight", and "KNIGHT" all resolve to PieceType::Knight.
     *
     * @param type The string representation of a piece type.
     * @return The matching piece type, or PieceType::None if there is no match.
     */
    PieceType stringToPieceType(const std::string& type);
} // namespace check
