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
    };
    
    /**
     * Stores the information contained in a ui event.
     */
    struct CheckUIEvent
    {
        CheckUIEventType type;
        std::optional<math::Coordinate<uint8_t>> startPos{std::nullopt};
        std::optional<math::Coordinate<uint8_t>> endPos{std::nullopt};
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
    std::string checkUIEventToString(CheckUIEventType event);

    /**
     * Each ui event returned from the ui starts with a string 'name'. This
     * method will convert that name from the string and return the event.
     * For example, an input of "pickup 3 1" will return PickupPiece and
     * store 3 1 as the startPos.
     *
     * @note The events from the python ui's are well-defined and this method
     *       assumes this will remain the case.
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
     */
    void displayLegalMoves(PythonModule& module,
                           const CheckUIEvent& event,
                           const BoardState& state);
} // namespace check
