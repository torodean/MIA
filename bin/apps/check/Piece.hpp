/**
 * @file Piece.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief This defines storage for a piece and a base class to define
 *        behaviors for new pieces.
 */
#pragma once

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
        King,       ///< The King.
        Queen,      ///< The Queen.
        Pawn,       ///< The Pawn.
        Bishop,     ///< The Bishop.
        Knight,     ///< The Knight.
        Rook,       ///< The Rook.
    }; // enum PieceType
    
    /// Defines an enum for the colors of pieces.
    enum class Color : uint8_t
    {
        White,      ///< The usual white color.
        Black,      ///< The usual black color.
    };

    /**
     * This is a storage class to represent one piece. It is a highly
     * simplified representation of a piece which is intended to simply
     * hold a 'state' representation of a piece and not any functionality.
     */
    struct Piece
    {
        /// An enum representation indicating the type of piece.
        PieceType type{PieceType::None};
        /// The color of the piece.
        Color color{Color::white};
    };
} // namespace check
