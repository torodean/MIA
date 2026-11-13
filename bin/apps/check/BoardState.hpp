/**
 * @file BoardState.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief This defines storage for the current state of a game (the board 
 *        state, where all pieces are, who's turn it is, times, etc).
 *
 * For the check system, uint8_t values are used for the various coordinates.
 * Since this is a version of chess, 255 is plenty of storage for either 
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
     * One entry of the BoardState position history: a complete, restorable
     * record of the position. Stored at the starting position and after every
     * completed turn, so it doubles as the game's undo history.
     */
    struct PositionSnapshot
    {
        /// The positionKey() of this position.
        std::string key;
        /// A compact, reversible serialization of this position's board.
        std::string board;
    };

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
         * initialize the object, this should be used when the default constructor
         * is used.
         * @param columns The number of columns of the board.
         * @param rows The number of rows of the board.
         * @param startingPos The starting position to initialize this board state to.
         * @param startPlayerState The starting player state (i.e., # of players and whos starts).
         * @throws MIAException if the starting position is invalid.
         */
        void initialize(uint8_t columns, 
                        uint8_t rows,
                        const std::vector<PiecePosition>& startingPos,
                        const PlayerState& startPlayerState);

        /**
         * Sets the starting position of the board.
         * @param startingPos The starting position to initialize this board state to.
         * @throws MIAException if the starting position is invalid.
         */
        void setBoardToStartingPosition(const std::vector<PiecePosition>& startingPos);

        /**
         * Determines whether a square is occupied or not.
         *
         * @param xPos The x-coordinate (column) of the input square.
         * @param yPos The y-coordinate (row) of the input square.
         * @return true if there is a piece on the square, false otherwise.
         */
        bool isSquareOccupied(math::Coordinate<uint8_t> pos) const;

        /**
         * Returns the piece on the requested square.
         *
         * @param xPos The x-coordinate (column) of the input square.
         * @param yPos The y-coordinate (row) of the input square.
         * @return The piece on the square or std::nullopt if the square
         *         is not occupied.
         */
        const std::optional<Piece>& getPieceOnSquare(math::Coordinate<uint8_t> pos) const;

        /**
         * @brief Determines whether a square contains a piece of a different color.
         * @param xPos The x-coordinate of the square.
         * @param yPos The y-coordinate of the square.
         * @param color The color to compare against.
         * @return true if the square contains a piece of a different color, false otherwise.
         */
        bool containsOpponentPiece(math::Coordinate<uint8_t> pos, Color color) const;

        /**
         * Returns whomever's turn it is in the current board state.
         */
        uint8_t whosTurnIsIt() const;

        /**
         * Advance the turn to the next player.
         *
         * Advancing the turn ends the current one, so the position the next
         * player faces is recorded in the position history (see positionKey()).
         * This is the only point a completed turn is recorded, and the event
         * flows call it after every board update of a turn (the castling rook
         * move and any promotion replacement included).
         *
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
         * @param fromPos The coordinate of the source square.
         * @param toPos The coordinate of the destination square.
         * @param recordInHistory When false, the move is left out of the moves
         *        history. Used for component moves, such as the rook move of a
         *        castle, so the history holds one entry per turn.
         * @throws MIAException if the source square is not occupied.
         */
        void movePiece(math::Coordinate<uint8_t> fromPos,
                       math::Coordinate<uint8_t> toPos,
                       bool recordInHistory = true);

        /**
         * @brief Stores the castling rook's move on the last recorded move.
         *
         * The rook move of a castle is not recorded as its own history entry,
         * so its coordinates are attached to the king's entry. This keeps the
         * castle reversible, as the rook's original square cannot be derived
         * once the rook has moved.
         *
         * @param rookMove The rook's starting and ending squares of the castle.
         * @throws MIAException if no move has been recorded yet.
         */
        void setLastMoveCastlingData(const CastlingMetaData& rookMove);

        /**
         * @brief Removes the piece from the requested square, if one exists.
         *
         * Does nothing if the square is already empty.
         *
         * @param xPos The x-coordinate (column) of the square.
         * @param yPos The y-coordinate (row) of the square.
         */
        void removePiece(math::Coordinate<uint8_t> pos);

        /**
         * @brief Places a piece on the requested square.
         *
         * Any piece currently on the square is overwritten.
         *
         * @param xPos The x-coordinate (column) of the square.
         * @param yPos The y-coordinate (row) of the square.
         * @param piece The piece to place on the square.
         */
        void placePiece(math::Coordinate<uint8_t> pos, check::Piece piece);

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
        bool isSquareOnBoard(math::Coordinate<uint8_t> pos) const;

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
         * @brief Returns every move made in this game, in the order made.
         *
         * The history holds exactly one entry per completed turn.
         *
         * @return A reference to the vector of recorded moves.
         */
        const std::vector<MoveState>& getMoves() const;

        /**
         * @brief Returns the key of every position which has occurred so far.
         *
         * The first entry is the starting position and the last entry is the
         * current position of the board. reset() restarts the history.
         *
         * @return A reference to the vector of recorded position keys.
         */
        const std::vector<PositionSnapshot>& getPositionHistory() const;

        /**
         * @brief Returns the pieces currently on the board, one entry per
         *        occupied square.
         *
         * This is the live position, as opposed to getStartingPosition().
         *
         * @return A vector of the occupied squares and the piece on each.
         */
        std::vector<PiecePosition> getBoardPosition() const;

        /**
         * @brief Determines whether there is a move available to undo.
         *
         * @return true when at least one turn has been completed, false at the
         *         starting position.
         */
        bool canUndo() const;

        /**
         * @brief Undoes a single completed turn.
         *
         * The board, the turn, and the recorded move history are all restored
         * to their state before the most recent turn, using the position
         * history rather than reversing the move. Does nothing when there is
         * nothing to undo.
         */
        void undoMove();

        /**
         * @brief Undoes completed turns until it is the given player's turn.
         *
         * At least one turn is undone (when any are available), and undoing
         * continues until whosTurnIsIt() equals targetTurn.
         *
         * @param targetTurn The player index whose turn the undo stops at.
         * @return The number of turns undone (0 when there was nothing to undo).
         */
        size_t undoToPlayersTurn(uint8_t targetTurn);

        /**
         * @brief Returns the player index which moved most recently, that is,
         *        the player who comes just before the player to move in the turn
         *        rotation. This will skip computer players.
         *
         * @return The player index which made the last recorded move.
         */
        uint8_t previousPlayerInRotation() const;

        /**
         * Determines whether a square is attacked by an opposing piece.
         * @param xPos The x-coordinate (column) to check.
         * @param yPos The y-coordinate (row) to check.
         * @param defendingColor The 'friendly' color in terms of an attack.
         */
        bool isSquareAttacked(math::Coordinate<uint8_t> pos,
                              Color defendingColor) const;

        /**
         * Set the runtime context for this object to access.
         */
        void setRuntimeContext(const RuntimeContext& ctx);

        /**
         * Returns a reference to the square at the specified coordinate.
         * @param pos The location of the square to retrieve.
         * @return A reference to the square in question.
         */
        const SquareState& getSquareAt(math::Coordinate<uint8_t> pos) const;

        /**
         * This will load this object from a set of moves. This is used to create
         * this object from a save file.
         *
         * @note This will assume that this object has been either initialized or
         *       re-initialized with the board configuration that these moves expect.
         *
         * @param moves The move history from a save file.
         */
        void loadBoardFromMoveHistory(const std::vector<MoveState>& moves);

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
        uint16_t coordToIndex(math::Coordinate<uint8_t> pos) const;

        /**
         * @brief Takes a snapshot of the current position.
         *
         * Combines the position key (for the repetition rules) with a compact,
         * reversible board serialization into a single PositionSnapshot, which
         * is the unit stored in the position history and used for undo.
         *
         * @return A snapshot of this board's current position.
         */
        PositionSnapshot makeSnapshot() const;

        /**
         * @brief Builds the compact board serialization stored in a snapshot.
         *
         * Only the occupied squares are recorded: one 5-byte record each
         * (x, y, piece type, piece color, and the square's moved state), after
         * a single byte for whose turn it is. Empty squares take no space, so
         * the length depends on the number of pieces rather than the board size.
         *
         * @return The compact serialization of this board (see
         *         restoreFromSnapshot()).
         */
        std::string serializeBoard() const;

        /**
         * @brief Restores the board from a snapshot's compact board serialization.
         *
         * This is the inverse of serializeBoard(). It rebuilds the squares from
         * scratch, replays every recorded piece (restoring each square's moved
         * state so castling is affected exactly as it was), and sets whose turn
         * it is. The caller truncates the move history to match the restored
         * position.
         *
         * @param board The compact board serialization to restore from.
         */
        void restoreFromSnapshot(const std::string& board);

        /**
         * Stores the player state for this board configuration (e.g., whos turn it is).
         * This is dynamically changing, and the current board state depends on it. Thus,
         * it can only be set in initialize.
         */
        PlayerState playerState{};

        /// Stores the x size of the board (number of columns).
        uint8_t xSize{0};
        /// Stores the y size of the board (number of rows).
        uint8_t ySize{0};

        /*
         * This stores each SquareState object for the game board. Since the 
         * board has a maximum limit of uint8_t x uint8_t, this data is stored
         * in a vector rather than some sort of map. The vector storage allows
         * each square its own index which corresponds to the coordinate of the
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
        /// Stores the starting player state for the board (needed for reset()).
        PlayerState startingPlayerState;

        /**
         * Stores the moves that have been made up to this point in the game. This
         * tracks the moves that led to this game state.
         */
        std::vector<MoveState> moves;

        /*
         * Stores a snapshot of every position which has occurred in this game:
         * the starting position followed by one entry per turn completed by
         * advanceTurn(). Each entry holds both the position key (which the
         * repetition rules compare) and a compact, reversible board, so this
         * history doubles as the game's undo mechanism.
         */
        std::vector<PositionSnapshot> positionHistory;
    }; // class BoardState

    /**
     * @brief Builds the key which identifies the current position.
     *
     * The key holds the piece placement, the moved state of every king and
     * rook, whose turn it is, and whether an en passant capture is available.
     * Two positions share a key when they are identical for the rules which
     * compare positions, such as threefold repetition.
     *
     * @param boardState The current state of the board.
     * @return The position key string for the board.
     */
    std::string positionKey(const BoardState& boardState);
    
    /**
     * This will save the board state to a file. The file contains the content
     * of the positionHistory contained within the boardState, which can be
     * reloaded by loadBoardState() to return the board to the state it was
     * in when this method was called.
     *
     * The configFile will always be the first line of the save file, followed
     * by the list of moves. These moves are enough to reconstruct the current
     * state of the game by replaying all of the moves.
     *
     * @param boardState The current state of the board.
     * @param outputFile The file to output the save state to.
     * @param configFile The config file that was used to generate the board
     *        setup for the game that is being saved.
     */
    void saveBoardState(const BoardState& boardState,
                        const std::string& outputFile,
                        const std::string& configFile);
    
    /**
     * This will load the board state from a file. The file must contain the
     * content of the positionHistory contained within the boardState. It's
     * reloaded by this method to return the board to the state it was
     * in when the saveBoardState() method was called.
     *
     * @param[out] boardState The current state of the board.
     * @param inputFile The file to read the save state from.
     * @return true on success, false otherwise (e.g., the file does not exist).
     */
    bool loadBoardState(BoardState& boardState,
                        const std::string& inputFile);
} // namespace check
