/**
 * @file Piece.cpp
 * @author Antonius Torode
 * @date 10/16/2026
 * @brief Implements the features in Piece.hpp
 */

#include <array>

// Include the associated header file.
#include "Piece.hpp"
#include "StringUtils.hpp"
#include "BoardState.hpp"
#include "PlayerState.hpp"

// Include the various piece types.
#include "Pawn.hpp"
#include "Queen.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"
#include "Knight.hpp"
#include "King.hpp"

namespace check
{
    std::string pieceTypetoString(PieceType type)
    {
        switch(type)
        {
            case PieceType::King:   return "king";
            case PieceType::Queen:  return "queen";
            case PieceType::Pawn:   return "pawn";
            case PieceType::Bishop: return "bishop";
            case PieceType::Knight: return "knight";
            case PieceType::Rook:   return "rook";
            case PieceType::None:
            default:                return "none";
        }
    }


    PieceType stringToPieceType(const std::string& type)
    {
        std::string input = string_utils::toLower(type);
        
        if (input == "king")   return PieceType::King;
        if (input == "queen")  return PieceType::Queen;
        if (input == "pawn")   return PieceType::Pawn;
        if (input == "bishop") return PieceType::Bishop;
        if (input == "knight") return PieceType::Knight;
        if (input == "rook")   return PieceType::Rook;

        // Default.
        return PieceType::None;
    }


    std::string colorToString(Color color)
    {
        switch(color)
        {
            case Color::White: return "white";
            case Color::Black: return "black";
            case Color::Red:   return "red";
            case Color::Green: return "green";
            case Color::None:
            default:           return "none";
        }
    }


    Color stringToColor(const std::string& color)
    {
        std::string input = string_utils::toLower(color);
        
        if (input == "white") return Color::White;
        if (input == "black") return Color::Black;
        if (input == "red")   return Color::Red;
        if (input == "green") return Color::Green;

        // Default.
        return Color::None;
    }


    std::vector<math::Coordinate<uint8_t>> BasePieceLogic::getLegalCaptures(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        std::vector<math::Coordinate<uint8_t>> captures;

        for (const auto& move : getLegalMoves(currentPos, boardState, color))
        {
            if (boardState.containsOpponentPiece(move.x, move.y, color))
            {
                captures.push_back(move);
            }
        }

        return captures;
    }


    std::vector<math::Coordinate<uint8_t>> BasePieceLogic::getAttackedSquares(
        const math::Coordinate<uint8_t>& currentPos,
        const BoardState& boardState,
        Color color)
    {
        return getLegalMoves(currentPos, boardState, color);
    }


    std::unique_ptr<BasePieceLogic> createPieceLogic(PieceType type)
    {
        switch (type)
        {
            case PieceType::Pawn:
                return std::make_unique<PawnLogic>();

            case PieceType::Queen:
                return std::make_unique<QueenLogic>();

            case PieceType::Bishop:
                return std::make_unique<BishopLogic>();

            case PieceType::Rook:
                return std::make_unique<RookLogic>();

            case PieceType::Knight:
                return std::make_unique<KnightLogic>();

            case PieceType::King:
                return std::make_unique<KingLogic>();

            case PieceType::Count:
            case PieceType::None:
                return nullptr;
        }

        return nullptr;
    }


    std::vector<math::Coordinate<uint8_t>> getLegalMoves(const PiecePosition& piecePos,
                                                         const BoardState& boardState)
    {
        auto logic = createPieceLogic(piecePos.piece.type);

        if (!logic)
            return {};

        // Only the current player can move their own pieces.
        if (whosTurnIsItToColor(boardState.whosTurnIsIt()) != piecePos.piece.color)
            return {};

        /* 
         * These are the moves the piece could make if the king were not involved
         * (the piece's own movement rules, board bounds, and blocking pieces).
         */
        std::vector<math::Coordinate<uint8_t>> pseudoLegalMoves =
            logic->getLegalMoves(piecePos.coordinate, boardState, piecePos.piece.color);

        /* 
         * Remove every move which leaves this player's king in check. When the
         * king is in check, this leaves only the moves which get out of the
         * check: move the king off of the attacked square, capture the attacking
         * piece, or move a piece in between the attacker and the king. When the
         * king is not in check, this instead keeps a pinned piece from moving off
         * of the line which shields its king.
         */
        std::vector<math::Coordinate<uint8_t>> legalMoves;

        for (const math::Coordinate<uint8_t>& move : pseudoLegalMoves)
        { // Keep only the moves which leave the king safe.
            if (leavesKingInCheck(piecePos, boardState, move))
                continue;

            legalMoves.push_back(move);
        }

        return legalMoves;
    }


    std::vector<math::Coordinate<uint8_t>> getLegalCaptures(const PiecePosition& piecePos,
                                                            const BoardState& boardState)
    {
        auto logic = createPieceLogic(piecePos.piece.type);

        if (!logic)
            return {};

        std::vector<math::Coordinate<uint8_t>> pseudoLegalCaptures =
            logic->getLegalCaptures(piecePos.coordinate, boardState, piecePos.piece.color);

        // Apply the same check filtering used for the legal moves.
        std::vector<math::Coordinate<uint8_t>> legalCaptures;

        for (const math::Coordinate<uint8_t>& capture : pseudoLegalCaptures)
        { // Keep only the captures which leave the king safe.
            if (leavesKingInCheck(piecePos, boardState, capture))
                continue;

            legalCaptures.push_back(capture);
        }

        return legalCaptures;
    }


    bool leavesKingInCheck(const PiecePosition& piecePos,
                           const BoardState& boardState,
                           const math::Coordinate<uint8_t>& destination)
    {
        /*
         * Simulate the move on a copy of the board so the real game state stays
         * untouched. This copy is a full snapshot of the squares, the player
         * state, and the move history, so the move is applied exactly as it would
         * be in a real game (including special cases such as en passant, which
         * also removes the captured pawn from the simulated board).
         */
        BoardState simulatedBoard(boardState);

        simulatedBoard.movePiece(piecePos.coordinate.x, piecePos.coordinate.y,
                                 destination.x, destination.y);

        /* 
         * If this player's king is still attacked once the move is applied, the
         * move did not get the king out of check and is therefore not legal.
         */
        return isKingInCheck(simulatedBoard, piecePos.piece.color);
    }


    std::vector<math::Coordinate<uint8_t>> getAttackedSquares(const BoardState& boardState,
                                                              Color defendingColor)
    {
        std::vector<math::Coordinate<uint8_t>> attackedSquares;

        // Create each piece-logic implementation once.
        std::array<std::unique_ptr<BasePieceLogic>, static_cast<size_t>(PieceType::Count)> pieceLogic;

        for (uint8_t type = static_cast<uint8_t>(PieceType::King);
             type < static_cast<uint8_t>(PieceType::Count);
             ++type)
        { // Iterate over all valid piece types.
            PieceType pieceType = static_cast<PieceType>(type);
            pieceLogic[type] = createPieceLogic(pieceType);
        }

        for (uint16_t y = 0; y < boardState.getYSize(); ++y)
        {
            for (uint16_t x = 0; x < boardState.getXSize(); ++x)
            {
                const auto& piece = boardState.getPieceOnSquare(static_cast<uint8_t>(x),
                                                                static_cast<uint8_t>(y));

                // Skip empty squares and pieces belonging to the defending side.
                if (!piece.has_value() || 
                    piece->type == PieceType::None || 
                    piece->color == defendingColor)
                {
                    continue;
                }

                PiecePosition piecePos{*piece, {static_cast<uint8_t>(x), static_cast<uint8_t>(y)}};

                std::unique_ptr<BasePieceLogic>& logic = pieceLogic[static_cast<uint8_t>(piece->type)];

                if (!logic)
                    continue;

                std::vector<math::Coordinate<uint8_t>> pieceAttacks = 
                    logic->getAttackedSquares(piecePos.coordinate, boardState, piecePos.piece.color);

                attackedSquares.insert(attackedSquares.end(),
                                       pieceAttacks.begin(),
                                       pieceAttacks.end());
            }
        }

        return attackedSquares;
    }
} // namespace check
