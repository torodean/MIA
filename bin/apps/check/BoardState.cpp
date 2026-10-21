/**
 * @file BoardState.cpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief Implements the BoardState class and methods.
 */

#include <sstream>

// Include the associated header file.
#include "BoardState.hpp"
#include "MIAException.hpp"
#include "Error.hpp"

// Used for en passant detection.
#include "Pawn.hpp" 

namespace check
{
    BoardState::BoardState(uint8_t columns, uint8_t rows) :
        xSize(columns), ySize(rows), squareStates(createSquareStates(columns, rows))
    { }


    void BoardState::initialize(uint8_t columns, 
                                uint8_t rows,
                                const std::vector<PiecePosition>& startingPos)
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
        setStartingPosition(startingPosition);
        initialized = true;
    }


    void BoardState::setStartingPosition(const std::vector<PiecePosition>& startingPos)
    {
        uint16_t index;
        for (const PiecePosition& pos : startingPos)
        { // Loop over all piece in the starting position.
            index = coordToIndex(pos.coordinate);
            squareStates[index].updatePiece(pos.piece);
        }
        initialized = true;
    }


    bool BoardState::isSquareOccupied(math::Coordinate<uint8_t> pos) const
    {
        uint16_t index = coordToIndex(pos);

        std::stringstream stream;
        if (context)
        {
            stream << "Checking if square is occupied: "
                   << "Square " << pos << ", index="
                   << std::to_string(index) << ": ";
        }

        const SquareState& square = squareStates[index];

        if (context)
        {
            stream << (square.isOccupied() ? "true" : "false");
            context->logger.log(stream.str(), {"BoardState", "isSquareOccupied()"}, context->verboseMode);
        }

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
    }


    void BoardState::movePiece(math::Coordinate<uint8_t> fromPos,
                               math::Coordinate<uint8_t> toPos)
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

        // Check for an en passant special-case.
        if (isEnPassantMove(move, *this))
        { // Handle removing the en-passant taken pawn.
            const std::optional<MoveState> lastMove = this->getLastMove();
            removePiece(lastMove->endingPos);
            move.wasEnPassant = true;
        }

        // Check for a pawn promotion special case.
        if (tiggersPromotion(move, *this))
        {
            /*
             * Handles promoting a pawn. This will set the move trigger, but 
             * the replacement is handled by the event interaction.
             */
            move.triggersPawnPromo = true;
        }

        // Add the move to the board moves and update board state.
        moves.push_back(move);
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
         * starting pieces on top of the current board. setStartingPosition only writes
         * the starting pieces down, so without this rebuild every piece which moved
         * during the game would stay on the square it moved to once the game reset.
         */
        squareStates = createSquareStates(xSize, ySize);

        setStartingPosition(startingPosition);
        playerState.whosTurnIsIt = 1; // Reset player.
    }


    std::optional<MoveState> BoardState::getLastMove() const
    {
        if (moves.empty())
            return std::nullopt;
        return moves.back();
    }


    bool BoardState::isSquareAttacked(math::Coordinate<uint8_t> pos,
                                      Color defendingColor) const
    {
        bool isAttacked = false;
        std::stringstream stream;
        if (context)
        {
            stream << "Checking if the (" << std::to_string(pos.x) << ", " 
                   << std::to_string(pos.y) << ") square is attacked: ";
        }
        // Get all squares currently attacked.
        auto attackedSquares = getAttackedSquares(*this, defendingColor);

        for (const auto& coordinate : attackedSquares)
        { // Iterate over all attacked squares.
            if (coordinate == pos)
            {
                isAttacked = true;
                break;
            }
        }

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
} // namespace check
