/**
 * @file RPGSimulator.cpp
 * @author Antonius Torode
 * @date 07/10/2025
 * Description: Implementation of a terminal-based RPG simulator that lists and executes player actions.
 */

#include <iostream>
#include <limits>
#include <cmath>   // for round()
#include <sstream>

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


    namespace
    {
        /// The runtime context used to log simulator diagnostics. This is set by
        /// setRuntimeContext() and is null until then.
        const RuntimeContext* runtimeContext{nullptr};

        void updateModifiers(rpg::Player& player)
        {
            rpg::helper_methods::applyModifiers
                <stats::AttributeRegistry, stats::VitalRegistry,
                 stats::Attributes, stats::Vitals>
                 (attributeRegistry, vitalRegistry, player.getAttributes(), player.getVitals());
        }

        /**
         * Logs a simulator message through the runtime context's logger. The message
         * is always written to the log file. It is printed to stdout when the context's
         * verbose mode is enabled, or when printToCoutOverride is true. Does nothing
         * when no runtime context has been set.
         * @param message The message to log.
         * @param printToCoutOverride Whether to print the message to stdout regardless
         *        of the verbose mode.
         */
        void log(const std::string& message, bool printToCoutOverride)
        {
            if (runtimeContext)
                runtimeContext->logger.log(message, runtimeContext->verboseMode || printToCoutOverride);
        }

        /**
         * Builds a message from a sequence of streamable values and logs it with log().
         * The values are inserted into a string stream in order, so the message
         * formatting matches an equivalent std::cout statement.
         * @param printToCoutOverride Whether to print the message to stdout regardless
         *        of the verbose mode.
         * @param args The values to insert into the message.
         */
        template <typename... Args>
        void logStream(bool printToCoutOverride, Args&&... args)
        {
            std::ostringstream oss;
            (oss << ... << std::forward<Args>(args));
            log(oss.str(), printToCoutOverride);
        }
    } // namespace


    void setRuntimeContext(const RuntimeContext& context)
    {
        runtimeContext = &context;
    }


    void setupSimulator(rpg::Player& player)
    {
        log("Creating default simulator values!", true);
        int initialHealth = vitalRegistry.getByName(health)->getBaseMax();
        int minHealth = vitalRegistry.getByName(health)->getBaseMin();
        int maxHealth = initialHealth;
        int initialMana = vitalRegistry.getByName(mana)->getBaseMax();
        int minMana = vitalRegistry.getByName(mana)->getBaseMin();
        int maxMana = initialMana;

        // Initialize vitals.
        player.getVitals().add(health, initialHealth, minHealth, maxHealth);
        player.getVitals().add(mana, initialMana, minMana, maxMana);
        logStream(true, "Vitals initialized: Health=", initialHealth,
                        ", Mana=", initialMana, ".");

        // Initialize wallet.
        player.getWallet().add(copper, 100);
        player.getWallet().add(silver, 10);
        logStream(true, "Wallet initialized: Copper=100, Silver=10.");

        // Initialize attributes.
        player.getAttributes().add(strength, 1);
        player.getAttributes().add(dexterity, 1);
        player.getAttributes().add(constitution, 1);
        player.getAttributes().add(intelligence, 1);
        logStream(true, "Attributes initialized: Strength=1, Constitution=1, Intelligence=1.");

        // Initialize progress markers.
        player.getProgress().add(level, 1);
        player.getProgress().add(xp, 0);

        // Apply stat cross-modifiers.
        updateModifiers(player);
    }


    void displayPlayerStatus(rpg::Player& player)
    {
        logStream(true, "Player Status:\n"
                        "\tLevel: ", player.getProgress().get(level).get(),
                        "\n\tXP: ", player.getProgress().get(xp).get(),
                        "\n\tHealth: ", player.getVitals().get(health).getCurrent(),
                        "/", player.getVitals().get(health).getCurrentMax(),
                        "\n\tMana: ", player.getVitals().get(mana).getCurrent(),
                        "/", player.getVitals().get(mana).getCurrentMax(),
                        "\n\tSilver: ", player.getWallet().get(silver).getQuantity(),
                        "\n\tCopper: ", player.getWallet().get(copper).getQuantity(),
                        "\n\tStrength: ", player.getAttributes().get(strength).getCurrent(),
                        "\n\tDexterity: ", player.getAttributes().get(dexterity).getCurrent(),
                        "\n\tConstitution: ", player.getAttributes().get(constitution).getCurrent(),
                        "\n\tIntelligence: ", player.getAttributes().get(intelligence).getCurrent());
    }


    void displaySimulatorOptions()
    {
        logStream(true, "Available Actions:\n"
                        "0. Revive\n"
                        "1. Level up (automated)\n"
                        "2. Loot Treasure (automated)\n"
                        "5. Spend Currency (automated)\n"
                        "20. Fight a Mob (automated)\n"
                        "21. Fight a Mob (manual)\n"
                        "70. Short Rest (20% recovery)\n"
                        "71. Long Rest (100% recovery)\n"
                        "98. Save Game State\n"
                        "99. Exit");
        std::cout << "Enter integer choice: ";
    }


    void runSimulator(rpg::Player& player, std::string saveFile)
    {
        while (true)
        {
            // Display player status.
            displayPlayerStatus(player);

            // Display menu.
            logStream(true, "=================================");
            displaySimulatorOptions();

            int choice;
            std::cin >> choice;

            // Clear input buffer
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Separator between available actions and action being performed.
            logStream(true, "=================================");

            if (isDead(player) && choice != 0)
            {
                logStream(true, "Player is dead, must revive before continuing!");
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
                    logStream(true, "Exiting simulator.");
                    return;
                default:
                    logStream(true, "Invalid choice. Please enter a valid option.");
            }
        }
    }


    void fightMobAutomated(rpg::Player& player)
    {
        int currentHealth = player.getVitals().get(health).getCurrent();
        int currentMana = player.getVitals().get(mana).getCurrent();

        logStream(true, "Player encounters a hostile mob...");

        // Loop the rounds until the fight has a reason to end.
        int round = 0;
        /*
         * Any case that would end the fight uses a break in order to immediately stop the loop
         * rather than looping back around again.
         */
        while (true)
        {
            round++;
            logStream(true, "\n-- Round ", round, " --");

            int mobDamage = 0;
            if (math::randomChance(0.1))
            { // Random chance (10%) the mob misses an attack.
                logStream(true, "The mobs attack missed!");
            }
            else
            {
                mobDamage = math::randomInt(5, 20);
                logStream(true, "Mob attacks! Player takes ", mobDamage, " damage.");
            }

            if (player.getVitals().has(health, mobDamage))
            {
                currentHealth -= mobDamage;
                player.getVitals().update(health, currentHealth);
                logStream(true, "Player survives with ", currentHealth, " health.");
            }
            else
            {
                player.getVitals().update(health, 0);
                logStream(true, "Player takes lethal damage and dies.");
                break; // break the fight immediately.
            }

            // 50% chance to cast a spell.
            if (math::randomChance(0.5))
            {
                int spellCost = math::randomInt(10, 25);
                logStream(true, "Player attempts to cast a spell (cost ", spellCost, " mana).");

                if (player.getVitals().has(mana, spellCost))
                {
                    currentMana -= spellCost;
                    player.getVitals().update(mana, currentMana);
                    logStream(true, "Spell cast successfully. Remaining mana: ", currentMana, ".");

                    logStream(true, "The spell hits! Mob is damaged!");
                    if (math::randomChance(0.5))
                    { // 50% chance the spell kills the mob.
                        logStream(true, "Mob is defeated!");
                        player.getProgress().get(xp).add(25); // Gain 25 xp from auto kills.
                        logStream(true, "The player gains 25 xp!");
                        break;
                    }
                    else
                    {
                        logStream(true, "Mob is still standing!");
                    }
                }
                else
                {
                    logStream(true, "Not enough mana to cast the spell.");
                    logStream(true, "The player attempts to flee!");
                    if (math::randomChance(0.5))
                    { // 50% chance the player can flee the mob.
                        logStream(true, "The player successfully flees!");
                        break;
                    }
                    else
                    {
                        logStream(true, "The player was unable to flee!");
                    }
                }
            }
            else
            {
                logStream(true, "Player was unable to cast a spell. Mob moving too fast!");
            }

            // Random chance (10%) mob flees after any round
            if (math::randomChance(0.1))
            {
                logStream(true, "The mob suddenly flees!");
                break;
            }
        } // while (true)

        logStream(true, "The Fight ends.");
    } // fightMobAutomated()


    void displayMobStatus(rpg::Player& mob)
    {
        logStream(true, "Mob Status:\n"
                        "\t Health: ", mob.getVitals().get(health).getCurrent(),
                        "/", mob.getVitals().get(health).getCurrentMax(),
                        "\n\t Strength: ", mob.getAttributes().get(strength).getCurrent(),
                        "\n\t Dexterity: ", mob.getAttributes().get(dexterity).getCurrent());
    }


    void displayFightSimOptions()
    {
        logStream(true, "Available Actions:\n"
                        "1. Attack\n"
                        "2. Cast Spell\n"
                        "3. Heal\n"
                        "8. Do nothing\n"
                        "9. Flee");
        std::cout << "Enter integer choice: ";
    }


    bool playerAttacksMob(rpg::Player& player, rpg::Player& mob)
    {
        logStream(true, "The player attacks the mob!");

        // Determine if the attack hits.
        double hitChance = 0.95; // 95% hit chance.
        int playerDexterity = player.getAttributes().get(dexterity).getCurrent();
        int mobDexterity = mob.getAttributes().get(dexterity).getCurrent();
        if (mobDexterity > playerDexterity)
            hitChance -= 0.1; // If the mob is faster, reduce hit chance by 10%.

        logStream(true, "\tHit chance: ", hitChance);

        if (!math::randomChance(hitChance))
        {
            logStream(true, "The player's attack missed!");
            return true;
        }

        // Determine the attack damage.
        int attackDamage = math::randomInt(5, 20); // Base damage.
        int playerStrength = player.getAttributes().get(strength).getCurrent();
        attackDamage += playerStrength; // Add strength as attack damage.

        logStream(true, "Attacking the mob for ", attackDamage, " damage!");

        // Update the mob health.
        int mobHealth = mob.getVitals().get(health).getCurrent();
        mob.getVitals().update(health, mobHealth - attackDamage);

        // Determine if the fight should continue or not.
        if (mobHealth - attackDamage <= 0)
        {
            logStream(true, "The mob has died!");
            // Gain xp equal to the max health of the mob killed.
            int xpGained = mob.getVitals().get(health).getCurrentMax();
            player.getProgress().get(xp).add(xpGained);
            logStream(true, "The player gains ", xpGained, " xp!");
            return false;
        }

        return true;
    } // playerAttacksMob()


    bool mobAttacksPlayer(rpg::Player& player, rpg::Player& mob)
    {
        logStream(true, "The mob attacks the player!");

        // Determine if the attack hits.
        double hitChance = 0.95; // 95% hit chance.
        int playerDexterity = player.getAttributes().get(dexterity).getCurrent();
        int mobDexterity = mob.getAttributes().get(dexterity).getCurrent();

        if (playerDexterity > mobDexterity)
            hitChance -= 0.1; // If the player is faster, reduce the mob's hit chance by 10%.

        logStream(true, "\tHit chance: ", hitChance);

        if (!math::randomChance(hitChance))
        {
            logStream(true, "The mob's attack missed!");
            return true;
        }

        // Determine the attack damage.
        int attackDamage = math::randomInt(5, 20); // Base damage.
        int mobStrength = mob.getAttributes().get(strength).getCurrent();
        attackDamage += mobStrength;

        logStream(true, "The mob hits the player for ", attackDamage, " damage!");

        // Update the player's health.
        int playerHealth = player.getVitals().get(health).getCurrent();
        int remainingHealth = playerHealth - attackDamage;

        if (remainingHealth <= 0)
        {
            player.getVitals().update(health, 0);

            logStream(true, "The player takes lethal damage and dies!");
            return false;
        }

        player.getVitals().update(health, remainingHealth);
        logStream(true, "Player survives with ", remainingHealth, " health.");

        return true;
    } // mobAttacksPlayer()


    bool castSpellAgainstMob(rpg::Player& player, rpg::Player& mob)
    {
        logStream(true, "The player casts a spell at the mob!");

        // Determine if the spell hits.
        double hitChance = 0.75; // 75% hit chance.
        int playerDexterity = player.getAttributes().get(dexterity).getCurrent();
        int mobDexterity = mob.getAttributes().get(dexterity).getCurrent();
        if (mobDexterity > playerDexterity)
            hitChance -= 0.1; // If the mob is faster, reduce hit chance by 10%.

        logStream(true, "\tHit chance: ", hitChance);

        if (!math::randomChance(hitChance))
        {
            logStream(true, "The player's spell missed!");
            return true;
        }

        // Determine spell cost and damage.
        int minSpellCost = 10;
        int maxSpellCost = 30;
        int remainingMana = player.getVitals().get(mana).getCurrent();
        int currentIntelleligence = player.getAttributes().get(intelligence).getCurrent();
        if (remainingMana < minSpellCost)
        {
            logStream(true, "Not enough mana to cast spell!");
            return true;
        }
        int spellCost = math::randomInt(minSpellCost, maxSpellCost);
        if (remainingMana < spellCost)
            spellCost = remainingMana;
        int spellDamage = 2*spellCost + currentIntelleligence;

        logStream(true, "The spell hits for ", spellDamage, " damage!");

        // Update player and mob values.
        player.getVitals().update(mana, remainingMana - spellCost);
        int currentMobHealth = mob.getVitals().get(health).getCurrent();
        mob.getVitals().update(health, currentMobHealth - spellDamage);

        if (currentMobHealth - spellDamage < 0)
        {
            logStream(true, "The mob was killed!");
            return false;
        }

        return true;
    }


    void heal(rpg::Player& player)
    {
        logStream(true, "Player casts heal!");

        int currentHealth = player.getVitals().get(health).getCurrent();
        int maxHealth = player.getVitals().get(health).getCurrentMax();
        int currentMana = player.getVitals().get(mana).getCurrent();
        int missingHealth = maxHealth - currentHealth;

        if (missingHealth == 0)
        { // Nothing to heal.
            logStream(true, "The player has nothing to heal (already at full health)!");
            return;
        }

        int healthToRestore = math::randomInt(1, missingHealth);

        if (currentMana == 0)
        {
            logStream(true, "The player has no mana to use heal!");
            return;
        }

        if (currentMana < healthToRestore)
            healthToRestore = currentMana;

        // Update the appropriate values.
        logStream(true, "The player heals for: ", healthToRestore,
                        ", using ", healthToRestore, " mana!");
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
            logStream(true, "Player was able to flee!");
            return false;
        }
        else
        { // Flee unsuccessful.
            logStream(true, "Player was unable to flee!");
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
            logStream(true, "---------------------------------");
            displayFightSimOptions();

            int choice;
            std::cin >> choice;

            // Clear input buffer
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Separator between available actions and action being performed.
            logStream(true, "---------------------------------");

            if (mobAttacksFirst)
            {
                logStream(true, "The mob is faster than the player, it attacks first!");
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
                    logStream(true, "Invalid choice. Please enter a valid option.");
                    break;
            }

            if (fightOngoing && !mobAttacksFirst)
            { // The mob attacks next if the fight is not over.
                logStream(true, "The mob is slower than the player, it attacks second!");
                fightOngoing = mobAttacksPlayer(player, mob);
            }
        } // while (fightOngoing)

        logStream(true, "The Fight ends.");
    } // runFightSim()


    void fightMobManual(rpg::Player& player)
    {
        runFightSim(player);
    } // fightMobManual()


    void lootTreasure(rpg::Player& player)
    {
        logStream(true, "Player loots a treasure chest...");

        // Random coin amounts
        int copperAmount = math::randomInt(10, 100);
        int silverAmount = math::randomInt(1, 10);

        player.getWallet().add(copper, copperAmount);
        player.getWallet().add(silver, silverAmount);

        logStream(true, "Player gains ", copperAmount, " Copper Coin",
                        (copperAmount > 1 ? "s" : ""), " and ",
                        silverAmount, " Silver Coin",
                        (silverAmount > 1 ? "s" : ""), ".");
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
        logStream(true, "Player takes a ",
                        restTypeToString(restType),
                        " to recover vitals...");

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

        logStream(true, "Recovered ", healthRestore, " health (now at ", currentHealth, ").");
        logStream(true, "Recovered ", manaRestore, " mana (now at ", currentMana, ").");
    } // rest()


    void spendCurrency(rpg::Player& player)
    {
        logStream(true, "Player spends currency at a vendor...");

        uint32_t copperOwned = player.getWallet().get(copper).getQuantity();
        uint32_t silverOwned = player.getWallet().get(silver).getQuantity();

        // Random spend amounts: Copper (5–50), Silver (1–5)
        uint32_t copperSpend = math::randomInt(5, 50);
        uint32_t silverSpend = math::randomInt(1, 5);

        bool spentAnything = false;

        if (copperOwned >= copperSpend)
        {
            player.getWallet().update(copper, copperOwned - copperSpend);
            logStream(true, "Spent ", copperSpend, " Copper Coin",
                            (copperSpend > 1 ? "s" : ""), ".");
            spentAnything = true;
        }
        else if (copperOwned > 0)
        {
            player.getWallet().update(copper, copperOwned - copperOwned);
            logStream(true, "Only had ", copperOwned, " Copper Coin",
                            (copperOwned > 1 ? "s" : ""), ", all spent.");
            spentAnything = true;
        }

        if (silverOwned >= silverSpend)
        {
            player.getWallet().update(silver, silverOwned - silverSpend);
            logStream(true, "Spent ", silverSpend, " Silver Coin",
                            (silverSpend > 1 ? "s" : ""), ".");
            spentAnything = true;
        }
        else if (silverOwned > 0)
        {
            player.getWallet().update(silver, silverOwned - silverOwned);
            logStream(true, "Only had ", silverOwned, " Silver Coin",
                            (silverOwned > 1 ? "s" : ""), ", all spent.");
            spentAnything = true;
        }

        if (!spentAnything)
        {
            logStream(true, "Player has no currency to spend.");
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

        // Log a status message on attribute changes.
        logStream(true, "Leveled up! \n",
                        " - Strength increased by ", strIncrease,
                        " - Dexterity increased by ", dexIncrease,
                        " - Intelligence increased by ", intIncrease,
                        " - Constitution increased by ", conIncrease,
                        ".");

        // Update cross-modifiers.
        updateModifiers(player);
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
            logStream(true, "Player is not dead!");
    }


    void savePlayerData(rpg::Player& player, std::string& saveFile)
    {
        if (saveFile == "")
        {
            logStream(true, "Could not Save game state. No file given.");
            return;
        }

        player.saveToFile(saveFile);
        logStream(true, "Saved game state.");
    }
} // namespace rpg_sim
