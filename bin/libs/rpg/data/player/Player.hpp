/**
 * File: Player.hpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A class representing a player with containers and stats.
 */
#pragma once

#include <string>
#include "Wallet.hpp"
#include "Vitals.hpp"

namespace rpg
{
    class Player
    {
    public:
        /**
         * Default constructor.
         */
        Player() = default;

        /**
         * Gets the player's currency container.
         * @return Reference to the currency container.
         */
        currency::Wallet& getWallet() { return wallet; }

        /**
         * Writes the contents of the player's data to a file.
         * @param filename The name of the file to write to.
         * @return True if successful, false if the file cannot be opened.
         */
        bool saveToFile(const std::string& filename) const;
        
        /**
         * Loads the player's data from a file.
         * The file is expected to contain serialized blocks for each data type.
         *
         * @param filename The path to the file containing the saved wallet data.
         * @return True if loading and deserialization succeed, false otherwise.
         */
        bool loadFromFile(const std::string& filename);

    private:
        currency::Wallet wallet;  ///< The player's currency container.
        stats::Vitals vitals;     ///< The player's vitals.
    };
} // namespace rpg
