/**
 * @file BoardState.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief This defines storage for the current state of a game (the board 
 *        state, where all pieces are, who's turn it is, times, etc).
 *
 * For the check system, uint8_t values are used for the various coordinates.
 * Since this is a versin of chess, 255 is plenty of storage for either 
 * coordinate direction and values higher would not make much sense as any
 * intended game board for this system.
 */
#pragma once

#include <cstdint>
#include <vector>
#include <optional>

#include "MathTypes.hpp"
#include "SquareState.hpp"
#include "PlayerState.hpp"

namespace check
{
    /**
     * This class represents the state of the entire game board. This is
     * a collection of SquareState objects, with one representing each square
     * of the game board.
     */
    class BoardState
    {
    public:
        /**
         * Main constructor for a BoardState object. This will construct a board
         * state of an appropriate size with a SquareState for each square.
         * @param columns The number of columns of the board.
         * @param rows The number of rows of the board.
         */
        BoardState(uint8_t columns, uint8_t rows);

        /**
         * Determines whether a square is occupied or not.
         *
         * @param xPos The x-coordinate (column) of the input square.
         * @param yPos The y-coordinate (row) of the input square.
         * @return true if there is a piece on the square, false otherwise.
         */
        bool isSquareOccupied(uint8_t xPos, uint8_t yPos) const;

        /**
         * Returns the piece on the requested square.
         *
         * @param xPos The x-coordinate (column) of the input square.
         * @param yPos The y-coordinate (row) of the input square.
         * @return The piece on the square or std::nullopt if the square
         *         is not occupied.
         */
        std::optional<check::Piece> getPieceOnSquare(uint8_t xPos, uint8_t yPos) const;

        /**
         * Returns whomever's turn it is in the current board state.
         */
        uint8_t whosTurnIsIt() const;

        /**
         * Advance the turn to the next player.
         * @param skipahead An optional value to skip-ahead. This is added to support
         *        game variations with player/turn skips.
         */
        void advanceTurn(uint8_t skipAhead = 1);

        /**
         * @brief Moves the piece on one square to another square.
         *
         * This is a pure state operation; move legality is handled elsewhere.
         * The piece is removed from the source square and placed on the
         * destination square, overwriting any piece currently there. The
         * source square is left empty.
         *
         * @param fromXPos The x-coordinate (column) of the source square.
         * @param fromYPos The y-coordinate (row) of the source square.
         * @param toXPos The x-coordinate (column) of the destination square.
         * @param toYPos The y-coordinate (row) of the destination square.
         * @throws MIAException if the source square is not occupied.
         */
        void movePiece(uint8_t fromXPos, uint8_t fromYPos,
                       uint8_t toXPos, uint8_t toYPos);

        /**
         * @brief Removes the piece from the requested square, if one exists.
         *
         * Does nothing if the square is already empty.
         *
         * @param xPos The x-coordinate (column) of the square.
         * @param yPos The y-coordinate (row) of the square.
         */
        void removePiece(uint8_t xPos, uint8_t yPos);

        /**
         * @brief Places a piece on the requested square.
         *
         * Any piece currently on the square is overwritten.
         *
         * @param xPos The x-coordinate (column) of the square.
         * @param yPos The y-coordinate (row) of the square.
         * @param piece The piece to place on the square.
         */
        void placePiece(uint8_t xPos, uint8_t yPos, check::Piece piece);

        /**
         * @brief Returns the number of columns of the board.
         *
         * @return The x size (number of columns) of the board.
         */
        uint8_t getXSize() const;

        /**
         * @brief Returns the number of rows of the board.
         *
         * @return The y size (number of rows) of the board.
         */
        uint8_t getYSize() const;

        /**
         * @brief Returns the collection of square states of the board.
         *
         * @return A reference to the vector of square states, indexed by
         *         index(x,y) = x + y*xSize.
         */
        const std::vector<check::SquareState>& getSquareStates() const;

        /**
         * @brief Determines whether a coordinate lies on the board.
         *
         * @param xPos The x-coordinate (column) to check.
         * @param yPos The y-coordinate (row) to check.
         * @return true if the coordinate is within the board bounds,
         *         false otherwise.
         */
        bool isSquareOnBoard(uint8_t xPos, uint8_t yPos) const;

        /**
         * @brief Resets the board state to an empty game.
         *
         * Removes all pieces from every square and resets the turn state
         * to the first player's turn. The board dimensions are unchanged.
         */
        void reset();

    private:
    
        /**
         * Calculates the index value of a square based on the coordinates.
         * The index is given by index(x,y) = x + y*xSize (see squareStates),
         * which has a maximum value of 255+255^2 = 65280. Therefore, 2-byte
         * storage which holds a maximum value of 65535 is sufficient.
         *
         * @param xPos The x-coordinate (column) of the input square.
         * @param yPos The y-coordinate (row) of the input square.
         */
        uint16_t coordToIndex(uint8_t xPos, uint8_t yPos);

        /// Stores the player state for this board configuration (e.g., whos turn it is). 
        PlayerState playerState;

        /// Stores the x size of the board (number of columns).
        uint8_t xSize;
        /// Stores the y size of the board (number of rows).
        uint8_t ySize;

        /*
         * This stores each SquareState object for the game board. Since the 
         * board has a maximum limit of uint8_t x uint8_t, this data is stored
         * in a vector rather than some sort of map. The vector storage allowes
         * sach square it's own index which corresponds to the coordinate of the
         * square. The bottom left square represents the coordinate (0,0), which is
         * the start of indexing. The indexes increase as the x coordinate increases,
         * and then once the max x coordinate is reached, the next index represents
         * the first column of the next row (0,1).
         *
         * This gives the formula: index(x,y) = x + y*xSize
         */
        std::vector<check::SquareState> squareStates; 
    }; // class BoardState
} // namespace check
