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
// Used for pausing between ui events.
#include "Timing.hpp"
// Needed for castling special case.
#include "King.hpp"

namespace check
{
    namespace
    {
        /**
         * @brief Converts a single event token into a board coordinate.
         *
         * The coordinate tokens are read off of the event string the UI sends in,
         * so a token which is not a valid coordinate is reported as a malformed
         * event instead of the standard library exception the numeric conversion
         * would otherwise throw.
         *
         * @param token The event token to convert.
         * @return The coordinate the token holds.
         * @throws MIAException if the token does not hold a valid coordinate.
         */
        uint8_t eventTokenToCoordinate(const std::string& token)
        {
            try
            { // stringToUint8 throws for non-numerical or out of range tokens.
                return string_utils::stringToUint8(token);
            }
            catch (const error::MIAException& ex)
            { // Report this as a malformed event rather than a bad parameter error.
                MIA_THROW(error::ErrorCode::Check_Malformed_Event, std::string(ex.what()));
            }
        }
    } // namespace


    CheckUIEventType stringToCheckUIEventType(const std::string& input)
    {
        std::string event = string_utils::toLower(input);

        if (event == "pickup_piece")      return CheckUIEventType::PickupPiece;
        if (event == "drop_piece")        return CheckUIEventType::DropPiece;
        if (event == "reset_board")       return CheckUIEventType::ResetBoard;
        if (event == "save_board")        return CheckUIEventType::SaveBoard;
        if (event == "pawn_promo_choice") return CheckUIEventType::PawnPromotion;
        if (event == "stop")              return CheckUIEventType::Stop;

        return CheckUIEventType::Unknown;
    }


    std::string CheckUIEventTypeToString(CheckUIEventType event)
    {
        switch(event)
        {
            case CheckUIEventType::PickupPiece:   return "pickup_piece";
            case CheckUIEventType::DropPiece:     return "drop_piece";
            case CheckUIEventType::ResetBoard:    return "reset_board";
            case CheckUIEventType::SaveBoard:     return "save_board";
            case CheckUIEventType::PawnPromotion: return "pawn_promo_choice";
            case CheckUIEventType::Stop:          return "stop";
            case CheckUIEventType::Unknown:
            default:                              return "unknown";
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
                if (eventTokens.size() != 3)
                    MIA_THROW(error::ErrorCode::Check_Malformed_Event, 
                              "Expected 3 tokens for PickupPiece event.");

                event.startPos = math::Coordinate<uint8_t>{eventTokenToCoordinate(eventTokens[1]),
                                                           eventTokenToCoordinate(eventTokens[2])};
                break;
            }
            case CheckUIEventType::DropPiece:
            {
                if (eventTokens.size() != 5)
                    MIA_THROW(error::ErrorCode::Check_Malformed_Event, 
                              "Expected 5 tokens for DropPiece event.");

                event.startPos = math::Coordinate<uint8_t>{eventTokenToCoordinate(eventTokens[1]),
                                                           eventTokenToCoordinate(eventTokens[2])};
                event.endPos = math::Coordinate<uint8_t>{eventTokenToCoordinate(eventTokens[3]),
                                                         eventTokenToCoordinate(eventTokens[4])};
                break;
            }
            case CheckUIEventType::PawnPromotion:
            {
                if (eventTokens.size() != 2)
                    MIA_THROW(error::ErrorCode::Check_Malformed_Event, 
                              "Expected 2 tokens for PawnPromotion event.");

                event.chosenPromoPiece = eventTokens[1];
                break;
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
        const std::optional<Piece>& piece = state.getPieceOnSquare({event.startPos->x,
                                                                    event.startPos->y});

        if (!piece.has_value())
        { // Nothing was picked up, so there are no legal moves to display or cache.
            return uiState;
        }

        PiecePosition piecePos;
        piecePos.piece = piece.value();
        piecePos.coordinate.x = event.startPos.value().x;
        piecePos.coordinate.y = event.startPos.value().y;

        std::vector<math::Coordinate<uint8_t>> legalMoves = getLegalMoves(piecePos, state);

        // The legal moves are always cached (the drop event validates against them),
        // but the UI highlights are only applied when they are enabled in the config.
        if (display)
        {
            for (const math::Coordinate<uint8_t>& legalMove : legalMoves)
            {
                module.call("highlightSquare", legalMove.x, legalMove.y);
            }
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
            state.movePiece(startPos, endPos);

            // Check if this was an en passant move.
            const std::optional<MoveState> lastMove = state.getLastMove();
            if (lastMove.has_value() && lastMove->wasEnPassant)
            { // Handle en passant in ui.
                int8_t direction = lastMove->piece.color == Color::White ? 1 : -1;
                module.call("removePiece", 
                            static_cast<uint8_t>(lastMove->endingPos.x), 
                            static_cast<uint8_t>(lastMove->endingPos.y - direction));
            }

            // Check if this was a pawn promotion triggering move.
            if (lastMove.has_value() && lastMove->triggersPawnPromo)
            { // Handle a pawn promotion in the ui.
                module.call("showPawnPromoBox", 
                            "queen,rook,bishop,knight", // TODO - This list should be generated for future expansion.
                            colorToString(lastMove->piece.color));

                /*
                 * This is a special case, we don't want the rest of this method 
                 * to change the turn and determine the game result until the pawn
                 * promotion turn has been selected. So... return ongoing and handle
                 * the rest of the event handling in processPawnPromotion(); This
                 * assumes that a pawn_promo_choice event immediately follows, which
                 * is something that is forced by the UI.
                 */
                 module.call("movePiece", startPos.x, startPos.y, endPos.x, endPos.y);
                 return ongoing;
            }

            // Check if the last move was a castle of the king.
            if (lastMove.has_value() && lastMove->wasCastling)
            { // Handles moving the rook during castling.
                CastlingMetaData castleData = getCastlingRookData(lastMove.value(), state);
                math::Coordinate<uint8_t> startRookPos = castleData.rookStartingPos;
                math::Coordinate<uint8_t> endRookPos = castleData.rookEndingPos;
                state.movePiece(startRookPos, endRookPos);
                module.call("movePiece", startRookPos.x, startRookPos.y, endRookPos.x, endRookPos.y);
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


    GameResult processPawnPromotion(PythonModule& module,
                                    const CheckUIEvent& event,
                                    BoardState& state)
    {
        GameResult ongoing;

        if (event.type != CheckUIEventType::PawnPromotion)
        { // Do nothing for non-pickup events.
            return ongoing;
        }

        Piece promoPiece;
        if (event.chosenPromoPiece.has_value())
        {
            promoPiece.type = stringToPieceType(event.chosenPromoPiece.value());
            if (promoPiece.type == PieceType::None)
                MIA_THROW(error::ErrorCode::Check_Malformed_Event);
        }
        else
        {
            MIA_THROW(error::ErrorCode::Check_Malformed_Event);
        }

        // The last move will determine which piece is getting promoted.
        const std::optional<MoveState> lastMove = state.getLastMove();

        if (!lastMove.has_value())
        { // There is no move to promote, so this event cannot be handled.
            MIA_THROW(error::ErrorCode::Check_Malformed_Event,
                      "Received a pawn promotion event without a pawn move to promote.");
        }

        promoPiece.color = lastMove->piece.color;

        module.call("setPiece", 
                    event.chosenPromoPiece.value(),
                    colorToString(lastMove->piece.color),
                    lastMove->endingPos.x,
                    lastMove->endingPos.y);
        state.placePiece(lastMove->endingPos, promoPiece); 

        // Advance to the next player's turn.
        state.advanceTurn();

        /*
         * The turn has been advanced, so this determines the result for the player who
         * has to respond to this move. This is the order the end of game conditions are
         * written for, as both checkmate and stalemate apply to the player on move.
         */
        GameResult result = getGameResult(state);

        return result;
    }


    void registerPieceArtInUI(PythonModule& module,
                              const std::vector<PieceArt>& pieceArts)
    {
        for (const PieceArt& pieceArt : pieceArts)
        {
            if (pieceArt.type == PieceArtType::Image)
            {
                MIA_THROW(error::ErrorCode::Feature_In_Dev, 
                          "Image files not yet supported for check app ui art.");
            }
            module.call("registerPiece", pieceArt.pieceName, pieceArt.art);
            timing::sleepMilliseconds(20);
        }
    }
} // namespace check
