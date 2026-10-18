/**
 * @file EventHandler.cpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief Implements the various event handling defined in the EventHandler API.
 */

// Include the associated header.
#include "EventHandler.hpp"

#include "StringUtils.hpp"

namespace check
{
    CheckUIEvent stringToCheckUIEvent(const std::string& input)
    {
        std::string event = string_utils::toLower(input);
        
        if (event == "pickup_piece") return CheckUIEvent::PickupPiece;
        if (event == "drop_piece")   return CheckUIEvent::DropPiece;
        if (event == "reset_board")  return CheckUIEvent::ResetBoard;
        if (event == "save_board")   return CheckUIEvent::SaveBoard;
        if (event == "stop")         return CheckUIEvent::Stop;
        
        return CheckUIEvent::Unknown;
    }


    std::string checkUIEventToString(CheckUIEvent event)
    {
        switch(event)
        {
            case CheckUIEvent::PickupPiece: return "pickup_piece";
            case CheckUIEvent::DropPiece:   return "drop_piece";
            case CheckUIEvent::ResetBoard:  return "reset_board";
            case CheckUIEvent::SaveBoard:   return "save_board";
            case CheckUIEvent::Stop:        return "stop";
            case CheckUIEvent::Unknown:
            default:                        return "unknown";
        }
    }


    CheckUIEvent getEventType(const std::string& input)
    {
        std::string eventName = string_utils::getBeforeChar(input, ' ');
        return stringToCheckUIEvent(eventName);
    }
} // namespace check
