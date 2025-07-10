/**
 * File: RPGSimulator.hpp
 * Author: Antonius Torode
 * Created on: 07/10/2025
 * Description: Helper for a terminal-based RPG simulator that lists and executes player actions.
 *     This file is used for testing random features of the RPG system.
 */
#pragma once

#include "Player.hpp"
#include "CurrencyRegistry.hpp"
#include "VitalRegistry.hpp"

namespace rpg_sim
{
    /**
     * Sets up the simulator by giving some initial values to things.
     * Runs the selected action and continues until the user exits.
     * @param player[rpg::Player&] - The player data.
     * @param saveFile[std::string&  ] - The file to save the data to.
     */
    void setupSimulator(rpg::Player& player, std::string&   saveFile);
    
    /**
     * Displays the menu of possible actions and processes user input.
     * Runs the selected action and continues until the user exits.
     * @param player[rpg::Player&] - The player data.
     * @param saveFile[std::string&  ] - The file to save the data to.
     */
    void runSimulator(rpg::Player& player, std::string&   saveFile);

    /**
     * Simulates fighting a random mob, affecting vitals.
     * @param player[rpg::Player&] - The player data.
     * @param saveFile[std::string&  ] - The file to save the data to.
     */
    void fightMob(rpg::Player& player, std::string&   saveFile);

    /**
     * Simulates looting a treasure chest, gaining currency.
     * @param player[rpg::Player&] - The player data.
     * @param saveFile[std::string&  ] - The file to save the data to.
     */
    void lootTreasure(rpg::Player& player, std::string&   saveFile);

    /**
     * Simulates resting to recover vitals.
     * @param player[rpg::Player&] - The player data.
     * @param saveFile[std::string&  ] - The file to save the data to.
     */
    void rest(rpg::Player& player, std::string&   saveFile);

    /**
     * Simulates spending currency at a vendor.
     * @param player[rpg::Player&] - The player data.
     * @param saveFile[std::string&  ] - The file to save the data to.
     */
    void spendCurrency(rpg::Player& player, std::string&   saveFile);

    extern currency::CurrencyRegistry& currencyRegistry;
    extern stats::VitalRegistry& vitalRegistry;
} // namespace rpg_sim
