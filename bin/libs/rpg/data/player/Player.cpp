/**
 * File: Player.cpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
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
        file << vitals.serialize() << "\n";
        file << wallet.serialize() << "\n";
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
        } 
        catch (const std::exception&) 
        {
            return false;
        }

        return true;
    }
    
} // namespace rpg
