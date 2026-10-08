/**
 * @file Player.cpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A class representing a player with containers and stats.
 */

#include <fstream>
#include <sstream>
#include "Player.hpp"

namespace rpg
{
    bool Player::saveToFile(const std::string& filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open()) 
        {
            return false;
        }
        file << attributes.serialize() << std::endl;
        file << vitals.serialize() << std::endl;
        file << wallet.serialize() << std::endl;
        file << progress.serialize() << std::endl;
        file.close();
        return true;
    }


    bool Player::loadFromFile(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) 
        {
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        std::string data = buffer.str();
        try 
        {
            vitals = stats::Vitals::deserialize(data);
            wallet = currency::Wallet::deserialize(data);
            attributes = stats::Attributes::deserialize(data);
            progress = progress::ProgressMarkers::deserialize(data);
        } 
        catch (const std::exception&) 
        {
            return false;
        }

        return true;
    }
} // namespace rpg
