/**
 * @file BoardState.cpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief Implements the BoardState class and methods.
 */

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
        xSize = columns;
        ySize = rows;
        startingPosition = startingPos;
        setStartingPosition(startingPosition);
        squareStates = createSquareStates(xSize, ySize);
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
        // TODO
        return true;
    }


    std::optional<check::Piece> BoardState::getPieceOnSquare(uint8_t xPos, uint8_t yPos) const
    {
        // TODO
        return std::nullopt;
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


    void BoardState::placePiece(uint8_t xPos, uint8_t yPos, check::Piece piece)
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
    

    MoveState BoardState::getLastMove()
    {
        return moves.back();
    }


    uint16_t BoardState::coordToIndex(uint8_t xPos, uint8_t yPos)
    {
        return static_cast<uint16_t>(xPos + yPos*xSize);
    }
} // namespace check
