/**
 * @file RPGSimulator.hpp
 * @author Antonius Torode
 * @date 07/10/2025
 * Description: Helper for a terminal-based RPG simulator that lists and executes player actions.
 *     This file is used for testing random features of the RPG system.
 */
#pragma once

#include <string>

#include "RuntimeContext.hpp"
#include "Player.hpp"
#include "CurrencyRegistry.hpp"
#include "VitalRegistry.hpp"
#include "AttributeRegistry.hpp"
#include "ProgressRegistry.hpp"

namespace rpg_sim
{
    /**
     * Sets the runtime context used by the simulator methods to log diagnostics.
     * This must be called before any simulator method which logs diagnostics;
     * when it is not called, diagnostic messages are discarded.
     * @param context The application's runtime context, which must outlive the
     *        simulator's use of it.
     */
    void setRuntimeContext(const RuntimeContext& context);

    /**
     * Sets up the simulator by giving some initial values to things.
     * @param player The player data.
     */
    void setupSimulator(rpg::Player& player);

    /**
     * Displays the player status (various indicators used by this simulator).
     * @param player The player data.
     */
    void displayPlayerStatus(rpg::Player& player);

    /**
     * Displays the various simulator options.
     */
    void displaySimulatorOptions();

    /**
     * Displays the menu of possible actions and processes user input.
     * Runs the selected action and continues until the user exits.
     * @param player The player data.
     * @param saveFile The file to save the data to.
     */
    void runSimulator(rpg::Player& player, std::string saveFile = "");

    /**
     * Simulates fighting a random mob, affecting vitals. This is entirely
     * automated and just loops over rounds with some simplified outcomes.
     * @param player The player data.
     */
    void fightMobAutomated(rpg::Player& player);
    
    /*
     * Creates a random Mob appropriate for the player to face. This uses the
     * rpg::Player class because a mob/boss could potentially have all the same
     * values stored in this class.
     *
     * Create a somewhat random mob to fight. Mob health is 10% to 150% of the 
     * players health. Strength is random and less than (or equal to) the player's.
     * Dexterity is a random value from 1 to twice the players dexterity (this gives 
     * about a 50/50 chance for the mob to go first in battles).
     *
     * @param player The player data.
     */
    rpg::Player createMob(rpg::Player& player);
    
    /**
     * Displays the status of a mob.
     * @param mob The mob data to display.
     * 
     */
    void displayMobStatus(rpg::Player& mob);
    
    /**
     * Displays the various fight simulator options.
     */
    void displayFightSimOptions();
    
    /**
     * Simulates a player attacking a mob. This is a physical attack,
     * so the attack damage is increased by strength value.
     * @param player The player data.
     * @param mob The mob data.
     * @return true if the fight should continue, false otherwise.
     */
    bool playerAttacksMob(rpg::Player& player, rpg::Player& mob);
    
    /**
     * Simulates a mob attacking a player. This is a physical attack,
     * so the attack damage is increased by strength value.
     * @param player The player data.
     * @param mob The mob data.
     * @return true if the fight should continue, false otherwise.
     */
    bool mobAttacksPlayer(rpg::Player& player, rpg::Player& mob);
    
    /**
     * Simulates a player attacking a mob with a spell. This is a magic attack,
     * so the attack damage is increased by the players intellect value. This
     * also consumes mana based on how much damage is done.
     *
     * @param player The player data.
     * @param mob The mob data.
     * @return true if the fight should continue, false otherwise.
     */
    bool castSpellAgainstMob(rpg::Player& player, rpg::Player& mob);
    
    /**
     * Simulates a player healing themselves. The heal amount is increased by the
     * players intellect value. This also consumes mana based on how much is healed.
     * This restores a random amount of missing health. Health is restored at a rate 
     * of 1 health per 1 mana.
     *
     * @param player The player data.
     */
    void heal(rpg::Player& player);
    
    /**
     * Simulates a player fleeing from a mob. The stronger the mob is, the harder
     * it is to flee.
     * @return true if the fight should continue, false otherwise.
     */
    bool fleeMob(rpg::Player& player, rpg::Player& mob);

    /**
     * Simulates fighting a random mob, affecting vitals. This provides the
     * user with various options to perform during the battle.
     * @param player The player data.
     */
    void fightMobManual(rpg::Player& player);

    /**
     * Simulates looting a treasure chest, gaining currency.
     * @param player The player data.
     */
    void lootTreasure(rpg::Player& player);

    /**
     * Types of rests that are available. This is an enum rather than a bool
     * in case it is expanded later.
     */
    enum RestType
    {
        shortRest,   ///< A short rest, recovers a small amount of vitals.
        longRest     ///< A long rest, recovers all vitals.
    };
    
    /// Converts a RestType value to a string.
    std::string restTypeToString(RestType type);

    /**
     * Simulates resting to recover vitals.
     * @param player The player data.
     * @param restType Determines the type of rest.
     */
    void rest(rpg::Player& player, RestType restType = RestType::shortRest);

    /**
     * Simulates spending currency at a vendor.
     * @param player The player data.
     */
    void spendCurrency(rpg::Player& player);

    /**
     * Levels up the player by randomly increasing Intelligence and Constitution attributes.
     * Updates the player's current attribute values and outputs the increases to the console.
     * @param player The player whose attributes are increased.
     */
    void levelUp(rpg::Player& player);
    
    /**
     * Checks if the player is dead.
     * @param player The player data.
     * @return true if the player is dead, false otherwise.
     */
    bool isDead(rpg::Player& player);
    
    /**
     * Revives the player with 1 health.
     * @param player The player data.
     */
    void revive(rpg::Player& player);
    
    /**
     * Saves the game state and player data to a file..
     * @param player The player data.
     * @param saveFile The file to save the data to.
     */
    void savePlayerData(rpg::Player& player, std::string& saveFile);

    /// The registries to locate the available game metadata.
    extern currency::CurrencyRegistry& currencyRegistry;
    extern stats::VitalRegistry& vitalRegistry;
    extern stats::AttributeRegistry& attributeRegistry;
    extern progress::ProgressRegistry& progressRegistry;

} // namespace rpg_sim
