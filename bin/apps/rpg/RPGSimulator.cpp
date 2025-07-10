/**
 * File: RPGSimulator.cpp
 * Author: Antonius Torode
 * Created on: 07/10/2025
 * Description: Implementation of a terminal-based RPG simulator that lists and executes player actions.
 */

#include "RPGSimulator.hpp"
#include "Vitals.hpp"
#include "Wallet.hpp"
#include <iostream>
#include <limits>

namespace rpg_sim
{
    currency::CurrencyRegistry& currencyRegistry = currency::CurrencyRegistry::getInstance();
    stats::VitalRegistry& vitalRegistry = stats::VitalRegistry::getInstance();

    void setupSimulator(rpg::Player& player, std::string&  saveFile)
    {
        // TODO
    }

    void runSimulator(rpg::Player& player, std::string& saveFile)
    {
        while (true)
        {
            // Display player status
            std::cout << "\nPlayer Status: TODO\n" << std::endl;

            // Display menu
            std::cout << "\nAvailable Actions:\n"
                      << "1. Fight a Mob\n"
                      << "2. Loot Treasure\n"
                      << "3. Rest\n"
                      << "4. Spend Currency\n"
                      << "99. Exit\n"
                      << "Enter integer choice: ";

            int choice;
            std::cin >> choice;

            // Clear input buffer
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Handle choice
            switch (choice)
            {
                case 1:
                    fightMob(player, saveFile);
                    break;
                case 2:
                    lootTreasure(player, saveFile);
                    break;
                case 3:
                    rest(player, saveFile);
                    break;
                case 4:
                    spendCurrency(player, saveFile);
                    break;
                case 99:
                    std::cout << "Exiting simulator.\n";
                    return;
                default:
                    std::cout << "Invalid choice. Please enter a number between 1 and 9.\n";
            }
        }
    }

    void fightMob(rpg::Player& player, std::string& saveFile)
    {
        // TODO: Implement mob fighting logic affecting vitals
        std::cout << "Player engages a random mob, affecting vitals.\n";
    }

    void lootTreasure(rpg::Player& player, std::string& saveFile)
    {
        // TODO: Implement treasure looting logic adding currency
        std::cout << "Player loots a treasure chest, gaining currency.\n";
    }

    void rest(rpg::Player& player, std::string& saveFile)
    {
        // TODO: Implement rest logic recovering vitals
        std::cout << "Player rests to recover vitals.\n";
    }

    void spendCurrency(rpg::Player& player, std::string& saveFile)
    {
        // TODO: Implement currency spending logic
        std::cout << "Player spends currency at a vendor.\n";
    }
} // namespace rpg_sim

