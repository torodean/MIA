/**
 * @file PlayerState.hpp
 * @author Antonius Torode
 * @date 10/15/2026
 * @brief This defines the state of the players (who's turn is it, how many players, etc).
 */
#pragma once

#include <cstdint>

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
    }; // class PlayerState
}; // namespace check
