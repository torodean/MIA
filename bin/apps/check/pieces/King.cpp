/**
 * @file King.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the logic for the king piece.
 */

#include <cstdint>
#include <cstdlib>

// Include the associated header file.
#include "King.hpp"
#include "BoardState.hpp"

namespace check
{
    namespace
    {
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
            CastleDirection direction)
        {
            std::vector<SquareState> squaresInQuestion;
            squaresInQuestion.push_back(state.getSquareAt(kingPos));

            // Determine the castling direction.
            const int8_t step = (direction == CastleDirection::LEFT) ? -1 : 1;

            for (uint8_t x = kingPos.x + step; state.isSquareOnBoard({x, kingPos.y}); x += step)
            {
                math::Coordinate<uint8_t> pos{x, kingPos.y};

                squaresInQuestion.push_back(state.getSquareAt(pos));

                std::optional<Piece> piece = state.getPieceOnSquare(pos);

                if (piece.has_value())
                { // Stop once we hit a piece.
                    if (piece->type == PieceType::Rook &&
                        piece->color == state.getPieceOnSquare(kingPos)->color)
                    {
                        math::Coordinate<uint8_t> nextPos{static_cast<uint8_t>(x + step), kingPos.y};

                        if (state.isSquareOnBoard(nextPos))
                        { // Special case where we hit a rook but there's more squares in that direction.
                            squaresInQuestion.push_back(state.getSquareAt(nextPos));
                        }

                        break;
                    }

                    break;
                }
            }

            return squaresInQuestion;
        }
    } // namespace

    std::vector<math::Coordinate<uint8_t>> KingLogic::getLegalMoves(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        // Define the eight possible moves for a king.
        const std::vector<math::Coordinate<int8_t>> steps =
        {
            { 0,  1},
            { 0, -1},
            { 1,  0},
            {-1,  0},
            { 1,  1},
            {-1,  1},
            { 1, -1},
            {-1, -1}
        };

        // A king cannot capture a friendly piece.
        std::vector<math::Coordinate<uint8_t>> legalMoves = 
            generateStepMoves(currentPos, steps, boardState, color);

        // Check for castling options.
        if (canKingCastle(boardState, currentPos, CastleDirection::LEFT))
        {
            legalMoves.push_back({static_cast<uint8_t>(currentPos.x-2), currentPos.y});
        }
        if (canKingCastle(boardState, currentPos, CastleDirection::RIGHT))
        {
            legalMoves.push_back({static_cast<uint8_t>(currentPos.x+2), currentPos.y});
        }

        return legalMoves;
    }


    std::vector<math::Coordinate<uint8_t>> KingLogic::getAttackedSquares(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        std::vector<math::Coordinate<uint8_t>> attackedSquares;

        // Define the eight squares a king attacks.
        const int8_t moves[8][2] =
        {
            { 0,  1},
            { 0, -1},
            { 1,  0},
            {-1,  0},
            { 1,  1},
            {-1,  1},
            { 1, -1},
            {-1, -1}
        };

        for (const auto& move : moves)
        {
            math::Coordinate<uint8_t> pos{static_cast<uint8_t>(currentPos.x + move[0]),
                                          static_cast<uint8_t>(currentPos.y + move[1])};

            if (boardState.isSquareOnBoard(pos))
                attackedSquares.push_back(pos);
        }

        return attackedSquares;
    }


    bool isKingInCheck(const BoardState& state, Color color)
    {
        bool kingInCheck = false;

        for (const SquareState& square : state.getSquareStates())
        { // Get the square with the king on it.
            const std::optional<Piece>& piece = square.getPiece();

            if (!piece.has_value() || piece->type != PieceType::King || piece->color != color)
            {
                continue;
            }

            const math::Coordinate<uint8_t>& coordinate = square.getCoordinate();

            kingInCheck = state.isSquareAttacked({coordinate.x, coordinate.y}, color);
            break;
        }

        // No king of the specified color was found.
        return kingInCheck;
    }


    bool canKingCastle(const BoardState& state,
                       const math::Coordinate<uint8_t>& kingPos,
                       CastleDirection direction)
    {
        std::optional<Piece> king = state.getPieceOnSquare(kingPos);

        if (!king.has_value())
        { // This square does not contain a king.
            return false;
        }

        // Create a list of all squares between the king and the rook (or the edge of the board).
        std::vector<SquareState> squaresInQuestion = 
            getSquaresInQuestion(state, kingPos, direction);

        /*
         * If the square next to the king is attacked, the king cannot castle. Since the king
         * will always move 2 squares when castling, this is the only square this check needs
         * performed on. The end-move check is already accounted for in legal moves since you
         * cannot move into check already.
         */
        if (squaresInQuestion.size() >= 2)
        { // Make sure this check is valid first.
            // Index of 1 will be the square next to the king since it iterates from the king.
            if (state.isSquareAttacked(squaresInQuestion[1].getCoordinate(), king->color))
                return false;
        }

        bool hasRook = false;
        uint8_t pieceCount = 0;

        for (const SquareState& square : squaresInQuestion)
        { // Check if any square contains a rook.
            std::optional<Piece> squarePiece = square.getPiece();
            if (squarePiece.has_value())
            {
                pieceCount++;
                if (squarePiece->type == PieceType::Rook)
                    hasRook = true;
            }
        }

        if (!hasRook)
        { // No rook means no castling.
            return false;
        }

        bool specialCase = false;
        if (pieceCount > 2)
            specialCase = true;

        /*
         * Handle a special case first. If the last piece is not a rook, it either means
         * that a rook appeared before the edge of the board, or that the rook is at the
         * edge of the board:
         * ♜□□□♚□□□ OR □♞♜□□♚□□ OR □♞♜♚□□□□ OR □♞♜□♚□□□
         * |-------|           |------|          |---|             |-----| 
         * (the 'squares' in question from getSquaresInQuestion() are marked)
         * In order to allow castling, the king must be able to move two pieces to the
         * left. So if a different piece is at the end, and the total squares in question
         * is less than 4, it's an automatic false. 
         *
         * In the case where the end piece of the squares in question has a piece, the squares
         * in question are just the pieces between it and the king. The ones that allow castling
         * are the ones of a certain length. The king castles by moving over two squares:
         * ♞♜□□♚ OR ♞♜♚ OR ♞♜□♚
         *     ^        ^          ^
         *    good     bad        bad
         * (As seen above, the squares in question must be 5 or more in this case).
         * 
         * If a rook does not appear in the next square over, then this is the classic case 
         * of a different piece being closest to the king: ♜□♞♚□□□□
         *                                                  |----|
         *
         * If the end piece has no value, it's either just the end of the board, or the rook
         * did not have another piece next to it:
         * □□□□♚□□□ OR □□♜□□♚□□ OR □□♜♚□□□□ OR □□♜□♚□□□
         * |-------|           |------|          |---|             |-----|
         * (All of these cases allow castling in some way)
         *
         * If the piece to the left of the rook is also a rook, the check for if a piece has 
         * moved needs care to only check against the closest rook: ♜♜□□♚ -> ♜□♚♜□
         */
        if (specialCase)
        {
            std::optional<Piece> endPiece = squaresInQuestion.back().getPiece();
            if (!endPiece.has_value())
            { // End piece has no value. Therefore don't care about it beyond this point.
                squaresInQuestion.pop_back();
            }
            else
            { // End piece has a piece.
                if (squaresInQuestion.size() < 5)
                { // No square for the king to castle to.
                    return false;
                }
                if (endPiece->type == PieceType::Rook)
                { // Don't care about the piece beyond this point.
                    squaresInQuestion.pop_back();
                }
            }
        }

        // After the special cases, just see if the rook and king have moved yet, if not, true.
        for (const SquareState& square : squaresInQuestion)
        { // Check if any square contains a rook.
            std::optional<Piece> squarePiece = square.getPiece();
            if (squarePiece.has_value() && square.hasChanged())
            {
                if (squarePiece->type == PieceType::Rook)
                { // rook has moved.
                    return false;
                }
                if (squarePiece->type == PieceType::King)
                { // king has moved.
                    return false;
                }
            }
        }

        return true;
    }


    bool isCastlingMove(const MoveState& move)
    {
        if (move.piece.type != PieceType::King)
        { // Castling is currently only allowed with a king.
            return false;
        }

        return std::abs(move.endingPos.x - move.startingPos.x) == 2;
    }


    CastlingMetaData getCastlingRookData(const MoveState& move,
                                         const BoardState& state)
    {
        CastlingMetaData data;

        /*
         * Assuming the king has moved 2 squares. In this case, the end position
         * minus the starting position will give either +2 or -2 (the sign is the
         * direction the king has moved).
         */
        int8_t direction = (move.endingPos.x - move.startingPos.x) / 2;

        // Rook always goes next to the king. 
        data.rookEndingPos.x = move.endingPos.x - direction;
        data.rookEndingPos.y = move.endingPos.y;

        // Assuming a rook exists since this is during a castle.
        bool foundRook = false;
        for (int16_t x = move.startingPos.x; 
             state.isSquareOnBoard({static_cast<uint8_t>(x), move.startingPos.y}) && !foundRook; 
             x += direction)
        { // This is looping over all squares until crossing the board boundary or finding a rook.
            SquareState square = state.getSquareAt({static_cast<uint8_t>(x), move.startingPos.y});
            std::optional<Piece> squarePiece = square.getPiece();
            if (squarePiece.has_value() && squarePiece->type == PieceType::Rook)
            { // This is our castling rook.
                data.rookStartingPos = square.getCoordinate();
                foundRook = true;
                break;
            }
        }

        return data;
    }
} // namespace check
