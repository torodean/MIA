/**
 * @file BoardState.cpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief Implements the BoardState class and methods.
 */

#include <iostream>
#include <sstream>

// Include the associated header file.
#include "BoardState.hpp"

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
            index = coordToIndex(pos.coordinate.x, pos.coordinate.y);
            squareStates[index].updatePiece(pos.piece);
        }
        initialized = true;
    }


    bool BoardState::isSquareOccupied(uint8_t xPos, uint8_t yPos) const
    {
        uint16_t index = coordToIndex(xPos, yPos);
        
        std::stringstream stream;
        if (context)
        {
            stream << "Checking if square is occupied: "
                   << "Square (" << std::to_string(xPos) << "," << std::to_string(yPos) << "), index="
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
    
    
    bool BoardState::containsOpponentPiece(uint8_t xPos, uint8_t yPos, Color color) const
    {
        const auto& piece = getPieceOnSquare(xPos, yPos);
        return piece.has_value() && piece->color != color;
    }


    const std::optional<Piece>& BoardState::getPieceOnSquare(uint8_t xPos, uint8_t yPos) const
    {
        uint16_t index = coordToIndex(xPos, yPos);
        const SquareState& square = squareStates[index];

        return square.getPiece();
    }


    uint8_t BoardState::whosTurnIsIt() const
    {
        return playerState.whosTurnIsIt;
    }


    void BoardState::advanceTurn(uint8_t skipAhead)
    {
        for (uint8_t i=0; i<skipAhead; i++)
        { // Call this skipAhead number of times.
            playerState.advanceTurn();
        }
    }


    void BoardState::movePiece(uint8_t fromXPos, uint8_t fromYPos,
                               uint8_t toXPos, uint8_t toYPos)
    {
    
    }


    void BoardState::removePiece(uint8_t xPos, uint8_t yPos)
    {
    
    }


    void BoardState::placePiece(uint8_t xPos, uint8_t yPos, Piece piece)
    {
    
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


    bool BoardState::isSquareOnBoard(uint8_t xPos, uint8_t yPos) const
    {
        return xPos < xSize && yPos < ySize;
    }


    void BoardState::reset()
    {
        moves.clear();
        setStartingPosition(startingPosition);
        playerState.whosTurnIsIt = 1; // Reset player.
    }
    

    std::optional<MoveState> BoardState::getLastMove() const
    {
        if (moves.empty())
            return std::nullopt;
        return moves.back();
    }


    bool BoardState::isSquareAttacked(uint8_t xPos, uint8_t yPos,
                                      Color defendingColor) const
    {
        bool isAttacked = false;
        std::stringstream stream;
        if (context)
        {
            stream << "Checking if the (" << std::to_string(xPos) << ", " 
                   << std::to_string(yPos) << ") square is attacked: ";
        }
        // Get all squares currently attacked.
        auto attackedSquares = getAttackedSquares(*this, defendingColor);

        for (const auto& coordinate : attackedSquares)
        { // Iterate over all attacked squares.
            if (coordinate.x == xPos && coordinate.y == yPos)
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


    bool BoardState::isKingInCheck(Color color) const
    {
        bool kingInCheck = false;
        std::stringstream stream;
        if (context)
        {
            stream << "Checking if the " << colorToString(color) << " king is in check: ";
        }
        for (const SquareState& square : squareStates)
        {
            const std::optional<Piece>& piece = square.getPiece();

            if (!piece.has_value() ||
                piece->type != PieceType::King ||
                piece->color != color)
            {
                continue;
            }

            const math::Coordinate<uint8_t>& coordinate = square.getCoordinate();

            kingInCheck = isSquareAttacked(coordinate.x, coordinate.y, color);
            break;
        }

        if (context)
        {
            stream << (kingInCheck ? "true" : "false");
            context->logger.log(stream.str(), {"BoardState", "isKingInCheck()"}, context->verboseMode);
        }

        // No king of the specified color was found.
        return false;
    }


    uint16_t BoardState::coordToIndex(uint8_t xPos, uint8_t yPos) const
    {
        return static_cast<uint16_t>(xPos + yPos*xSize);
    }


    void BoardState::setRuntimeContext(const RuntimeContext& ctx)
    {
        context = &ctx;
    }
} // namespace check
