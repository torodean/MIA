/**
 * @file CheckConfig.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief This creates storage to hold and parse all of the configuration values
 *        for the CHECK application.
 */
#pragma once

#include <cstdint>
#include <vector>

#include "MathTypes.hpp"
#include "Piece.hpp"
#include "PlayerState.hpp"
#include "MIAConfig.hpp"

namespace check
{
    /**
     * Storage for all configurable CHECK application values.
     */
    struct CheckConfig
    {
        bool verboseMode{false};   ///< Whether or not to force verboseMode.
        bool showLegalMoves{true}; ///< Whether or not to force verboseMode.
        uint8_t boardSizeX;        ///< The x size of the board.
        uint8_t boardSizeY;        ///< The y size of the board.

        /// State of the players when the game starts.
        PlayerState startingPlayerState;

        /// Stores the starting board position.
        std::vector<PiecePosition> startingPosition;
    }; // struct CheckConfig

    /**
     * This is a helper to convert a string to a startingPosition. The string input
     * has a pre-defined expected format. The various positions are separated by a
     * semi-colon (";") delimiter, and each element of the positions are separated
     * by a comma (",") delimiter. Each position expects a string of the format:
     * "<x-position>,<y-position>,<piece type>,<piece color>"
     * @param input The input string to parse.
     * @return A vector of PiecePosition objects constructed from the input.
     * @throws MIAException if the string is malformed.
     */
    std::vector<PiecePosition> stringToStartingPosition(const std::string& input);

    /**
     * This is a helper to convert a startingPosition to a string format. This can
     * be helpful for things such as saving a game state, where the positions need
     * to be converted to a string. Each position is converted to a string of the 
     * format: "<x-position>,<y-position>,<piece type>,<piece color>", and each
     * position is separated by a semi-colon (";") delimiter.
     * @param positions A Vector of piece positions.
     * @param A string representation of the piece positions.
     */
    std::string positionsToString(const std::vector<PiecePosition>& positions);

    /**
     * This will construct a CheckConfig object from the MIAConfig by pulling out
     * and parsing all necessary values.
     * @throws MIAException for various malformed configuration components.
     */
    CheckConfig constructConfig(const config::MIAConfig& config);    

    /**
     * Converts a CheckConfig into a vector of state strings. Each state string is
     * of the format "<x-position>,<y-position>,<piece type>,<piece color>". These
     * state strings are what the setBoardState(cells) method in the python module
     * expects.
     * @param input The input CheckConfig to parse.
     * @return A vector of strings constructed from the input.
     */
    std::vector<std::string> positionsToStateStrings(
        const std::vector<PiecePosition>& positions);
} // namespace check
