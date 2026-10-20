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
#include "MoveState.hpp"
// Used to push the starting position back to the UI on a reset.
#include "CheckConfig.hpp"

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


    EventState processPickupPiece(PythonModule& module,
                                  const CheckUIEvent& event,
                                  const BoardState& state,
                                  bool display)
    {
        EventState uiState;
        if (event.type != CheckUIEventType::PickupPiece)
        { // Do nothing for non-pickup events.
            return uiState;
        }

        // A picked up square does not have to hold a piece, so check before using it.
        const std::optional<Piece>& piece = state.getPieceOnSquare(event.startPos->x,
                                                                  event.startPos->y);

        if (!piece.has_value())
        { // Nothing was picked up, so there are no legal moves to display or cache.
            return uiState;
        }

        PiecePosition piecePos;
        piecePos.piece = piece.value();
        piecePos.coordinate.x = event.startPos.value().x;
        piecePos.coordinate.y = event.startPos.value().y;

        std::vector<math::Coordinate<uint8_t>> legalMoves = getLegalMoves(piecePos, state);
        
        for (const math::Coordinate<uint8_t>& legalMove : legalMoves)
        {
            module.call("highlightSquare", legalMove.x, legalMove.y);
        }

        uiState.legalMoves = legalMoves;
        return uiState;
    }


    GameResult processMoveEvent(PythonModule& module,
                                const CheckUIEvent& event,
                                BoardState& state,
                                EventState& eventState)
    {
        GameResult ongoing;

        if (event.type != CheckUIEventType::DropPiece)
        { // Do nothing for non-drop events.
            return ongoing;
        }

        // Ensure the event contains both coordinates.
        if (!event.startPos.has_value() || !event.endPos.has_value())
        {
            return ongoing;
        }

        const math::Coordinate<uint8_t>& startPos = event.startPos.value();
        const math::Coordinate<uint8_t>& endPos = event.endPos.value();

        // Ensure the destination is a legal move.
        bool isLegalMove = false;
        for (const auto& legalMove : eventState.legalMoves)
        {
            if (legalMove.x == endPos.x && legalMove.y == endPos.y)
            {
                isLegalMove = true;
                break;
            }
        }

        if (!isLegalMove)
        {
            return ongoing;
        }

        try
        { // Attempt to move the piece.
            state.movePiece(startPos.x, startPos.y, endPos.x, endPos.y);
            const std::optional<MoveState> lastMove = state.getLastMove();
            if (lastMove.has_value() && lastMove->wasEnPassant)
            { // Handle en passant in ui.
                int8_t direction = lastMove->piece.color == Color::White ? 1 : -1;
                module.call("removePiece", 
                            static_cast<uint8_t>(lastMove->endingPos.x), 
                            static_cast<uint8_t>(lastMove->endingPos.y - direction));
            }
        }
        catch(const error::MIAException& e)
        { // Occurs if source square is not occupied.
            return ongoing;
        }

        // Move the piece in the Python UI.
        module.call("movePiece", startPos.x, startPos.y, endPos.x, endPos.y);

        // Advance to the next player's turn.
        state.advanceTurn();

        // Clear the cached legal moves after processing the move.
        eventState.legalMoves.clear();

        /*
         * The turn has been advanced, so this determines the result for the player who
         * has to respond to this move. This is the order the end of game conditions are
         * written for, as both checkmate and stalemate apply to the player on move.
         */
        GameResult result = getGameResult(state);
        eventState.gameOver = result.isGameOver();

        return result;
    }


    void processGameOver(PythonModule& module, const GameResult& result)
    {
        if (!module.hasMethod("showGameResult"))
        { // This result cannot be displayed by the loaded UI module.
            return;
        }

        module.call("showGameResult",
                    gameResultToString(result.type),
                    gameOverReasonToString(result.reason),
                    colorToString(result.winner));
    }


    void processResetBoard(PythonModule& module,
                           BoardState& state,
                           EventState& eventState)
    {
        state.reset();

        /*
         * Any highlight left over from the previous game is cleared, then the starting
         * position is pushed back as the whole board state. The UI's setBoardState
         * replaces every square, so this also removes the pieces which were captured
         * or moved during the game which just finished.
         */
        module.call("clearHighlights");
        module.call("setBoardState", positionsToStateStrings(state.getStartingPosition()));

        eventState.legalMoves.clear();
        eventState.gameOver = false;
    }
} // namespace check
