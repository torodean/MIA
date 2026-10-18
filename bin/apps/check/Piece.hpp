/**
 * @file Piece.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief This defines storage for a piece and a base class to define
 *        behaviors for new pieces.
 */
#pragma once

#include <string>
#include <vector>

#include "MathTypes.hpp"

namespace check
{
    /// Predeclare BoardState so BasePieceLogic can reference it.
    class BoardState;

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

    /**
     * Converts a piece type into its string representation.
     * @note This produces all-lowercase output.
     * @param type The piece type to convert.
     * @return The string representation of the piece type, matching the
     *         enumerator name (e.g. PieceType::Knight becomes "knight").
     *         PieceType::None becomes "none".
     */
    std::string pieceTypetoString(PieceType type);

    /**
     * Converts a string into a piece type. Matching is case insensitive,
     * so "knight", "Knight", and "KNIGHT" all resolve to PieceType::Knight.
     *
     * @param type The string representation of a piece type.
     * @return The matching piece type.
     * @throws MIAException if the string does not match any piece type.
     */
    PieceType stringToPieceType(const std::string& type);

    /**
     * The order here matters. The players are assigned an index, and their
     * colors correspond to that index. So player 1 = white, 2 = black, etc...
     */
    enum class Color : uint8_t
    {
        None,       ///< Represents a neutral piece.
        White,      ///< The usual white color.
        Black,      ///< The usual black color.
        Red,
        Green,
    };

    /**
     * Converts a color into its string representation.
     * @note This produces all-lowercase output.
     * @param color The color to convert.
     * @return The string representation of the color, matching the
     *         enumerator name (e.g. Color::White becomes "white").
     */
    std::string colorToString(Color color);

    /**
     * Converts a string into a color. Matching is case insensitive, so
     * "white", "White", and "WHITE" all resolve to Color::White.
     *
     * @param color The string representation of a color.
     * @return The matching color.
     * @throws MIAException if the string does not match any color.
     */
    Color stringToColor(const std::string& color);

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
        Color color{Color::White};
    };

    /**
     * Stores a position for a single piece. This holds information for
     * a single piece and its location.
     */
    struct PiecePosition
    {
        Piece piece;
        math::Coordinate<uint8_t> coordinate;
    };
    
    /**
     * This class is a base virtual class for any piece. Every piece needs
     * some sort of logic defining how it moves, captures, legal moves, etc.
     * Each piece should have a class to define this logic, and each of those
     * classes should inherit this base class and follow its structure.
     */
     class BasePieceLogic
     {
     public:
        /**
         * The main constructor for this class. This takes only the PieceType,
         * which is stored and defines which type of piece this object defines
         * the logic for.
         */
        BasePieceLogic(PieceType pieceType);

        /**
         * This returns the legal moves for a piece based on the current state of 
         * the board. This uses canJumpPieces(), the color of the piece (for direction),
         * and the current state of the board to do this. This should be implemented
         * for every derived piece.
         */
        virtual std::vector<math::Coordinate<uint8_t>> getLegalMoves(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color) = 0;

    protected:

        /**
         * Determines whether a piece can jump other pieces. This is off by default,
         * as most pieces cannot jump other pieces (for the primary chess pieces, the
         * knight is the classic example of one that can).
         */
        virtual bool canJumpPieces() { return false; };

    private:

        PieceType type; ///< Stores what type of piece this object defines logic for.
     }; // class BasePieceLogic
} // namespace check
