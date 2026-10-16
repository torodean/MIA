/**
 * @file SquareState.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief This defines storage for the current state of a a square.
 */
#pragma once

#include <cstdint>
#include <optional>

#include "MathTypes.hpp"

namespace check
{
    // Placeholder here until it's created.
    struct Piece
    {
        PieceType type{PieceType::None};
        Color color{Color::white};
    };

    /**
     * This class represents the state of a single square on the game board.
     */
    class SquareState
    {
    public:
        /**
         * @brief Primary constructor. This will construct the square state for
         *        a square at a specific coordinate.
         */
        SquareState(uint8_t xCoord, uint8_t yCoord);

        /**
         * Updates (or adds) a piece to this square. An existing piece will be
         * overwritten, and if no piece is currently here, a new piece will be
         * added.
         */
        void updatePiece(check::Piece cPiece);

        /**
         * Remove the piece from this square.
         */
        void removePiece();

        /**
         * Returns whether or not this square is occupied.
         */
        bool isOccupied() const;

        /**
         * Returns the piece on this square or std::nullopt when empty.
         */
        std::optional<check::Piece> getPiece() const;

        /**
         * Returns the coordinate of this square.
         */
        const math::Coordinate<uint8_t>& getCoordinate() const;

    private:

        /// The piece that is currently on this square.
        check::Piece piece;
        /// The coordinate of this square on the game board.
        math::Coordinate<uint8_t> coordinate;
    }; // class SquareState
} // namespace check
