/**
 * @file Piece.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief This defines storage for a piece and a base class to define
 *        behaviors for new pieces.
 */
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "MathTypes.hpp"

namespace check
{
    /// Predeclare BoardState so BasePieceLogic can reference it.
    class BoardState;

    /**
     * This enum represents different types of piece art that can be used.
     */
    enum class PieceArtType : uint8_t
    {
        Unknown,  ///< Represents an unknown/unset type.
        Glyph,    ///< Represents a unicode glyph (e.g., "\u265A").
        Image     ///< Represents a path to an image.
    };

    /**
     * Stores the art for displaying a piece. The art must match the type.
     */
    struct PieceArt
    {
        std::string pieceName; ///< The name of this piece.
        PieceArtType type;     ///< The type of art stored.
        std::string art;       ///< The art glyph or file path to an image.
    };

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
     * @return The matching color, or Color::None if there is no match.
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
        BasePieceLogic(PieceType pieceType) : type(pieceType) {};

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

        /**
         * This returns the legal captures for a piece based on the current state of
         * the board. This should be implemented for every derived piece. Since most pieces
         * have the same logic to determine this (based on legal moves), this has a
         * base implementation that can be used.
         */
        virtual std::vector<math::Coordinate<uint8_t>> getLegalCaptures(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color);

        /**
         * This returns all squares attacked by a piece based on the current state of
         * the board. This includes squares occupied by friendly pieces and unoccupied
         * squares. For most pieces, this is just the legal moves. The base implementation
         * here defines it for typical pieces. Some pieces move differently than they
         * capture (e.g., pawns), so for these, this should be re-implemented.
         */
        virtual std::vector<math::Coordinate<uint8_t>> getAttackedSquares(
            const math::Coordinate<uint8_t>& currentPos,
            const BoardState& boardState,
            Color color);

    private:

        PieceType type; ///< Stores what type of piece this object defines logic for.
     }; // class BasePieceLogic

    /**
     * Creates the appropriate logic implementation for a piece type.
     * @param type The type of piece to create logic for.
     * @return A pointer to the appropriate BasePieceLogic implementation.
     */
    std::unique_ptr<BasePieceLogic> createPieceLogic(PieceType type);

    /**
     * This is a helper to get the legal moves of any piece. This will use the
     * appropriate implemented BasePieceLogic based on the input piece and
     * return a list of legal moves.
     *
     * @note Only moves of the current player's pieces are returned, and moves
     *       which would leave (or put) that player's king in check are filtered
     *       out. When that king is in check, this means only the moves which get
     *       the king out of check are returned (move the king, capture the
     *       attacking piece, or block the attack).
     *
     * @param piecePos The piece to get the legal moves of. This contains color and location.
     * @param boardState The current state of the board.
     * @return A list of all legal moves for a piece.
     * @throws MIAException if the square this piece is on is not occupied.
     */
    std::vector<math::Coordinate<uint8_t>> getLegalMoves(const PiecePosition& piecePos,
                                                         const BoardState& boardState);

    /**
     * This is a helper to get the legal captures of any piece. This will use
     * the appropriate implemented BasePieceLogic based on the input piece and
     * return a list of legal captures.
     *
     * @note Captures which would leave (or put) this piece's king in check are
     *       filtered out, just as they are in getLegalMoves().
     *
     * @param piecePos The piece to get the legal captures of. This contains color and location.
     * @param boardState The current state of the board.
     * @return A list of all legal captures for a piece.
     * @throws MIAException if the square this piece is on is not occupied.
     */
    std::vector<math::Coordinate<uint8_t>> getLegalCaptures(const PiecePosition& piecePos,
                                                            const BoardState& boardState);

    /**
     * Determines whether moving a piece to the input square would still leave
     * this piece's king in check. The move is simulated on a copy of the input
     * board state, so the actual game state is never modified.
     *
     * This is the check which keeps the legal moves of a piece honest. A move
     * leaves the king in check when:
     *   - the king was already in check and the move did not resolve it,
     *   - the move uncovered an attack on the king (a pinned piece moving away
     *     from the line which shields the king), or
     *   - the piece is a king which steps onto a square an opposing piece
     *     attacks.
     *
     * @note Attack detection uses the pseudo-legal attacks of each piece (see
     *       BasePieceLogic::getAttackedSquares), so this never recurses back
     *       into the legal move filtering done by getLegalMoves().
     *
     * @param piecePos The piece to simulate moving. This contains color and location.
     * @param boardState The current state of the board.
     * @param destination The square to simulate moving the piece to.
     * @return true if this piece's king would still be in check after this move,
     *         false if the move leaves the king safe.
     * @throws MIAException if the square this piece is on is not occupied.
     */
    bool leavesKingInCheck(const PiecePosition& piecePos,
                           const BoardState& boardState,
                           const math::Coordinate<uint8_t>& destination);

    /**
     * This returns all squares attacked by pieces on the board.
     * @param boardState The current state of the board.
     * @return A list of all attacked squares on the board.
     * @param defendingColor The color being defended against attacks.
     */
    std::vector<math::Coordinate<uint8_t>> getAttackedSquares(const BoardState& boardState,
                                                              Color defendingColor);
} // namespace check
