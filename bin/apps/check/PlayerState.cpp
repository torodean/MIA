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


    Color whosTurnIsItToColor(uint8_t whosTurnIsIt)
    {
        switch (whosTurnIsIt)
        {
            case 1:  return Color::White;
            case 2:  return Color::Black;
            case 3:  return Color::Red;
            case 4:  return Color::Green;
            default: return Color::None;
        }
    }


    Color whosTurnIsItToColor(const PlayerState& playerState)
    {
        return whosTurnIsItToColor(playerState.whosTurnIsIt);
    }
}; // namespace check
