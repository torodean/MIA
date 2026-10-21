/**
 * @file GameResult.cpp
 * @author Antonius Torode
 * @date 10/20/2026
 * @brief Implements the game result detection for the check app.
 */

// Include the associated header file.
#include "GameResult.hpp"

#include <vector>

// Used to read the board and determine whose turn it is.
#include "BoardState.hpp"
#include "PlayerState.hpp"
#include "SquareState.hpp"
// Needed for determining game result.
#include "King.hpp"

namespace check
{
    namespace
    {
        /**
         * @brief Determines the color which delivered the current checkmate.
         *
         * This is the color of the piece from the last recorded move, which is
         * the piece that delivered the mate. When no move has been recorded
         * (which only happens for a position which was set up directly rather
         * than played to), this falls back to the color opposing the input.
         *
         * @param boardState The current state of the board.
         * @param loser The color which was checkmated.
         * @return The color of the winning player.
         */
        Color winnerOfCheckmate(const BoardState& boardState, Color loser)
        {
            const std::optional<MoveState> lastMove = boardState.getLastMove();

            if (lastMove.has_value() && lastMove->piece.color != loser)
            {
                return lastMove->piece.color;
            }

            // Fall back to the opposing color for a position which was not played to.
            if (loser == Color::White)
                return Color::Black;

            return Color::White;
        }
    }


    bool GameResult::isGameOver() const
    {
        return type != GameResultType::Ongoing;
    }


    bool hasAnyLegalMove(const BoardState& boardState, Color color)
    {
        for (const SquareState& square : boardState.getSquareStates())
        {
            if (!square.isOccupied())
            { // Skip every square which does not currently hold a piece.
                continue;
            }

            const Piece& piece = square.getPiece().value();

            if (piece.color != color)
            { // Skip the pieces which are not this color.
                continue;
            }

            PiecePosition piecePos;
            piecePos.piece = piece;
            piecePos.coordinate = square.getCoordinate();

            if (!getLegalMoves(piecePos, boardState).empty())
            { // Stop at the first legal move found, as that is all this needs.
                return true;
            }
        }

        return false;
    }


    bool isCheckmate(const BoardState& boardState)
    {
        Color currentPlayer = whosTurnIsItToColor(boardState.whosTurnIsIt());

        return isKingInCheck(boardState, currentPlayer) &&
               !hasAnyLegalMove(boardState, currentPlayer);
    }


    bool isStalemate(const BoardState& boardState)
    {
        Color currentPlayer = whosTurnIsItToColor(boardState.whosTurnIsIt());

        return !isKingInCheck(boardState, currentPlayer) &&
               !hasAnyLegalMove(boardState, currentPlayer);
    }


    bool hasInsufficientMaterial(const BoardState& boardState)
    {
        uint8_t remainingMinorPieces = 0;

        for (const SquareState& square : boardState.getSquareStates())
        {
            if (!square.isOccupied())
            { // Skip every square which does not currently hold a piece.
                continue;
            }

            switch (square.getPiece().value().type)
            {
                case PieceType::King:
                    // A king cannot deliver a checkmate on its own, so it adds nothing.
                    break;

                case PieceType::Knight:
                case PieceType::Bishop:
                    /*
                     * A single remaining minor cannot mate on its own either, so these
                     * are counted here and only decided on once the whole board has
                     * been scanned.
                     */
                    remainingMinorPieces++;
                    break;

                /*
                 * Pawns, rooks, and queens can all still take part in a checkmate. Any
                 * piece type which gets added later lands here too, so a draw is never
                 * declared for a position until that type's mating ability has been
                 * considered.
                 */
                case PieceType::Pawn:
                case PieceType::Rook:
                case PieceType::Queen:
                case PieceType::None:
                case PieceType::Count:
                default:
                    return false;
            }
        }

        return remainingMinorPieces <= 1;
    }


    GameResult getGameResult(const BoardState& boardState)
    {
        GameResult result;
        Color currentPlayer = whosTurnIsItToColor(boardState.whosTurnIsIt());

        if (isCheckmate(boardState))
        {
            result.type = GameResultType::Win;
            result.reason = GameOverReason::Checkmate;
            result.winner = winnerOfCheckmate(boardState, currentPlayer);
            return result;
        }

        if (isStalemate(boardState))
        {
            result.type = GameResultType::Draw;
            result.reason = GameOverReason::Stalemate;
            return result;
        }

        if (hasInsufficientMaterial(boardState))
        {
            result.type = GameResultType::Draw;
            result.reason = GameOverReason::InsufficientMaterial;
            return result;
        }

        // No end of game condition holds, so the game is still ongoing.
        return result;
    }


    std::string gameResultToString(GameResultType type)
    {
        switch (type)
        {
            case GameResultType::Win:  return "win";
            case GameResultType::Draw: return "draw";

            case GameResultType::Ongoing:
            default:                   return "ongoing";
        }
    }


    std::string gameOverReasonToString(GameOverReason reason)
    {
        switch (reason)
        {
            case GameOverReason::Checkmate:            return "checkmate";
            case GameOverReason::Stalemate:            return "stalemate";
            case GameOverReason::InsufficientMaterial: return "insufficient material";

            case GameOverReason::None:
            default:                                   return "none";
        }
    }
} // namespace check
