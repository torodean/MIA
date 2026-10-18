/**
 * @file PlayerState.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief This defines the state of the players (who's turn is it, how many players, etc).
 */
#pragma once

#include <cstdint>

#include "Piece.hpp"

namespace check
{
    /**
     * Storage for a player state. This stores things like the number of players,
     * and who's turn it is.
     */
    struct PlayerState
    {
        /// Default constructor. This uses usual chess values (2 players, player 1 is first).
        PlayerState() = default;

        /**
         * Constructor for the player state which defines various values. 
         */
        PlayerState(uint8_t numOfPlayers, uint8_t whosTurn) :
            numberOfPlayers(numOfPlayers), whosTurnIsIt(whosTurn) {};
    
        uint8_t numberOfPlayers{2}; ///< Total number of players.
        uint8_t whosTurnIsIt{1};    ///< Stores whos turn it is.
        
        /**
         * Advance to the next players turn. This wil increment whosTurnIsIt
         * until it reaches the number of players, then loop back around to 1.
         */
        void advanceTurn();
    }; // class PlayerState
    
    /**
     * Converts the current player's index to the corresponding piece color.
     * Player 1 is White, player 2 is Black, player 3 is Red, and player 4 is Green.
     * @param whosTurnIsIt The current state of the players.
     * @return The color corresponding to the current player's index, or Color::None
     *         if the index does not correspond to a defined color.
     */
    Color whosTurnIsItToColor(uint8_t whosTurnIsIt);
    
    /**
     * Converts the current player's index to the corresponding piece color.
     * Player 1 is White, player 2 is Black, player 3 is Red, and player 4 is Green.
     * @param playerState The current state of the players.
     * @return The color corresponding to the current player's index, or Color::None
     *         if the index does not correspond to a defined color.
     */
    Color whosTurnIsItToColor(const PlayerState& playerState);
}; // namespace check
