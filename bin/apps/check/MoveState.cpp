/**
 * @file MoveState.cpp
 * @author Antonius Torode
 * @date 11/05/2026
 * @brief Implements the MoveState string conversion defined in the MoveState API.
 */

// Include the associated header.
#include "MoveState.hpp"

#include <cstdint>
#include <sstream>

// Needed to convert the moved piece's type for the glyph lookup.
#include "PieceType.hpp"
#include "StringUtils.hpp"

namespace check
{
    namespace
    {
        /**
         * Converts a square coordinate into the label the board's labeled border
         * uses: the file letter ("a", "b", "c", ...) followed by the rank number
         * counting up from the bottom ("1", "2", "3", ...). For example, the
         * coordinate {4, 1} becomes "e2".
         *
         * @param pos The coordinate of the square to convert.
         * @return The square label for the coordinate.
         */
        std::string squareToString(const math::Coordinate<uint8_t>& pos)
        {
            return std::string(1, static_cast<char>('a' + pos.x)) + std::to_string(pos.y + 1);
        }

        /**
         * Converts a configured "\\uXXXX" glyph art string into the utf-8 encoded
         * character string it represents. This assumes the input was already
         * identified as a glyph (see determinePieceArtType in CheckConfig.hpp),
         * which guarantees the exact 6-character escape form.
         *
         * @param art The configured glyph art escape string.
         * @return The utf-8 encoded string for the glyph.
         */
        std::string glyphArtToUtf8(const std::string& art)
        {
            uint32_t codepoint = static_cast<uint32_t>(std::stoul(art.substr(2), nullptr, 16));

            std::string utf8;
            if (codepoint < 0x80)
            {
                utf8 += static_cast<char>(codepoint);
            }
            else if (codepoint < 0x800)
            {
                utf8 += static_cast<char>(0xC0 | (codepoint >> 6));
                utf8 += static_cast<char>(0x80 | (codepoint & 0x3F));
            }
            else if (codepoint < 0x10000)
            {
                utf8 += static_cast<char>(0xE0 | (codepoint >> 12));
                utf8 += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                utf8 += static_cast<char>(0x80 | (codepoint & 0x3F));
            }
            else
            {
                utf8 += static_cast<char>(0xF0 | (codepoint >> 18));
                utf8 += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
                utf8 += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                utf8 += static_cast<char>(0x80 | (codepoint & 0x3F));
            }

            return utf8;
        }
    } // namespace


    std::string moveStateToString(const MoveState& move, const std::vector<PieceArt>& pieceArts)
    {
        std::string output;

        /* 
         * Prefix the glyph for the piece that moved when glyph art backs the piece type. No glyph 
         * (e.g., image art or unconfigured art) leaves the prefix off entirely.
         */
        for (const PieceArt& art : pieceArts)
        {
            if (art.pieceName == pieceTypeToString(move.piece.type) &&
                art.type == PieceArtType::Glyph &&
                art.art.size() == 6 && art.art.substr(0, 2) == "\\u" &&
                string_utils::isHexDigits(art.art.substr(2)))
            {
                output += glyphArtToUtf8(art.art) + ": ";
                break;
            }
        }

        output += squareToString(move.startingPos);
        output += move.wasCapture ? " -x " : " -> ";
        output += squareToString(move.endingPos);

        return output;
    }


    std::string createSaveLine(const MoveState& moveState)
    {
        std::stringstream output;
        output << moveState.startingPos << ";" << moveState.endingPos;
        return output.str();
    }


    MoveState loadFromSaveLine(const std::string& input)
    {
        using namespace string_utils;

        MoveState state;

        std::vector<std::string> coords = delimiterString(input, ';');
        /*
         * Coordinates are written by Coordinate's operator<< as "(x, y)" (with a
         * space after the comma), so the value between ',' and ')' carries a
         * leading space. Trim before converting, since stringToUint8() rejects
         * any non-digit character and would otherwise throw.
         */
        state.startingPos.x = stringToUint8(trim(getBetweenXAndY(coords[0], '(', ',')));
        state.startingPos.y = stringToUint8(trim(getBetweenXAndY(coords[0], ',', ')')));
        state.endingPos.x = stringToUint8(trim(getBetweenXAndY(coords[1], '(', ',')));
        state.endingPos.y = stringToUint8(trim(getBetweenXAndY(coords[1], ',', ')')));

        return state;
    }
} // namespace check
