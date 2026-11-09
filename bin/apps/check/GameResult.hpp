/**
 * @file GameResult.hpp
 * @author Antonius Torode
 * @date 10/20/2026
 * @brief This defines the result of a game (a win, a draw, or still ongoing)
 *        and the methods which determine that result from a board state.
 */
#pragma once

#include <cstdint>
#include <string>

#include "Piece.hpp"

namespace check
{
    class BoardState;

    /**
     * This enum stores the possible outcomes of a game. This is what the UI
     * displays when a game ends.
     */
    enum class GameResultType : uint8_t
    {
        Ongoing,    ///< The game has not ended yet.
        Win,        ///< A player has won the game.
        Draw,       ///< The game has ended with no winner.
    };

    /**
     * This enum stores the reason a game ended. Every win or draw is produced
     * by exactly one of these reasons.
     */
    enum class GameOverReason : uint8_t
    {
        None,                  ///< The game has not ended.
        Checkmate,             ///< The current player is in check with no legal move.
        Stalemate,             ///< The current player is not in check but has no legal move.
        InsufficientMaterial,  ///< Neither player can force a checkmate.
        ThreefoldRepetition,   ///< The same position has occurred three times.
        FiftyMoveRule,         ///< No pawn-like move or capture for fifty moves.
    };

    /// Occurrences of a position which draw the game by threefold repetition.
    inline constexpr uint32_t THREEFOLD_REPETITION_LIMIT = 3;

    /// Quiet moves which draw by the fifty-move rule (50 per player).
    inline constexpr uint16_t FIFTY_MOVE_RULE_LIMIT = 100;

    /**
     * Stores the result of a game. This holds the outcome, the reason for that
     * outcome, and the winning player (Color::None when there is no winner).
     */
    struct GameResult
    {
        /// The outcome of the game.
        GameResultType type{GameResultType::Ongoing};
        /// The reason the game ended (GameOverReason::None while it is ongoing).
        GameOverReason reason{GameOverReason::None};
        /// The color which won, Color::None for a draw or an ongoing game.
        Color winner{Color::None};

        /**
         * @brief Determines whether this result ends the game.
         *
         * @return true if the game ended in a win or a draw, false when it is
         *         still ongoing.
         */
        bool isGameOver() const;
    };

    /**
     * @brief Determines whether the input color has at least one legal move.
     *
     * This checks every piece of this color on the board and stops at the first
     * legal move it finds. Since only the current player may move, this only
     * returns true for the color whose turn it currently is.
     *
     * @param boardState The current state of the board.
     * @param color The color to search for legal moves.
     * @return true if this color has at least one legal move, false otherwise.
     */
    bool hasAnyLegalMove(const BoardState& boardState, Color color);

    /**
     * @brief Determines whether the current player is checkmated.
     *
     * The current player is checkmated when their king is in check and they have
     * no legal move which gets the king out of it.
     *
     * @param boardState The current state of the board.
     * @return true if the current player is checkmated, false otherwise.
     */
    bool isCheckmate(const BoardState& boardState);

    /**
     * @brief Determines whether the current player is stalemated.
     *
     * The current player is stalemated when their king is not in check but they
     * have no legal move at all.
     *
     * @param boardState The current state of the board.
     * @return true if the current player is stalemated, false otherwise.
     */
    bool isStalemate(const BoardState& boardState);

    /**
     * @brief Determines whether neither player can force a checkmate.
     *
     * This treats a position as having insufficient material when no pawn, rook,
     * or queen is on the board and at most one knight or bishop remains in total,
     * which covers king-versus-king, king-and-knight-versus-king, and
     * king-and-bishop-versus-king. King-and-bishop-versus-king-and-bishop of the
     * same square color is not detected here, as the square colors of a
     * configurable board are not always the ones which make that a draw.
     *
     * @param boardState The current state of the board.
     * @return true if neither player holds enough material to mate, false otherwise.
     */
    bool hasInsufficientMaterial(const BoardState& boardState);

    /**
     * @brief Determines whether the current position has occurred three times.
     *
     * Positions are compared by positionKey(), and the current occurrence
     * counts toward the limit. FIDE play requires a player to claim this draw;
     * here it ends the game automatically like stalemate does.
     *
     * @param boardState The current state of the board.
     * @return true if the current position reached THREEFOLD_REPETITION_LIMIT
     *         occurrences, false otherwise.
     */
    bool isThreefoldRepetition(const BoardState& boardState);

    /**
     * @brief Determines whether the fifty-move rule draws the game.
     *
     * Counts moves back to the last pawn-like move (pawn or spade) or capture
     * and compares against FIFTY_MOVE_RULE_LIMIT. This ends the game
     * automatically like stalemate does.
     *
     * @param boardState The current state of the board.
     * @return true if FIFTY_MOVE_RULE_LIMIT quiet moves have been played,
     *         false otherwise.
     */
    bool isFiftyMoveRule(const BoardState& boardState);

    /**
     * @brief Determines the current result of the game.
     *
     * The end-of-game conditions are checked in this order: checkmate, stalemate,
     * threefold repetition, fifty-move rule, then insufficient material. The
     * first one which applies becomes the result.
     * Since both checkmate and stalemate apply to the player who is to move, this
     * must be called after the turn has been advanced to the next player.
     *
     * @note A color with no king on the board is never reported as being in check
     *       (see BoardState::isKingInCheck), so such a color with no legal moves
     *       is reported as stalemated rather than checkmated.
     *
     * @param boardState The current state of the board.
     * @return The result of the game, which is ongoing when no end condition holds.
     */
    GameResult getGameResult(const BoardState& boardState);

    /**
     * @brief Converts a game result type into its string representation.
     *
     * @note This produces all-lowercase output.
     *
     * @param type The game result type to convert.
     * @return The string representation of the type ("ongoing", "win", or "draw").
     */
    std::string gameResultToString(GameResultType type);

    /**
     * @brief Converts a game over reason into its string representation.
     *
     * @note This produces all-lowercase output.
     *
     * @param reason The game over reason to convert.
     * @return The string representation of the reason ("none", "checkmate",
     *         "stalemate", "insufficient material", "threefold repetition",
     *         or "fifty-move rule").
     */
    std::string gameOverReasonToString(GameOverReason reason);
} // namespace check
