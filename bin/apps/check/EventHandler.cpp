/**
 * @file EventHandler.cpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief Implements the various event handling defined in the EventHandler API.
 */

// Include the associated header.
#include "EventHandler.hpp"

#include <vector>

#include "StringUtils.hpp"

namespace check
{
    CheckUIEventType stringToCheckUIEventType(const std::string& input)
    {
        std::string event = string_utils::toLower(input);
        
        if (event == "pickup_piece") return CheckUIEventType::PickupPiece;
        if (event == "drop_piece")   return CheckUIEventType::DropPiece;
        if (event == "reset_board")  return CheckUIEventType::ResetBoard;
        if (event == "save_board")   return CheckUIEventType::SaveBoard;
        if (event == "stop")         return CheckUIEventType::Stop;
        
        return CheckUIEventType::Unknown;
    }


    std::string CheckUIEventTypeToString(CheckUIEventType event)
    {
        switch(event)
        {
            case CheckUIEventType::PickupPiece: return "pickup_piece";
            case CheckUIEventType::DropPiece:   return "drop_piece";
            case CheckUIEventType::ResetBoard:  return "reset_board";
            case CheckUIEventType::SaveBoard:   return "save_board";
            case CheckUIEventType::Stop:        return "stop";
            case CheckUIEventType::Unknown:
            default:                        return "unknown";
        }
    }


    CheckUIEvent toCheckUIEvent(const std::string& input)
    {
        CheckUIEvent event;
        std::vector<std::string> eventTokens = string_utils::delimiterString(input, ' ');
        event.type = stringToCheckUIEventType(eventTokens[0]);
        
        // The rest of the parsing depends on the type of event.
        switch(event.type)
        {
            case CheckUIEventType::PickupPiece:
            {
                event.startPos = math::Coordinate<uint8_t>{
                    static_cast<uint8_t>(std::stoi(eventTokens[1])),
                    static_cast<uint8_t>(std::stoi(eventTokens[2])) };
                break;
            }
            case CheckUIEventType::DropPiece:
            {
                event.startPos = math::Coordinate<uint8_t>{
                    static_cast<uint8_t>(std::stoi(eventTokens[1])),
                    static_cast<uint8_t>(std::stoi(eventTokens[2])) };
                event.endPos = math::Coordinate<uint8_t>{
                    static_cast<uint8_t>(std::stoi(eventTokens[3])),
                    static_cast<uint8_t>(std::stoi(eventTokens[4])) };
            }

            // These events have nothing more to them.
            case CheckUIEventType::ResetBoard:
            case CheckUIEventType::SaveBoard:
            case CheckUIEventType::Stop:
            case CheckUIEventType::Unknown:
                break;
        }

        return event;
    }


    void displayLegalMoves(PythonModule& module,
                           const CheckUIEvent& event,
                           const BoardState& state)
    {
        if (event.type != CheckUIEventType::PickupPiece)
        { // Do nothing for non-pickup events.
            return;
        }

        // This could throw std::bad_optional_access but events are well-formed so shouldn't occur.
        const Piece& piece = state.getPieceOnSquare(event.startPos->x, event.startPos->y).value();

        PiecePosition piecePos;
        piecePos.piece = piece;
        piecePos.coordinate.x = event.startPos.value().x;
        piecePos.coordinate.y = event.startPos.value().y;

        std::vector<math::Coordinate<uint8_t>> legalMoves = getLegalMoves(piecePos, state);
        
        for (const math::Coordinate<uint8_t>& legalMove : legalMoves)
        {
            module.call("highlightSquare", legalMove.x, legalMove.y);
        }
    }
} // namespace check














