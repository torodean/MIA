/**
 * @file RPGSimulator.cpp
 * @author Antonius Torode
 * @date 07/10/2025
 * Description: Implementation of a terminal-based RPG simulator that lists and executes player actions.
 */

#include <iostream>
#include <limits>
#include <cmath>   // for round()

// rpg-related includes.
#include "RPGSimulator.hpp"
#include "Vitals.hpp"
#include "Wallet.hpp"
#include "Attributes.hpp"
#include "ModifierApplicator.hpp"

// MIA utility includes.
#include "MathUtils.hpp"

namespace rpg_sim
{
    currency::CurrencyRegistry& currencyRegistry = currency::CurrencyRegistry::getInstance();
    stats::VitalRegistry& vitalRegistry = stats::VitalRegistry::getInstance();
    stats::AttributeRegistry& attributeRegistry = stats::AttributeRegistry::getInstance();
    progress::ProgressRegistry& progressRegistry = progress::ProgressRegistry::getInstance();
    
    // Just so these aren't 'hard-coded' strings throughout the file.
    const std::string level = "Level";
    const std::string xp = "Experience";
    const std::string health = "Health";
    const std::string mana = "Mana";
    const std::string copper = "Copper Coin";
    const std::string silver = "Silver Coin";
    const std::string strength = "Strength";
    const std::string dexterity = "Dexterity";
    const std::string constitution = "Constitution";
    const std::string intelligence = "Intelligence";
    
    
    namespace helper_methods
    {
        void updateModifiers(rpg::Player& player)
        {
            rpg::helper_methods::applyModifiers
                <stats::AttributeRegistry, stats::VitalRegistry, 
                 stats::Attributes, stats::Vitals>
                 (attributeRegistry, vitalRegistry, player.getAttributes(), player.getVitals());
        }
    } // namespace helper_methods
    

    void setupSimulator(rpg::Player& player)
    {
        std::cout << "Creating default simulator values!" << std::endl;
        int initialHealth = vitalRegistry.getByName(health)->getBaseMax();
        int minHealth = vitalRegistry.getByName(health)->getBaseMin();
        int maxHealth = initialHealth;
        int initialMana = vitalRegistry.getByName(mana)->getBaseMax();
        int minMana = vitalRegistry.getByName(mana)->getBaseMin();
        int maxMana = initialMana;

        // Initialize vitals.
        player.getVitals().add(health, initialHealth, minHealth, maxHealth);
        player.getVitals().add(mana, initialMana, minMana, maxMana);
        std::cout << "Vitals initialized: " 
                  << "Health=" << initialHealth
                  << ", Mana=" << initialMana 
                  << std::endl;

        // Initialize wallet.
        player.getWallet().add(copper, 100);
        player.getWallet().add(silver, 10);
        std::cout << "Wallet initialized: Copper=100, Silver=10" << std::endl;
        
        // Initialize attributes.
        player.getAttributes().add(strength, 1);
        player.getAttributes().add(dexterity, 1);
        player.getAttributes().add(constitution, 1);
        player.getAttributes().add(intelligence, 1);
        std::cout << "Attributes initialized: Strength=1, Constitution=1, Intelligence=1." << std::endl;
        
        // Initialize progress markers.
        player.getProgress().add(level, 1);
        player.getProgress().add(xp, 0);
        
        // Apply stat cross-modifiers.
        helper_methods::updateModifiers(player);
    }
    
    
    void displayPlayerStatus(rpg::Player& player)
    {
        std::cout << "Player Status:" << std::endl
                  << "\tLevel: " << player.getProgress().get(level).get() << std::endl
                  << "\tXP: " << player.getProgress().get(xp).get() << std::endl
                  << "\tHealth: " << player.getVitals().get(health).getCurrent() << "/"
                                   << player.getVitals().get(health).getCurrentMax() << std::endl
                  << "\tMana: " << player.getVitals().get(mana).getCurrent() << "/"
                                   << player.getVitals().get(mana).getCurrentMax()<< std::endl
                  << "\tSilver: " << player.getWallet().get(silver).getQuantity() << std::endl
                  << "\tCopper: " << player.getWallet().get(copper).getQuantity() << std::endl
                  << "\tStrength: " << player.getAttributes().get(strength).getCurrent() << std::endl
                  << "\tDexterity: " << player.getAttributes().get(dexterity).getCurrent() << std::endl
                  << "\tConstitution: " << player.getAttributes().get(constitution).getCurrent() << std::endl
                  << "\tIntelligence: " << player.getAttributes().get(intelligence).getCurrent() << std::endl; 
    }
    
    
    void displaySimulatorOptions()
    {
        std::cout << "Available Actions:" << std::endl
                  << "0. Revive" << std::endl
                  << "1. Level up (automated)" << std::endl
                  << "2. Loot Treasure (automated)" << std::endl
                  << "5. Spend Currency (automated)" << std::endl
                  << "20. Fight a Mob (automated)" << std::endl
                  << "21. Fight a Mob (manual)" << std::endl
                  << "70. Short Rest (20% recovery)" << std::endl
                  << "71. Long Rest (100% recovery)" << std::endl
                  << "98. Save Game State" << std::endl
                  << "99. Exit" << std::endl
                  << "Enter integer choice: ";
    }
    

    void runSimulator(rpg::Player& player, std::string saveFile)
    {
        while (true)
        {
            // Display player status.
            displayPlayerStatus(player);

            // Display menu.
            std::cout << "=================================" << std::endl;
            displaySimulatorOptions();

            int choice;
            std::cin >> choice;

            // Clear input buffer
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            // Separator between available actions and action being performed.
            std::cout << "=================================" << std::endl;

            if (isDead(player) && choice != 0)
            {
                std::cout << "Player is dead, must revive before continuing!" << std::endl;
                continue;
            }
            
            // Handle choice
            switch (choice)
            {
                case 0:
                    revive(player);
                    break;
                case 1:
                    levelUp(player);
                    break;
                case 2:
                    lootTreasure(player);
                    break;
                case 5:
                    spendCurrency(player);
                    break;
                case 20:
                    fightMobAutomated(player);
                    break;
                case 21:
                    fightMobManual(player);
                    break;
                case 70:
                    rest(player, RestType::shortRest);
                    break;
                case 71:
                    rest(player, RestType::longRest);
                    break;
                case 98:
                    savePlayerData(player, saveFile);
                    break;
                case 99:
                    std::cout << "Exiting simulator." << std::endl;
                    return;
                default:
                    std::cout << "Invalid choice. Please enter a valid option." << std::endl;
            }
        }
    }
    

    void fightMobAutomated(rpg::Player& player)
    {
        int currentHealth = player.getVitals().get(health).getCurrent();
        int currentMana = player.getVitals().get(mana).getCurrent();

        std::cout << "Player encounters a hostile mob..." << std::endl;

        // Loop the rounds until the fight has a reason to end.
        int round = 0;
        /*
         * Any case that would end the fight uses a break in order to immediately stop the loop
         * rather than looping back around again.
         */
        while (true)
        {
            round++;
            std::cout << "\n-- Round " << round << " --" << std::endl;
            
            int mobDamage = 0;
            if (math::randomChance(0.1))
            { // Random chance (10%) the mob misses an attack.
                std::cout << "The mobs attack missed!" << std::endl;
            }
            else
            {
                mobDamage = math::randomInt(5, 20);
                std::cout << "Mob attacks! Player takes " << mobDamage << " damage." << std::endl;
            }

            if (player.getVitals().has(health, mobDamage)) 
            {
                currentHealth -= mobDamage;
                player.getVitals().update(health, currentHealth);
                std::cout << "Player survives with " << currentHealth << " health." << std::endl;
            } 
            else 
            {
                player.getVitals().update(health, 0);
                std::cout << "Player takes lethal damage and dies." << std::endl;
                break; // break the fight immediately.
            }

            // 50% chance to cast a spell.
            if (math::randomChance(0.5)) 
            {
                int spellCost = math::randomInt(10, 25);
                std::cout << "Player attempts to cast a spell (cost " << spellCost << " mana)." << std::endl;

                if (player.getVitals().has(mana, spellCost)) 
                {
                    currentMana -= spellCost;
                    player.getVitals().update(mana, currentMana);
                    std::cout << "Spell cast successfully. Remaining mana: " << currentMana << "." << std::endl;

                    std::cout << "The spell hits! Mob is damaged!" << std::endl;
                    if (math::randomChance(0.5))
                    { // 50% chance the spell kills the mob.
                        std::cout << "Mob is defeated!" << std::endl;
                        player.getProgress().get(xp).add(25); // Gain 25 xp from auto kills.
                        std::cout << "The player gains 25 xp!" << std::endl;
                        break;
                    }
                    else
                    {
                        std::cout << "Mob is still standing!" << std::endl;
                    }
                } 
                else 
                {
                    std::cout << "Not enough mana to cast the spell." << std::endl;
                    std::cout << "The player attempts to flee!" << std::endl;
                    if (math::randomChance(0.5))
                    { // 50% chance the player can flee the mob.
                        std::cout << "The player successfully flees!" << std::endl;
                        break;
                    }
                    else
                    {
                        std::cout << "The player was unable to flee!" << std::endl;
                    }
                }
            } 
            else 
            {
                std::cout << "Player was unable to cast a spell. Mob moving too fast!" << std::endl;
            }

            // Random chance (10%) mob flees after any round
            if (math::randomChance(0.1))
            {
                std::cout << "The mob suddenly flees!" << std::endl;
                break;
            }
        } // while (true)

        std::cout << "The Fight ends." << std::endl;
    } // fightMobAutomated()
    
    
    void displayMobStatus(rpg::Player& mob)
    {
        std::cout << "Mob Status:" << std::endl
                  << "\t Health: " << mob.getVitals().get(health).getCurrent() << "/"
                                   << mob.getVitals().get(health).getCurrentMax() << std::endl
                  << "\t Strength: " << mob.getAttributes().get(strength).getCurrent() << std::endl
                  << "\t Dexterity: " << mob.getAttributes().get(dexterity).getCurrent() << std::endl;
    }
    
    
    void displayFightSimOptions()
    {
        std::cout << "Available Actions:" << std::endl
                  << "1. Attack" << std::endl
                  << "2. Cast Spell" << std::endl
                  << "3. Heal" << std::endl
                  << "8. Do nothing" << std::endl
                  << "9. Flee" << std::endl
                  << "Enter integer choice: ";
    }
    
    
    bool playerAttacksMob(rpg::Player& player, rpg::Player& mob)
    {
        std::cout << "The player attacks the mob!" << std::endl;
        
        // Determine if the attack hits.
        double hitChance = 0.95; // 95% hit chance.
        int playerDexterity = player.getAttributes().get(dexterity).getCurrent();
        int mobDexterity = mob.getAttributes().get(dexterity).getCurrent();
        if (mobDexterity > playerDexterity)
            hitChance -= 0.1; // If the mob is faster, reduce hit chance by 10%.
        
        std::cout << "\tHit chance: " << hitChance << std::endl;

        if (!math::randomChance(hitChance))
        {
            std::cout << "The player's attack missed!" << std::endl;
            return true;
        }
        
        // Determine the attack damage.
        int attackDamage = math::randomInt(5, 20); // Base damage.
        int playerStrength = player.getAttributes().get(strength).getCurrent();
        attackDamage += playerStrength; // Add strength as attack damage.
        
        std::cout << "Attacking the mob for " << attackDamage << " damage!" << std::endl;
        
        // Update the mob health.        
        int mobHealth = mob.getVitals().get(health).getCurrent();
        mob.getVitals().update(health, mobHealth - attackDamage);
        
        // Determine if the fight should continue or not.
        if (mobHealth - attackDamage <= 0)
        {
            std::cout << "The mob has died!" << std::endl;
            // Gain xp equal to the max health of the mob killed.
            int xpGained = mob.getVitals().get(health).getCurrentMax();
            player.getProgress().get(xp).add(xpGained);
            std::cout << "The player gains " << xpGained << " xp!" << std::endl;
            return false;
        }

        return true;
    } // playerAttacksMob()
    

    bool mobAttacksPlayer(rpg::Player& player, rpg::Player& mob)
    {    
        std::cout << "The mob attacks the player!" << std::endl;

        // Determine if the attack hits.
        double hitChance = 0.95; // 95% hit chance.
        int playerDexterity = player.getAttributes().get(dexterity).getCurrent();
        int mobDexterity = mob.getAttributes().get(dexterity).getCurrent();

        if (playerDexterity > mobDexterity)
            hitChance -= 0.1; // If the player is faster, reduce the mob's hit chance by 10%.
            
        std::cout << "\tHit chance: " << hitChance << std::endl;

        if (!math::randomChance(hitChance))
        {
            std::cout << "The mob's attack missed!" << std::endl;
            return true;
        }
        
        // Determine the attack damage.
        int attackDamage = math::randomInt(5, 20); // Base damage.
        int mobStrength = mob.getAttributes().get(strength).getCurrent();
        attackDamage += mobStrength;

        std::cout << "The mob hits the player for " << attackDamage << " damage!" << std::endl;

        // Update the player's health.
        int playerHealth = player.getVitals().get(health).getCurrent();
        int remainingHealth = playerHealth - attackDamage;

        if (remainingHealth <= 0)
        {
            player.getVitals().update(health, 0);

            std::cout << "The player takes lethal damage and dies!" << std::endl;
            return false;
        }

        player.getVitals().update(health, remainingHealth);
        std::cout << "Player survives with " << remainingHealth << " health." << std::endl;
        
        return true;
    } // mobAttacksPlayer()
    

    bool castSpellAgainstMob(rpg::Player& player, rpg::Player& mob)
    {
        std::cout << "The player casts a spell at the mob!" << std::endl;
        
        // Determine if the spell hits.
        double hitChance = 0.75; // 75% hit chance.
        int playerDexterity = player.getAttributes().get(dexterity).getCurrent();
        int mobDexterity = mob.getAttributes().get(dexterity).getCurrent();
        if (mobDexterity > playerDexterity)
            hitChance -= 0.1; // If the mob is faster, reduce hit chance by 10%.
        
        std::cout << "\tHit chance: " << hitChance << std::endl;

        if (!math::randomChance(hitChance))
        {
            std::cout << "The player's spell missed!" << std::endl;
            return true;
        }

        // Determine spell cost and damage.
        int minSpellCost = 10;
        int maxSpellCost = 30;
        int remainingMana = player.getVitals().get(mana).getCurrent();
        int currentIntelleligence = player.getAttributes().get(intelligence).getCurrent();
        if (remainingMana < minSpellCost)
        {
            std::cout << "Not enough mana to cast spell!" << std::endl;
            return true;
        }
        int spellCost = math::randomInt(minSpellCost, maxSpellCost);
        if (remainingMana < spellCost)
            spellCost = remainingMana;
        int spellDamage = 2*spellCost + currentIntelleligence;
        
        std::cout << "The spell hits for " << spellDamage << " damage!" << std::endl;
        
        // Update player and mob values.
        player.getVitals().update(mana, remainingMana - spellCost);
        int currentMobHealth = mob.getVitals().get(health).getCurrent();
        mob.getVitals().update(health, currentMobHealth - spellDamage);
        
        if (currentMobHealth - spellDamage < 0)
        {
            std::cout << "The mob was killed!" << std::endl;
            return false;
        }

        return true;
    }


    void heal(rpg::Player& player)
    {
        std::cout << "Player casts heal!" << std::endl;
        
        int currentHealth = player.getVitals().get(health).getCurrent();
        int maxHealth = player.getVitals().get(health).getCurrentMax();
        int currentMana = player.getVitals().get(mana).getCurrent();
        int missingHealth = maxHealth - currentHealth;

        if (missingHealth == 0)
        { // Nothing to heal.
            std::cout << "The player has nothing to heal (already at full health)!" << std::endl;
            return;  
        }
        
        int healthToRestore = math::randomInt(1, missingHealth);
        
        if (currentMana == 0)
        {
            std::cout << "The player has no mana to use heal!" << std::endl;
            return;
        }
        
        if (currentMana < healthToRestore)
            healthToRestore = currentMana;

        // Update the appropriate values.
        std::cout << "The player heals for: " << healthToRestore 
                  << ", using " << healthToRestore << " mana!" << std::endl;
        player.getVitals().update(health, currentHealth + healthToRestore);
        player.getVitals().update(mana, currentMana - healthToRestore);
    }


    bool fleeMob(rpg::Player& player, rpg::Player& mob)
    {
        double fleeChance = 0.9; // Initial 90% chance to flee.
        int playerDexterity = player.getAttributes().get(dexterity).getCurrent();
        int mobDexterity = mob.getAttributes().get(dexterity).getCurrent();
        int playerHealth = player.getVitals().get(health).getCurrent();
        int mobHealth = mob.getVitals().get(health).getCurrent();
        
        if (mobDexterity > playerDexterity)
            fleeChance -= 0.2; // If the mob is faster, lower the flee chance by 20%.
            
        if (mobHealth > playerHealth)
            fleeChance -= 0.1; // If the mob has more health, lower the flee chance by 10%.
            
        if (math::randomChance(fleeChance))
        { // Flee success.
            std::cout << "Player was able to flee!" << std::endl;
            return false;
        }
        else
        { // Flee unsuccessful.  
            std::cout << "Player was unable to flee!" << std::endl;
            return true;
        }
    }


    rpg::Player createMob(rpg::Player& player)
    {
        rpg::Player mob;
        int playerMaxHealth = player.getVitals().get(health).getCurrentMax();
        int mobHealthMin = std::round(playerMaxHealth * 0.1);
        int mobHealthMax = std::round(playerMaxHealth * 1.5);
        int mobHealth = math::randomInt(mobHealthMin, mobHealthMax);
        mob.getVitals().add(health, mobHealth, 0, mobHealth);
        
        int playerStrength = player.getAttributes().get(strength).getCurrent();
        int mobStrength = math::randomInt(1, playerStrength);
        mob.getAttributes().add(strength, mobStrength);
        
        int playerDexterity = player.getAttributes().get(dexterity).getCurrent();
        int mobDexterity = math::randomInt(1, playerDexterity*2);
        mob.getAttributes().add(dexterity, mobDexterity);
        
        return mob;
    }

    
    void runFightSim(rpg::Player& player)
    {
        // Create a somewhat random mob to fight.
        rpg::Player mob = createMob(player);
        
        // Determine battle order.
        int playerDexterity = player.getAttributes().get(dexterity).getCurrent();
        int mobDexterity = mob.getAttributes().get(dexterity).getCurrent();
        bool mobAttacksFirst = mobDexterity > playerDexterity;

        // Fight the mob.
        bool fightOngoing = true;
        while (fightOngoing)
        {
            // Display mob and player status.
            displayMobStatus(mob);
            displayPlayerStatus(player);

            // Display menu.
            std::cout << "---------------------------------" << std::endl;
            displayFightSimOptions();

            int choice;
            std::cin >> choice;

            // Clear input buffer
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            // Separator between available actions and action being performed.
            std::cout << "---------------------------------" << std::endl;

            if (mobAttacksFirst)
            {
                std::cout << "The mob is faster than the player, it attacks first!" << std::endl;
                fightOngoing = mobAttacksPlayer(player, mob);
            }
            
            if (!fightOngoing)
            { // Ensure the mob did not kill the player.
                continue;
            }
            
            // Handle choice
            switch (choice)
            {
                case 1: fightOngoing = playerAttacksMob(player, mob);    break;
                case 2: fightOngoing = castSpellAgainstMob(player, mob); break;
                case 3: heal(player);                                    break;
                case 8:                                                  break;
                case 9: fightOngoing = fleeMob(player, mob);             break;
                default:
                    std::cout << "Invalid choice. Please enter a valid option." << std::endl;
                    break;
            }

            if (fightOngoing && !mobAttacksFirst)
            { // The mob attacks next if the fight is not over.
                std::cout << "The mob is slower than the player, it attacks second!" << std::endl;
                fightOngoing = mobAttacksPlayer(player, mob);
            }
        } // while (fightOngoing)

        std::cout << "The Fight ends." << std::endl;
    } // runFightSim()
    
    
    void fightMobManual(rpg::Player& player)
    {
        runFightSim(player);        
    } // fightMobManual()
    

    void lootTreasure(rpg::Player& player)
    {
        std::cout << "Player loots a treasure chest..." << std::endl;

        // Random coin amounts
        int copperAmount = math::randomInt(10, 100);
        int silverAmount = math::randomInt(1, 10);

        player.getWallet().add(copper, copperAmount);
        player.getWallet().add(silver, silverAmount);

        std::cout << "Player gains " << copperAmount << " Copper Coin" << (copperAmount > 1 ? "s" : "") << " and "
                  << silverAmount << " Silver Coin" << (silverAmount > 1 ? "s" : "") << "." << std::endl;
    }
    
    
    std::string restTypeToString(RestType type)
    {
        switch(type)
        {
            case RestType::longRest:  return "long rest";
            
            // Defaults to a short rest.
            case RestType::shortRest: 
            default:                  return "short rest";
        }
    }
    

    void rest(rpg::Player& player, RestType restType)
    {
        std::cout << "Player takes a "
                  << restTypeToString(restType)
                  << " to recover vitals..." << std::endl;

        int currentHealth = player.getVitals().get(health).getCurrent();
        int currentMana = player.getVitals().get(mana).getCurrent();
        int maxHealth = player.getVitals().get(health).getCurrentMax();
        int maxMana = player.getVitals().get(mana).getCurrentMax();

        int healthRestore = 0;
        int manaRestore = 0;
        
        if (restType == RestType::shortRest)
        { // short rest restores 20% of vitals.
            int missingHealth = maxHealth - currentHealth;
            int missingMana = maxMana - currentMana;
            
            healthRestore = missingHealth;
            manaRestore = missingMana;

            // Restore 20% or all of each vital (whichever is lower)
            if (missingHealth > maxHealth*0.2)
                healthRestore = maxHealth*0.2;
            if (missingMana > maxMana*0.2)
                manaRestore = maxMana*0.2;
        }
        else if (restType == RestType::longRest)
        { // long rest restores all vitals.
            healthRestore = maxHealth - currentHealth;
            manaRestore = maxMana - currentMana; 
        }

        currentHealth += healthRestore;
        currentMana += manaRestore;

        if (currentHealth > maxHealth) currentHealth = maxHealth;
        if (currentMana > maxMana) currentMana = maxMana;

        player.getVitals().update(health, currentHealth);
        player.getVitals().update(mana, currentMana);

        std::cout << "Recovered " << healthRestore << " health (now at " << currentHealth << ")." << std::endl;
        std::cout << "Recovered " << manaRestore << " mana (now at " << currentMana << ")." << std::endl;
    } // rest()
    

    void spendCurrency(rpg::Player& player)
    {
        std::cout << "Player spends currency at a vendor..." << std::endl;

        uint32_t copperOwned = player.getWallet().get(copper).getQuantity();
        uint32_t silverOwned = player.getWallet().get(silver).getQuantity();

        // Random spend amounts: Copper (5–50), Silver (1–5)
        uint32_t copperSpend = math::randomInt(5, 50);
        uint32_t silverSpend = math::randomInt(1, 5);

        bool spentAnything = false;

        if (copperOwned >= copperSpend)
        {
            player.getWallet().update(copper, copperOwned - copperSpend);
            std::cout << "Spent " << copperSpend << " Copper Coin" 
                      << (copperSpend > 1 ? "s" : "") << "." << std::endl;
            spentAnything = true;
        }
        else if (copperOwned > 0)
        {
            player.getWallet().update(copper, copperOwned - copperOwned);
            std::cout << "Only had " << copperOwned << " Copper Coin" 
                      << (copperOwned > 1 ? "s" : "") << ", all spent." << std::endl;
            spentAnything = true;
        }

        if (silverOwned >= silverSpend)
        {
            player.getWallet().update(silver, silverOwned - silverSpend);
            std::cout << "Spent " << silverSpend << " Silver Coin" 
                      << (silverSpend > 1 ? "s" : "") << "." << std::endl;
            spentAnything = true;
        }
        else if (silverOwned > 0)
        {
            player.getWallet().update(silver, silverOwned - silverOwned);
            std::cout << "Only had " << silverOwned << " Silver Coin" 
                      << (silverOwned > 1 ? "s" : "") << ", all spent." << std::endl;
            spentAnything = true;
        }

        if (!spentAnything)
        {
            std::cout << "Player has no currency to spend." << std::endl;
        }
    } // spendCurrency()
    
    
    void levelUp(rpg::Player& player)
    {
        int strIncrease = 1 + math::randomInt(0, 2);
        int dexIncrease = 1 + math::randomInt(0, 2);
        int intIncrease = 1 + math::randomInt(0, 2);
        int conIncrease = 1 + math::randomInt(0, 2);
        
        // Increment the current level.
        player.getProgress().get(level).add(1);

        // Get current attribute values.
        int currentStr = player.getAttributes().get(strength).getCurrent();
        int currentDex = player.getAttributes().get(dexterity).getCurrent();
        int currentInt = player.getAttributes().get(intelligence).getCurrent();
        int currentCon = player.getAttributes().get(constitution).getCurrent();

        // Update attribute values with new changes.
        player.getAttributes().update(strength, currentStr + strIncrease);
        player.getAttributes().update(dexterity, currentDex + dexIncrease);
        player.getAttributes().update(intelligence, currentInt + intIncrease);
        player.getAttributes().update(constitution, currentCon + conIncrease);

        // Print a status message on attribute changes.
        std::cout << "Leveled up! " << std::endl
                  << " - Strength increased by " << strIncrease
                  << " - Dexterity increased by " << dexIncrease
                  << " - Intelligence increased by " << intIncrease
                  << " - Constitution increased by " << conIncrease
                  << "." << std::endl;
          
        // Update cross-modifiers.
        helper_methods::updateModifiers(player);
    }
    
    
    bool isDead(rpg::Player& player)
    {
        if (player.getVitals().get(health).getCurrent() == 0)
            return true;
        return false;
    }
    
    
    void revive(rpg::Player& player)
    {
        if(isDead(player))
            player.getVitals().update(health, 1);
        else
            std::cout << "Player is not dead!" << std::endl;
    }
    

    void savePlayerData(rpg::Player& player, std::string& saveFile)
    {
        if (saveFile == "")
        {
            std::cout << "Could not Save game state. No file given." << std::endl;
            return;
        }
        
        player.saveToFile(saveFile);
        std::cout << "Saved game state." << std::endl;
    }
} // namespace rpg_sim
