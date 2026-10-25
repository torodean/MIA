/**
 * @file SquareState.cpp
 * @author Antonius Torode
 * @date 10/18/2026
 * @brief Implements the Square state class and methods.
 */

// Include the associated header file.
#include "SquareState.hpp"

namespace check
{

    SquareState::SquareState(uint8_t xCoord, uint8_t yCoord)
    {
        coordinate.x = xCoord;
        coordinate.y = yCoord;
    }


    void SquareState::updatePiece(Piece cPiece, bool initialPiece)
    {
        piece = cPiece;
        squareHasChanged = !initialPiece;
    }


    void SquareState::removePiece()
    {
        piece.reset();
        squareHasChanged = true;
    }


    bool SquareState::isOccupied() const
    {
        return piece.has_value() && piece->type != PieceType::None;
    }


    const std::optional<Piece>& SquareState::getPiece() const
    {
        return piece;
    }


    const math::Coordinate<uint8_t>& SquareState::getCoordinate() const
    {
        return coordinate;
    }


    bool SquareState::resetHasChanged()
    {
        squareHasChanged = false;
        return squareHasChanged;
    }


    bool SquareState::hasChanged() const
    {
        return squareHasChanged;
    }


    std::vector<SquareState> createSquareStates(uint8_t xSize, uint8_t ySize)
    {
        std::vector<SquareState> squareStates;
        squareStates.reserve(static_cast<uint16_t>(xSize) * ySize);

        for (uint16_t y = 0; y < ySize; ++y)
        {
            for (uint16_t x = 0; x < xSize; ++x)
            {
                SquareState state(static_cast<uint8_t>(x), static_cast<uint8_t>(y));
                squareStates.emplace_back(state);
            }
        }

        return squareStates;
    }
} // namespace check
