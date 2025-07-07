/**
 * File: Player.hpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A class representing a player with containers and stats.
 */
#pragma once

#include <string>
#include "CurrencyContainer.hpp"

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
        currency::CurrencyContainer& getWallet() { return wallet; }
        const currency::CurrencyContainer& getWallet() const { return wallet; }

        /**
         * Writes the contents of the player's currency container to a file.
         * @param filename The name of the file to write to.
         * @return True if successful, false if the file cannot be opened.
         */
        bool writeStatsToFile(const std::string& filename) const;

    private:
        currency::CurrencyContainer wallet; ///< The player's currency container.
    };
} // namespace rpg
