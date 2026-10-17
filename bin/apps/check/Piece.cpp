/**
 * @file Piece.cpp
 * @author Antonius Torode
 * @date 10/16/2026
 * @brief Implements the features in Piece.hpp
 */

// Include the associated header file.
#include "Piece.hpp"
#include "StringUtils.hpp"

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
} // namespace check
