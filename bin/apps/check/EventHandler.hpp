/**
 * @file EventHandler.hpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief This defines various helpers and methods for handling the events
 *        from the CHECK UI's.
 */
#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <vector>

#include "BoardState.hpp"
#include "CheckConfig.hpp"
#include "GameResult.hpp"
#include "MathTypes.hpp"
#include "PythonModule.hpp"
// Needed for piece art.
#include "Piece.hpp"
// Needed for the sound effects player.
#include "AudioPlayer.hpp"

namespace check
{
    /**
     * Creates a static audio player which plays the check game's sound
     * effects. The sounds are the tracks in a playlist so that a specific
     * sound can be played with AudioPlayer::play().
     *
     * TODO - The main application should build and configure the sound
     *        effects playlist instead of it being constructed here.
     *
     * @return The sound effects audio player.
     * @throws MIAException if the audio system fails to initialize.
     */
    audio::AudioPlayer& soundEffects();

    /**
     * This enum stores all of the possible events that can be returned from
     * the python UI.
     */
    enum class CheckUIEventType : uint8_t
    {
        Unknown,        ///< Represents an unknown event type.
        Stop,           ///< Represents the signal to stop the application.
        PickupPiece,    ///< Represents a piece was picked up.
        DropPiece,      ///< Represents a piece was dropped.
        ResetBoard,     ///< Represents the board/game should be reset.
        SaveBoard,      ///< Represents the board/game should be saved.
        LoadBoard,      ///< Represents the board/game should be loaded.
        EditMode,       ///< Represents the board edit mode should be toggled.
        UndoMove,       ///< Represents the last move should be undone.
        PiecePromotion, ///< Represents a piece is being promoted.
    };

    /**
     * Stores the information contained in a ui event.
     */
    struct CheckUIEvent
    {
        /// The type that this event represents.
        CheckUIEventType type;

        /// The coordinates sent along with this event.
        std::optional<math::Coordinate<uint8_t>> startPos{std::nullopt};
        std::optional<math::Coordinate<uint8_t>> endPos{std::nullopt};

        /// A single string argument such as a promo piece or a save/load path.
        std::optional<std::string> argument;
    };

    /**
     * Stores event state data. This is data that may be useful carried between
     * different events. For example, when determining legal moves to highlight
     * them, those legal moves can be re-used for dropping a piece rather than
     * recalculating them each time.
     */
    struct EventState
    {
        /// The legal moves of the piece picked up by the most recent pickup event.
        std::vector<math::Coordinate<uint8_t>> legalMoves;

        /*
         * Stores whether or not the game has ended. Once it has, the board is locked
         * and only a game reset or an application stop is acted on, since the UI holds
         * its win/draw message up until it is reset.
         */
        bool gameOver{false};
    };

    /**
     * Converts a string input to the equivalent CheckUIEvent object.
     * @param input The string to convert.
     * @return The equivalent CheckUIEvent.
     */
    CheckUIEventType stringToCheckUIEventType(const std::string& input);

    /**
     * Converts a CheckUIEvent input to the equivalent string.
     * @param event The CheckUIEvent to convert.
     * @return The equivalent std::string.
     */
    std::string checkUIEventTypeToString(CheckUIEventType event);

    /**
     * Each ui event returned from the ui starts with a string 'name'. This
     * method will convert that name from the string and return the event.
     * The event name and its arguments are separated by ';'. For example, an
     * input of "pickup_piece;3;1" will return PickupPiece and store 3 1 as
     * the startPos.
     *
     * @note The events from the python ui's are well-defined and this method
     *       assumes this will remain the case.
     * @throws MIAException for malformed events.
     */
    CheckUIEvent toCheckUIEvent(const std::string& input);

    /**
     * This will display the legal moves for a piece for when the "pickup_piece" 
     * event occured. This expects (and checks) that the event type is PickupPiece,
     * and will do nothing for other event types. This needs the board state to
     * determine the current state of the board.
     * @param module The python ui module to perform the highlighting. This method
     *        assumes that a PythonModule(col, row) method exists which highlights
     *        using the expected color.
     * @param event The event to process.
     * @param state The current state of the board.
     * @param display Whether or not to display the legal moves in the ui.
     * @return The state data of the legalMoves that were determined.
     */
    EventState processPickupPiece(PythonModule& module,
                                  const CheckUIEvent& event,
                                  const BoardState& state,
                                  bool display = false);

    /**
     * Process an event of a piece being dropped. This will handle moving a piece
     * appropriately. This assumes the eventState data came from a processPickupPiece
     * call. Once the move is made, this adds the move to the ui move history,
     * advances the turn in the board state and in the ui turn indicator, then
     * determines the result of the game and returns it. The caller should
     * pass that result to handleGameResult() so the board gets locked and
     * the result gets displayed when the game has ended.
     * @param module The python ui module to perform the ui move on. This method
     *        assumes that a movePiece(col, row, toCol, toRow) method exists which 
     *        moves a piece in the game ui.
     * @param event The event to process.
     * @param state The current state of the board.
     * @param eventState The state of the legal moves determined from processPickupPiece().
     * @param pieceArts The configured piece arts, used to build the glyph for
     *        the move history entry.
     * @return The result of the game after this move. This is ongoing when the move
     *         was rejected (an illegal drop) or when the game is still being played.
     */
    GameResult processMoveEvent(PythonModule& module,
                                const CheckUIEvent& event,
                                BoardState& state,
                                EventState& eventState,
                                const std::vector<PieceArt>& pieceArts);

    /**
     * Send the result of a finished game to the UI so that it can display it. This
     * expects that a showGameResult(result, reason, winner) method exists in the UI
     * module, where each input is a string: the result is "win" or "draw", the reason
     * is "checkmate", "stalemate", or "insufficient material", and the winner is a
     * piece color name ("none" for a draw).
     *
     * This does nothing when the loaded UI module does not define that method yet, so
     * a game can still be played against an older UI. Whether this gets reported
     * anywhere else is left to the caller.
     *
     * @param module The python ui module to notify.
     * @param result The result of the game which just ended.
     */
    void processGameOver(PythonModule& module, const GameResult& result);

    /**
     * Report the player whose turn it now is to the ui turn indicator. This
     * assumes that a setTurn(color) method exists in the ui module.
     *
     * @note Only two players (white and black) is currently supported.
     *
     * @param module The python ui module to notify.
     * @param state The current state of the board, whose turn is reported.
     */
    void processTurnChange(PythonModule& module, const BoardState& state);

    /**
     * Report a made move to the ui move history boxes. This assumes that an
     * addMoveToHistory(side, move) method exists in the ui module, where the
     * move is the display string built by moveStateToString().
     *
     * @note Only two players (white and black) is currently supported.
     *
     * @param module The python ui module to notify.
     * @param pieceArts The configured piece arts to look the glyph up in.
     * @param move The move state to display in the ui.
     */
    void processMoveToHistory(PythonModule& module,
                              const std::vector<PieceArt>& pieceArts,
                              const MoveState& move);

    /**
     * Mark the last move of the given color in the ui move history with a
     * marker (e.g., "#"), which is appended to the move's already recorded
     * history entry. This assumes that a markMoveInHistory(side, marker)
     * method exists in the ui module.
     *
     * @note Only two players (white and black) is currently supported.
     *
     * @param module The python ui module to notify.
     * @param color The color of the last player's move to mark.
     * @param marker The text to append to that move.
     */
    void addMoveMarker(PythonModule& module, Color color, const std::string& marker);

    /**
     * Handle the result of the game after a move was processed. This stores the game
     * over flag in the eventState so that no further board interaction is processed
     * until the game is reset, marks the checkmating move with "#" in the ui
     * move history, prints the outcome of a finished game to the console, and
     * hands the result to processGameOver() for the UI to display. This does
     * nothing besides storing the flag when the game is still being played.
     *
     * @param module The python ui module to notify of a game over.
     * @param result The result of the game after the last move.
     * @param eventState The event state to lock when the game is over.
     */
    void handleGameResult(PythonModule& module,
                          const GameResult& result,
                          EventState& eventState);

    /**
     * Process an event which resets the game. This returns the board to its
     * starting position, pushes that position to the UI, returns the ui turn
     * indicator to the starting player, clears the ui move history boxes, and
     * unlocks the board so that a new game can be played.
     *
     * @param module The python ui module to reset. This method assumes that the
     *        setBoardState(cells), clearHighlights() and clearMoveHistory()
     *        methods exist in the UI module.
     * @param state The current state of the board.
     * @param eventState The event state to unlock (its game over flag is cleared).
     */
    void processResetBoard(PythonModule& module,
                           BoardState& state,
                           EventState& eventState);

    /**
     * Process an undo event. This reverts the game to the point where the
     * previous player in the turn rotation (the player who just moved) is to
     * move again: undoToPlayersTurn(previousPlayerInRotation()) backs the game
     * up to that player's turn using the position history. The UI is brought
     * back to match (the undone move is removed from its history box, the board
     * and turn indicator are updated, any game over dialog is dismissed, and the
     * board is unlocked), and the result of the restored position is returned.
     *
     * @param module The python ui module to update. This method assumes that the
     *        setBoardState(cells) and setTurn(color) methods exist, and that the
     *        removeMoveFromHistory(side) and dismissGameResult() methods exist
     *        (they are checked before being called).
     * @param state The current state of the board, which is restored.
     * @param eventState The event state to unlock (its game over flag is cleared).
     * @return The result of the game after the undo (ongoing for a normal undo).
     */
    GameResult processUndoMove(PythonModule& module,
                               BoardState& state,
                               EventState& eventState);

    /**
     * This handles a piece_promo_choice event. This event is somewhat special in the
     * sense that it is forced once a type of piece reaches the last rank. When this 
     * happens, the processPickupPiece method moves the piece, and triggers the ui promo 
     * options. After the promo piece is selected in the ui, this event immediately follows.
     * The piece chosen is updated in the board state, the original piece is removed, 
     * the ui board is updated, the turn is advanced in the board state and the ui turn 
     * indicator, and then the new game state is determined and returned.
     *
     * @param module The python ui module to use. This method assumes that the
     *        setPiece(piece, color, x, y) method exists.
     * @param event The event to process.
     * @param state The current state of the board.
     * @throws MIAException if the event is malformed.
     */
    GameResult processPiecePromotion(PythonModule& module,
                                     const CheckUIEvent& event,
                                     BoardState& state);

    /**
     * This method will register all of the piece art values into the UI. All pieces
     * must be registered within the UI for it to know how to display a piece.
     *
     * @param module The python ui module to use. This method assumes that the
     *        registerPiece(piece, glyph) method exist in the UI module.
     * @param pieceArts A vector of piece art objects to load into the ui.
     */
    void registerPieceArtInUI(PythonModule& module,
                              const std::vector<PieceArt>& pieceArts);

    /**
     * This handles a save_board event. It writes the current board state to the
     * save file named by the event's argument. The save file first records the
     * config file the board was set up from, followed by every move of the game
     * in order, so the board can later be brought back to this exact state.
     *
     * @param state The current state of the board to save.
     * @param event The event to process; its argument holds the save file path.
     * @param configFile The config file the board was set up from, recorded as the
     *        first line of the save file so a later load can restore the settings.
     * @throws MIAException if the event has no save file path.
     */
    void processSaveEvent(BoardState& state,
                          const CheckUIEvent& event,
                          const std::string& configFile);

    /**
     * This handles a load_board event. It reads the save file named by the event's
     * argument, pulls out the config file that save was created with, and re-loads
     * that config into the outConfig. The caller is then responsible for
     * setting the board up for that config (setupBoard) and replaying the saved
     * moves (loadBoardState) to bring the board back to the saved state.
     *
     * @param event The event to process; its argument holds the save file path.
     * @param[out] outConfig The configuration the save file was created from.
     * @return true if the save file and its config were read successfully.
     */
    bool processLoadEvent(const CheckUIEvent& event, CheckConfig& outConfig);
} // namespace check
