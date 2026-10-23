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

#include "BoardState.hpp"
#include "GameResult.hpp"
#include "MathTypes.hpp"
#include "PythonModule.hpp"

namespace check
{
    /**
     * This enum stores all of the possible events that can be returned from
     * the python UI.
     */
    enum class CheckUIEventType : uint8_t
    {
        Unknown,      ///< Represents an unknown event type.
        Stop,         ///< Represents the signal to stop the application.
        PickupPiece,  ///< Represents a piece was picked up.
        DropPiece,    ///< Represents a piece was dropped.
        ResetBoard,   ///< Represents the board/game should be reset.
        SaveBoard,    ///< Represents the board/game should be saved.
        PawnPromotion,///< Represents a pawn is being promoted.
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

        /// A chosen piece string sent with this event (used for promotions and such).
        std::optional<std::string> chosenPromoPiece;
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
     * @param input The string to convert,
     * @return the equivalent CheckUIEvent. 
     */
    CheckUIEventType stringToCheckUIEventType(const std::string& input);

    /**
     * Converts a CheckUIEvent input to the equivalent string.
     * @param event The CheckUIEvent to convert,
     * @return the equivalent std::string. 
     */
    std::string CheckUIEventTypeToString(CheckUIEventType event);

    /**
     * Each ui event returned from the ui starts with a string 'name'. This
     * method will convert that name from the string and return the event.
     * For example, an input of "pickup 3 1" will return PickupPiece and
     * store 3 1 as the startPos.
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
     * call. Once the move is made, this determines the result of the game and stores
     * it in the eventState so that no further board interaction is processed.
     * @param module The python ui module to perform the ui move on. This method
     *        assumes that a movePiece(col, row, toCol, toRow) method exists which 
     *        moves a piece in the game ui.
     * @param event The event to process.
     * @param state The current state of the board.
     * @param eventState The state of the legal moves determined from processPickupPiece().
     * @return The result of the game after this move. This is ongoing when the move
     *         was rejected (an illegal drop) or when the game is still being played.
     */
    GameResult processMoveEvent(PythonModule& module,
                                const CheckUIEvent& event,
                                BoardState& state,
                                EventState& eventState);

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
     * Process an event which resets the game. This returns the board to its starting
     * position, pushes that position to the UI, and unlocks the board so that a new
     * game can be played.
     * @param module The python ui module to reset. This method assumes that the
     *        setBoardState(cells) and clearHighlights() methods exist in the UI module.
     * @param state The current state of the board.
     * @param eventState The event state to unlock (its game over flag is cleared).
     */
    void processResetBoard(PythonModule& module,
                           BoardState& state,
                           EventState& eventState);

    /**
     * This handles a pawn_promo_choice event. This event is somewhat special in the
     * sense that it is forced once a pawn reaches the last rank. When this happens,
     * the processPickupPiece method moves the pawn, and triggers the ui promo options.
     * After the promo piece is selected in the ui, this event immediately follows.
     * The piece chosen is updated in the board state, the pawn is removed, the ui 
     * board is updated, and then the new game state is determined and returned.
     *
     * @param module The python ui module to reset. This method assumes that the
     *        setBoardState(cells) and clearHighlights() methods exist in the UI module.
     * @param event The event to process.
     * @param state The current state of the board.
     * @throws MIAException if the event is malformed.
     */
    GameResult processPawnPromotion(PythonModule& module,
                                    const CheckUIEvent& event,
                                    BoardState& state);
} // namespace check
