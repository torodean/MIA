/**
 * @file SquareState.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief This defines storage for the current state of a a square.
 */
#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "MathTypes.hpp"
#include "Piece.hpp"

namespace check
{
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
         * @param cPiece The piece to update on this square.
         * @param initialPiece whether or not this is the initial piece being added.
         *        This determines whether to update squareHasChanged or not.
         */
        void updatePiece(Piece cPiece, bool initialPiece = false);

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
        const std::optional<Piece>& getPiece() const;

        /**
         * Returns the coordinate of this square.
         */
        const math::Coordinate<uint8_t>& getCoordinate() const;

        /**
         * Getter for the squareHasChanged bool.
         */
        bool hasChanged() const;

    private:

        /**
         * Stores whether this square has changed (a piece moved) since reset. This
         * is changed to true whenever updatePiece or removePiece is called. It is
         * primarily stored to later determine rules such as if castling is allowed.
         */
        bool squareHasChanged{false};
        /// The piece that is currently on this square.
        std::optional<Piece> piece;
        /// The coordinate of this square on the game board.
        math::Coordinate<uint8_t> coordinate;
    }; // class SquareState

    /**
     * @brief Constructs a vector of SquareState objects for a board of the
     *        specified dimensions.
     *
     * Squares are constructed in row-major order, with the indexing formula
     * index(x,y) = x + y*xSize.
     *
     * @param xSize The number of columns of the board.
     * @param ySize The number of rows of the board.
     * @return A vector containing a SquareState for each coordinate on the board.
     */
    std::vector<SquareState> createSquareStates(uint8_t xSize, uint8_t ySize);
} // namespace check
