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

namespace check
{
    /**
     * This enum stores all of the possible events that can be returned from
     * the python UI.
     */
    enum class CheckUIEvent : uint8_t
    {
        Unknown,      ///< Represents an unknown event type.
        Stop,         ///< Represents the signal to stop the application.
        PickupPiece,  ///< Represents a piece was picked up.
        DropPiece,    ///< Represents a piece was dropped.
        ResetBoard,   ///< Represents the board/game should be reset.
        SaveBoard,    ///< Represents the board/game should be saved.
    };
    
    /**
     * Converts a string input to the equivalent CheckUIEvent object.
     * @param input The string to convert,
     * @return the equivalent CheckUIEvent. 
     */
    CheckUIEvent stringToCheckUIEvent(const std::string& input);

    /**
     * Converts a CheckUIEvent input to the equivalent string.
     * @param event The CheckUIEvent to convert,
     * @return the equivalent std::string. 
     */
    std::string checkUIEventToString(CheckUIEvent event);

    /**
     * Each ui event returned from the ui starts with a string 'name'. This
     * method will convert that name from the string and return the event type.
     * For example, an input of "pickup 3 1" will return PickupPiece.
     *
     * @note The events from the python ui's are well-defined and this method
     *       assumes this will remain the case.
     */
    CheckUIEvent getEventType(const std::string& input);
    
    /**
     *
     *
     */
    void showLegalMoves();
} // namespace check
