/**
 * @file PieceType.cpp
 * @author Antonius Torode
 * @date 11/03/2026
 * @brief Implements the features in PieceType.hpp
 */

// Include the associated header file.
#include "PieceType.hpp"

// Used for case insensitive string matching.
#include "StringUtils.hpp"

namespace check
{
    PieceType& operator++(PieceType& type)
    {
        if (type == PieceType::Count)
        { // PieceType::Count is the maximum value allowed by this.
            type = PieceType::Count;
            return type;
        }

        type = static_cast<PieceType>(static_cast<uint8_t>(type) + 1);
        return type;
    }


    std::string pieceTypeToString(PieceType type)
    {
        switch(type)
        {
            case PieceType::King:    return "king";
            case PieceType::Queen:   return "queen";
            case PieceType::Pawn:    return "pawn";
            case PieceType::Bishop:  return "bishop";
            case PieceType::Knight:  return "knight";
            case PieceType::Rook:    return "rook";
            case PieceType::Templar: return "templar";
            case PieceType::Golem:   return "golem";
            case PieceType::Shard:   return "shard";
            case PieceType::Ankh:    return "ankh";
            case PieceType::Spade:   return "spade";
            case PieceType::Pedestal:return "pedestal";
            case PieceType::None:
            default:                 return "none";
        }
    }


    PieceType stringToPieceType(const std::string& type)
    {
        std::string input = string_utils::toLower(type);

        if (input == "king")    return PieceType::King;
        if (input == "queen")   return PieceType::Queen;
        if (input == "pawn")    return PieceType::Pawn;
        if (input == "bishop")  return PieceType::Bishop;
        if (input == "knight")  return PieceType::Knight;
        if (input == "rook")    return PieceType::Rook;
        if (input == "templar") return PieceType::Templar;
        if (input == "golem")   return PieceType::Golem;
        if (input == "shard")   return PieceType::Shard;
        if (input == "ankh")    return PieceType::Ankh;
        if (input == "spade")   return PieceType::Spade;
        if (input == "pedestal")return PieceType::Pedestal;

        // Default.
        return PieceType::None;
    }
} // namespace check
