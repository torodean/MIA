/**
 * File: Player.cpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A class representing a player with containers and stats.
 */

#include <fstream>
#include "Player.hpp"

namespace rpg
{

    bool Player::writeStatsToFile(const std::string& filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << "Player Inventory:\n";
        wallet.dump(file);
        file.close();
        return true;
    }

} // namespace rpg
