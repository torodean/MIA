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

        /**
         * Returns the squares attacked by the king. Attacked squares are independent of 
         * existing pieces. This implementation avoids calling the base getLegalMoves() 
         * to prevent recursive attack detection during king-castling validation.
         *
         * @param currentPos The current position of the king.
         * @param boardState The current state of the board.
         * @param color The color of the king.
         * @return A vector of coordinates representing the squares attacked by the king.
         */
        std::vector<math::Coordinate<uint8_t>> getAttackedSquares(
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

    /**
     * Enum storage for the direction that the king can castle in is done.
     * This is an enum so that if later variations of the game are added with
     * different supporting castleing directions, this can easily be expanded.
     */
    enum class CastleDirection : uint8_t
    {
        LEFT,   ///< The direction to the left of the king (horizontally).
        RIGHT,  ///< The direction to the right of the king (horizontally).
    };

    /**
     * @brief Determines whether the specified player's king can castle.
     *
     * This checks if a rook is next to the king, and that both that rook
     * and the king have not moved this game. For typical chess, the rook is
     * always at the corner, but for variations, the rook may not have started
     * at the corner.
     *
     * @param state The current state of the board.
     * @param kingPos The position of the king in question.
     * @param direction The direction to check for castling conditions.
     * @return true if the king can castle in the direction, false otherwise.
     */
    bool canKingCastle(const BoardState& state,
                       const math::Coordinate<uint8_t>& kingPos,
                       CastleDirection direction);

    /**
     * Returns the squares that must be considered when evaluating a castling move.
     * Starts at the king's position and adds squares in the specified direction
     * until reaching an occupied square or the edge of the board. If a friendly
     * rook is encountered, also includes the square immediately beyond it if that
     * square is on the board (for special game types which randomize piece positions).
     *
     * The returned value will iterate from the kings position out, so the king will
     * always be the zero-index entry.
     *
     * @param state The current state of the board.
     * @param kingPos The position of the king in question.
     * @param direction The direction to check for castling conditions.
     * @return A vector of square states containing the king's square and the
     *         relevant squares in the specified direction.
     */
    std::vector<SquareState> getSquaresInQuestion(
        const BoardState& state,
        const math::Coordinate<uint8_t>& kingPos,
        CastleDirection direction);

    /**
     * Determines if the move in question would be a castling move. This just checks
     * if the move would move the king 2 spaces rather than one.
     *
     * @param move The move that is being made.
     * @return true if the move is a king castling, false otherwise.
     */
    bool isCastlingMove(const MoveState& move);

    /**
     * Stores some move data for a rook that is being castled with.
     */
    struct CastlingMetaData
    {
        math::Coordinate<uint8_t> rookStartingPos;
        math::Coordinate<uint8_t> rookEndingPos;
    };

    /**
     * This will get the coordinate of the rook that is being castled with. This
     * assumes all checks for castling validity have been done and the move is in
     * the process of being made (the king has already moved)
     *
     * @param move The move that the king has made.
     * @param state The current state of the board.
     * @return The coordinates of the rook that needs moved during a castle.
     */
    CastlingMetaData getCastlingRookData(const MoveState& move,
                                         const BoardState& state);
} // namespace check
