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

#include "RuntimeContext.hpp"
#include "MathTypes.hpp"
#include "SquareState.hpp"
#include "PlayerState.hpp"
#include "MoveState.hpp"
#include "Piece.hpp"

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
         * The default constructor for this object. If this is used, the
         * initialize() method must be called.
         */
        BoardState() = default;

        /**
         * Main constructor for a BoardState object. This will construct a board
         * state of an appropriate size with a SquareState for each square.
         * @param columns The number of columns of the board.
         * @param rows The number of rows of the board.
         */
        BoardState(uint8_t columns, uint8_t rows);
        
        /**
         * initialize the object, this should be used when the default constructor
         * is used.
         * @param columns The number of columns of the board.
         * @param rows The number of rows of the board.
         * @param startingPos The starting position to initialize this board state to.
         * @throws MIAException if the starting position is invalid.
         */
        void initialize(uint8_t columns, 
                        uint8_t rows,
                        const std::vector<PiecePosition>& startingPos);

        /**
         * Sets the starting position of the board.
         * @param startingPos The starting position to initialize this board state to.
         * @throws MIAException if the starting position is invalid.
         */
        void setStartingPosition(const std::vector<PiecePosition>& startingPos);

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
        const std::optional<check::Piece>& getPieceOnSquare(uint8_t xPos, uint8_t yPos) const;
        
        /**
         * @brief Determines whether a square contains a piece of a different color.
         * @param xPos The x-coordinate of the square.
         * @param yPos The y-coordinate of the square.
         * @param color The color to compare against.
         * @return true if the square contains a piece of a different color, false otherwise.
         */
        bool containsOpponentPiece(uint8_t xPos, uint8_t yPos, Color color) const;

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
         * @brief Returns the starting position this board was built from.
         *
         * This is the position the board returns to when reset() is called. It is
         * needed to rebuild a UI (or write a save file) for that position.
         *
         * @return A reference to the piece positions making up the starting position.
         */
        const std::vector<check::PiecePosition>& getStartingPosition() const;

        /**
         * @brief Resets the board state to an empty game.
         *
         * Removes all pieces from every square and resets the turn state
         * to the first player's turn. The board dimensions are unchanged.
         */
        void reset();
        
        /**
         * This will return the last move that was made. This is needed for things
         * such as en passant (pawn moves) and other scenarios. If no moves have
         * been made, this will return std::nullopt.
         */
        std::optional<MoveState> getLastMove() const;
        
        /**
         * Determines whether a square is attacked by an opposing piece.
         * @param xPos The x-coordinate (column) to check.
         * @param yPos The y-coordinate (row) to check.
         * @param defendingColor The 'friendly' color in terms of an attack.
         */
        bool isSquareAttacked(uint8_t xPos, uint8_t yPos,
                              Color defendingColor) const;

        /**
         * Set the runtime context for this object to access.
         */
        void setRuntimeContext(const RuntimeContext& ctx);

    private:
    
        /// Stores runtimeContext such as the logger and verboseMode for this object to use. 
        const RuntimeContext* context{nullptr};
    
        /// Stores whether or not this object was initialized.
        bool initialized{false};

        /**
         * Calculates the index value of a square based on the coordinates.
         * The index is given by index(x,y) = x + y*xSize (see squareStates),
         * which has a maximum value of 255+255^2 = 65280. Therefore, 2-byte
         * storage which holds a maximum value of 65535 is sufficient.
         *
         * @param xPos The x-coordinate (column) of the input square.
         * @param yPos The y-coordinate (row) of the input square.
         */
        uint16_t coordToIndex(uint8_t xPos, uint8_t yPos) const;

        /// Stores the player state for this board configuration (e.g., whos turn it is). 
        PlayerState playerState{};

        /// Stores the x size of the board (number of columns).
        uint8_t xSize{0};
        /// Stores the y size of the board (number of rows).
        uint8_t ySize{0};

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
        std::vector<SquareState> squareStates;
        
        /// Stores the starting position for the board (needed for reset()).
        std::vector<PiecePosition> startingPosition;

        /**
         * Stores the moves that have been made up to this point in the game. This
         * tracks the moves that led to this game state.
         */
        std::vector<MoveState> moves;
    }; // class BoardState
    
    
} // namespace check
