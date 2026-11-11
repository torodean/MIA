/**
 * @file BoardState.cpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief Implements the BoardState class and methods.
 */

#include <algorithm>
#include <cstddef>
#include <sstream>
#include <string>

// Include the associated header file.
#include "BoardState.hpp"
#include "MIAException.hpp"
#include "Error.hpp"

// Used for en passant detection.
#include "Pawn.hpp" 
// Needed for castling special case.
#include "King.hpp"
// Used for building position keys.
#include "PieceType.hpp"

namespace check
{
    void BoardState::initialize(uint8_t columns, 
                                uint8_t rows,
                                const std::vector<PiecePosition>& startingPos,
                                const PlayerState& startPlayerState)
    {
        if (context)
        {
            std::stringstream stream;
            stream << "Initializing Board State with size " 
                   << std::to_string(columns) << " x " << std::to_string(rows) << "!";
           context->logger.log(stream.str(), {"BoardState", "initialize()"}, context->verboseMode);
        }

        xSize = columns;
        ySize = rows;
        startingPosition = startingPos;
        squareStates = createSquareStates(xSize, ySize);
        playerState = startPlayerState;
        setBoardToStartingPosition(startingPosition);
        startingPlayerState = startPlayerState;
        initialized = true;

        // The starting position is the first entry of the position history.
        positionHistory.clear();
        positionHistory.push_back(makeSnapshot());
    }


    void BoardState::setBoardToStartingPosition(const std::vector<PiecePosition>& startingPos)
    {
        uint16_t index;
        for (const PiecePosition& pos : startingPos)
        { // Loop over all piece in the starting position.
            index = coordToIndex(pos.coordinate);
            squareStates[index].updatePiece(pos.piece, true);
        }
        initialized = true;
    }


    bool BoardState::isSquareOccupied(math::Coordinate<uint8_t> pos) const
    {
        uint16_t index = coordToIndex(pos);
        /* Commented out for now since it's noisy.
        std::stringstream stream;
        if (context)
        {
            stream << "Checking if square is occupied: "
                   << "Square " << pos << ", index="
                   << std::to_string(index) << ": ";
        }
        */

        const SquareState& square = squareStates[index];
        /*
        if (context)
        {
            stream << (square.isOccupied() ? "true" : "false");
            context->logger.log(stream.str(), {"BoardState", "isSquareOccupied()"}, context->verboseMode);
        }
        */

        return square.isOccupied();
    }


    bool BoardState::containsOpponentPiece(math::Coordinate<uint8_t> pos, Color color) const
    {
        const auto& piece = getPieceOnSquare(pos);
        return piece.has_value() && piece->color != color;
    }


    const std::optional<Piece>& BoardState::getPieceOnSquare(math::Coordinate<uint8_t> pos) const
    {
        uint16_t index = coordToIndex(pos);
        const SquareState& square = squareStates[index];

        return square.getPiece();
    }


    uint8_t BoardState::whosTurnIsIt() const
    {
        return playerState.whosTurnIsIt;
    }


    void BoardState::advanceTurn(uint8_t skipAhead)
    {
        std::stringstream stream;
        if (context)
        {
            stream << "Advancing player turn from player " << std::to_string(playerState.whosTurnIsIt);
        }
        for (uint8_t i=0; i<skipAhead; i++)
        { // Call this skipAhead number of times.
            playerState.advanceTurn();
        }
        if (context)
        {
            stream << " to " << std::to_string(playerState.whosTurnIsIt);
            context->logger.log(stream.str(), {"BoardState", "advanceTurn()"}, context->verboseMode);
        }

        // The completed turn leaves a new position for the next player.
        positionHistory.push_back(makeSnapshot());
    }


    void BoardState::movePiece(math::Coordinate<uint8_t> fromPos,
                               math::Coordinate<uint8_t> toPos,
                               bool recordInHistory)
    {
        if (!isSquareOccupied(fromPos))
            MIA_THROW(error::ErrorCode::Check_No_Piece_On_Square);

        uint16_t fromIndex = coordToIndex(fromPos);
        uint16_t toIndex = coordToIndex(toPos);

        Piece piece = squareStates[fromIndex].getPiece().value();

        // Store the move in the move history.
        MoveState move;
        move.piece = piece;
        move.startingPos = fromPos;
        move.endingPos = toPos;

        if (containsOpponentPiece(toPos, piece.color))
        { // Store whether this was a capture move or not.
            move.wasCapture = true;
        }

        // Check for an en passant special-case.
        if (isEnPassantMove(move, *this))
        { // Handle removing the en-passant taken pawn.
            const std::optional<MoveState> lastMove = this->getLastMove();
            removePiece(lastMove->endingPos);
            move.wasEnPassant = true;
            move.wasCapture = true;
        }

        // Check for a pawn promotion special case.
        if (triggersPromotion(move, *this))
        {
            /*
             * Handles promoting a piece. This will set the move trigger, but 
             * the replacement is handled by the event interaction.
             */
            move.triggersPromo = true;
        }

        // Check for castling, this is a special case.
        if (isCastlingMove(move))
        {
            /*
             * Handles castling. This sets the castling trigger, and the rook movement
             * is done in the event handler so that the ui can be updated as well.
             */
            move.wasCastling = true;
        }

        // Add the move to the board moves and update board state.
        if (recordInHistory)
        {
            moves.push_back(move);
        }

        squareStates[toIndex].updatePiece(piece);
        squareStates[fromIndex].removePiece();
    }


    void BoardState::removePiece(math::Coordinate<uint8_t> pos)
    {
        uint16_t index = coordToIndex(pos);
        squareStates[index].removePiece();
    }


    void BoardState::placePiece(math::Coordinate<uint8_t> pos, Piece piece)
    {
        uint16_t index = coordToIndex(pos);
        squareStates[index].updatePiece(piece);
    }


    uint8_t BoardState::getXSize() const
    {
        return xSize;
    }


    uint8_t BoardState::getYSize() const
    {
        return ySize;
    }


    const std::vector<check::SquareState>& BoardState::getSquareStates() const
    {
        return squareStates;
    }


    bool BoardState::isSquareOnBoard(math::Coordinate<uint8_t> pos) const
    {
        return pos.x < xSize && pos.y < ySize;
    }


    const std::vector<PiecePosition>& BoardState::getStartingPosition() const
    {
        return startingPosition;
    }


    void BoardState::reset()
    {
        moves.clear();

        /*
         * The squares are rebuilt from scratch here rather than just re-placing the
         * starting pieces on top of the current board. setBoardToStartingPosition only writes
         * the starting pieces down, so without this rebuild every piece which moved
         * during the game would stay on the square it moved to once the game reset.
         */
        squareStates = createSquareStates(xSize, ySize);

        setBoardToStartingPosition(startingPosition);
        playerState.whosTurnIsIt = startingPlayerState.whosTurnIsIt; // Reset player.

        // The new game restarts the position history at the starting position.
        positionHistory.clear();
        positionHistory.push_back(makeSnapshot());
    }


    std::optional<MoveState> BoardState::getLastMove() const
    {
        if (moves.empty())
            return std::nullopt;
        return moves.back();
    }


    void BoardState::setLastMoveCastlingData(const CastlingMetaData& rookMove)
    {
        if (moves.empty())
        {
            MIA_THROW(error::ErrorCode::Out_Of_Order_Method_Calls,
                      "Castling data set before a move was recorded.");
        }

        moves.back().castlingData = rookMove;
    }


    const std::vector<MoveState>& BoardState::getMoves() const
    {
        return moves;
    }


    const std::vector<PositionSnapshot>& BoardState::getPositionHistory() const
    {
        return positionHistory;
    }


    std::vector<PiecePosition> BoardState::getBoardPosition() const
    {
        std::vector<PiecePosition> positions;

        for (const SquareState& square : squareStates)
        {
            const std::optional<Piece>& piece = square.getPiece();

            if (!piece.has_value())
                continue;

            PiecePosition pos;
            pos.piece = piece.value();
            pos.coordinate = square.getCoordinate();
            positions.push_back(pos);
        }

        return positions;
    }


    bool BoardState::canUndo() const
    {
        // The first entry is the starting position; there is nothing to undo
        // until a turn has completed and pushed a second entry.
        return positionHistory.size() > 1;
    }


    PositionSnapshot BoardState::makeSnapshot() const
    {
        PositionSnapshot snapshot;
        snapshot.key = positionKey(*this);
        snapshot.board = serializeBoard();
        return snapshot;
    }


    std::string BoardState::serializeBoard() const
    {
        std::string board;
        board.reserve(1 + 5 * squareStates.size());

        // A single byte names whose turn it is, followed by one 5-byte record
        // per occupied square: x, y, piece type, piece color, and the square's
        // moved state. Every value fits a byte, so the pieces follow directly
        // with no separators and the count is recovered from the length.
        board.push_back(static_cast<char>(whosTurnIsIt()));

        for (const SquareState& square : squareStates)
        {
            const std::optional<Piece>& piece = square.getPiece();

            if (!piece.has_value())
                continue;

            const math::Coordinate<uint8_t>& pos = square.getCoordinate();
            board.push_back(static_cast<char>(pos.x));
            board.push_back(static_cast<char>(pos.y));
            board.push_back(static_cast<char>(piece->type));
            board.push_back(static_cast<char>(piece->color));
            board.push_back(square.hasChanged() ? 1 : 0);
        }

        return board;
    }


    void BoardState::restoreFromSnapshot(const std::string& board)
    {
        if (board.empty())
        { // Nothing was recorded, so leave the board untouched.
            return;
        }

        playerState.whosTurnIsIt = static_cast<uint8_t>(board[0]);

        // Rebuild the squares, then replay the recorded pieces. updatePiece's
        // initialPiece argument restores each square's moved state (false keeps
        // it, true marks it moved), so castling is affected exactly as it was.
        squareStates = createSquareStates(xSize, ySize);

        const size_t pieceCount = (board.size() - 1) / 5;
        for (size_t i = 0; i < pieceCount; ++i)
        {
            const size_t offset = 1 + 5 * i;
            const math::Coordinate<uint8_t> pos{static_cast<uint8_t>(board[offset]),
                                                static_cast<uint8_t>(board[offset + 1])};

            Piece piece;
            piece.type = static_cast<PieceType>(static_cast<uint8_t>(board[offset + 2]));
            piece.color = static_cast<Color>(static_cast<uint8_t>(board[offset + 3]));

            squareStates[coordToIndex(pos)].updatePiece(
                piece, static_cast<uint8_t>(board[offset + 4]) == 0);
        }
    }


    void BoardState::undoMove()
    {
        if (!canUndo())
            return;

        // The current position is the last entry; dropping it exposes the one
        // the board should return to.
        positionHistory.pop_back();

        restoreFromSnapshot(positionHistory.back().board);

        // One turn was reverted, so drop the move that produced the position
        // which was just removed.
        moves.resize(positionHistory.size() - 1);
    }


    size_t BoardState::undoToPlayersTurn(uint8_t targetTurn)
    {
        size_t count = 0;

        /*
         * Undo at least one turn (when any are available), then keep going until
         * it is the target player's turn.
         */
        do
        {
            if (positionHistory.size() <= 1)
                break; // The starting position is the furthest back.

            undoMove();
            ++count;
        } while (whosTurnIsIt() != targetTurn);

        return count;
    }


    uint8_t BoardState::previousPlayerInRotation() const
    {
        const uint8_t players = playerState.numberOfPlayers;

        // The player that made the last recorded move.
        return ((playerState.whosTurnIsIt + players - 2) % players) + 1;
    }


    std::string positionKey(const BoardState& boardState)
    {
        std::string key;

        for (const SquareState& square : boardState.getSquareStates())
        {
            const std::optional<Piece>& piece = square.getPiece();

            if (!piece.has_value())
                continue;

            const math::Coordinate<uint8_t>& pos = square.getCoordinate();
            key.push_back(static_cast<char>(pos.x));
            key.push_back(static_cast<char>(pos.y));
            key.push_back(static_cast<char>(piece->type));
            key.push_back(static_cast<char>(piece->color));

            if (piece->type == PieceType::King || piece->type == PieceType::Rook)
            { // Their square's changed flag is the castling availability.
                key.push_back(square.hasChanged() ? 1 : 0);
            }
        }

        key.push_back(static_cast<char>(boardState.whosTurnIsIt()));

        const bool epAvailable = lastMoveAllowsEnPassant(
            boardState, whosTurnIsItToColor(boardState.whosTurnIsIt()));
        key.push_back(epAvailable ? 1 : 0);

        return key;
    }


    bool BoardState::isSquareAttacked(math::Coordinate<uint8_t> pos,
                                      Color defendingColor) const
    {
        std::stringstream stream;
        if (context)
        {
            stream << "Checking if the (" << std::to_string(pos.x) << ", " 
                   << std::to_string(pos.y) << ") square is attacked: ";
        }
        // Search all squares currently attacked for the square in question.
        const auto attackedSquares = getAttackedSquares(*this, defendingColor);
        const bool isAttacked = std::find(attackedSquares.begin(), attackedSquares.end(), pos)
                                != attackedSquares.end();

        if (context)
        {
            stream << (isAttacked ? "true" : "false");
            context->logger.log(stream.str(), {"BoardState", "isSquareAttacked()"}, context->verboseMode);
        }

        return isAttacked;
    }


    uint16_t BoardState::coordToIndex(math::Coordinate<uint8_t> pos) const
    {
        return static_cast<uint16_t>(pos.x + pos.y*xSize);
    }


    void BoardState::setRuntimeContext(const RuntimeContext& ctx)
    {
        context = &ctx;
    }


    const SquareState& BoardState::getSquareAt(math::Coordinate<uint8_t> pos) const
    {
        uint16_t index = coordToIndex(pos);
        return squareStates[index];
    }
} // namespace check
