/**
 * @file CheckConfig.cpp
 * @author Antonius Torode
 * @date 10/16/2026
 * @brief Implements the CheckConfig methods and features.
 */

// Include the associated header file.
#include "CheckConfig.hpp"
#include "Piece.hpp"
#include "StringUtils.hpp"
// For error handling.
#include "MIAException.hpp"
#include "Error.hpp"

namespace check
{
    std::vector<PiecePosition> stringToStartingPosition(const std::string& input)
    {
        std::vector<PiecePosition> positions;
        std::vector<std::string> entries = string_utils::delimiterString(input, ';');

        for (std::string& entry : entries)
        { // Trim any white space from the entry.
            entry = string_utils::trim(entry);
        }

        for (const std::string& entry : entries)
        {
            if (entry.empty())
                continue;

            std::vector<std::string> parts = string_utils::delimiterString(entry, ',');
            if (parts.size() != 4)
            {
                std::string err = "Invalid starting position format: " + entry;
                MIA_THROW(error::ErrorCode::Invalid_Parameter, err);
            }

            for (std::string& part : parts)
            { // Trim any white space from the parts.
                part = string_utils::trim(part);
            }

            PiecePosition pos;
            pos.coordinate = math::Coordinate<uint8_t>(string_utils::stringToUint8(parts[0]),
                                                       string_utils::stringToUint8(parts[1]) );
            pos.piece.type = stringToPieceType(parts[2]);
            pos.piece.color = stringToColor(parts[3]);
            positions.push_back(pos);
        }
        return positions;
    }


    std::string positionsToString(const std::vector<PiecePosition>& positions)
    {
        std::string output;
        for (const PiecePosition& pos : positions)
        {
            if (!output.empty())
                output += ";";

            output += std::to_string(pos.coordinate.x) + "," + std::to_string(pos.coordinate.y);
            output += "," + pieceTypeToString(pos.piece.type);
            output += "," + colorToString(pos.piece.color);
        }
        return output;
    }


    CheckConfig constructConfig(const config::MIAConfig& config)
    {
        CheckConfig out;

        out.verboseMode = config.getBool("verbose_mode");
        out.showLegalMoves = config.getBool("show_legal_moves");
        out.boardSizeX = string_utils::stringToUint8(config.getString("board_size_x"));
        out.boardSizeY = string_utils::stringToUint8(config.getString("board_size_y"));
        out.startingPlayerState = PlayerState(string_utils::stringToUint8(config.getString("number_of_players")),
                                              string_utils::stringToUint8(config.getString("whos_turn_is_it")));
        out.startingPosition = stringToStartingPosition(config.getString("starting_state"));
        out.pieceArts = extractPieceArtsFromConfig(config, "piece_");

        return out;
    }


    std::vector<std::string> positionsToStateStrings(
        const std::vector<PiecePosition>& positions)
    {
        std::string stateStringWhole = positionsToString(positions);
        std::vector<std::string> output = string_utils::delimiterString(stateStringWhole, ';');
        return output;
    }


    PieceArtType determinePieceArtType(const std::string& input)
    {
       if(input.size() == 6 && input.substr(0, 2) == "\\u" && string_utils::isHexDigits(input.substr(2)))
            return PieceArtType::Glyph;

        if(!input.empty()) // TODO - add a file check here.
            return PieceArtType::Image;

        return PieceArtType::Unknown;
    }


    std::vector<PieceArt> extractPieceArtsFromConfig(const config::MIAConfig& config,
                                                     const std::string& configPrefix)
    {
        std::vector<PieceArt> piecesArt;

        for (PieceType type = PieceType::King; type < PieceType::Count; ++type)
        { // Iterate over all valid piece types.
            PieceArt pieceArt;
            pieceArt.pieceName = pieceTypeToString(type);
            std::string configKey = configPrefix + pieceArt.pieceName;
            pieceArt.art = config.getString(configKey);
            pieceArt.type = determinePieceArtType(pieceArt.art);
            piecesArt.push_back(pieceArt);
        }

        return piecesArt;
    }
} // namespace check
