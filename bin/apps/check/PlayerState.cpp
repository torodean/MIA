/**
 * @file PlayerState.cpp
 * @author Antonius Torode
 * @date 10/17/2026
 * @brief Implements the PlayerState class methods.
 */

// Include the associated header file.
#include "PlayerState.hpp"


namespace check
{
    void PlayerState::advanceTurn()
    {
        if (whosTurnIsIt < numberOfPlayers)
        { // Advance to the next players turn.
            whosTurnIsIt++;
        }

        // If it's the last players turn, loop back to player 1.
        whosTurnIsIt = 1;
    }
}; // namespace check
