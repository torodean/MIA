/**
 * @file RPGSimulator.cpp
 * @author Antonius Torode
 * @date 07/10/2025
 * Description: Implementation of a terminal-based RPG simulator that lists and executes player actions.
 */

#include <iostream>
#include <limits>
#include <cstdlib> // for rand()
#include <ctime>   // for time()

#include "RPGSimulator.hpp"
#include "Vitals.hpp"
#include "Wallet.hpp"
#include "Attributes.hpp"
#include "ModifierApplicator.hpp"

namespace rpg_sim
{
    currency::CurrencyRegistry& currencyRegistry = currency::CurrencyRegistry::getInstance();
    stats::VitalRegistry& vitalRegistry = stats::VitalRegistry::getInstance();
    stats::AttributeRegistry& attributeRegistry = stats::AttributeRegistry::getInstance();
    
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
    
    const std::string healthName = "Health";
    const std::string manaName = "Mana";
    const std::string copper = "Copper Coin";
    const std::string silver = "Silver Coin";
    const std::string constitutionName = "Constitution";
    const std::string intelligenceName = "Intelligence";

    void setupSimulator(rpg::Player& player)
    {
        int initialHealth = vitalRegistry.getByName(healthName)->getBaseMax();
        int minHealth = vitalRegistry.getByName(healthName)->getBaseMin();
        int maxHealth = initialHealth;
        int initialMana = vitalRegistry.getByName(manaName)->getBaseMax();
        int minMana = vitalRegistry.getByName(manaName)->getBaseMin();
        int maxMana = initialMana;

        // Initialize vitals.
        player.getVitals().add(healthName, initialHealth, minHealth, maxHealth);
        player.getVitals().add(manaName, initialMana, minMana, maxMana);
        std::cout << "Vitals initialized: " << initialHealth << " Health, " << initialMana << " Mana.\n";

        // Initialize wallet.
        player.getWallet().add(copper, 100);
        player.getWallet().add(silver, 10);
        std::cout << "Wallet initialized: 100 Copper Coins, 10 Silver Coins.\n";
        
        // Initialize attributes.
        player.getAttributes().add(constitutionName, 2);
        player.getAttributes().add(intelligenceName, 2);
        std::cout << "Attributes initialized: 2 constitution, 2 intelligence.\n";
        
        // Apply stat cross-modifiers.
        helper_methods::updateModifiers(player);
    }

    void runSimulator(rpg::Player& player, std::string saveFile)
    {
        while (true)
        {
            // Display player status
            std::cout << "\nPlayer Status:" << std::endl
                      << "\t Health: " << player.getVitals().get(healthName).getCurrent() << "/"
                                       << player.getVitals().get(healthName).getCurrentMax() << std::endl
                      << "\t Mana: " << player.getVitals().get(manaName).getCurrent() << "/"
                                       << player.getVitals().get(manaName).getCurrentMax()<< std::endl
                      << "\t Silver: " << player.getWallet().get(silver).getQuantity() << std::endl
                      << "\t Copper: " << player.getWallet().get(copper).getQuantity() << std::endl
                      << "\t Constitution: " << player.getAttributes().get(constitutionName).getCurrent() << std::endl
                      << "\t Intelligence: " << player.getAttributes().get(intelligenceName).getCurrent() << std::endl;


            // Display menu
            std::cout << "\nAvailable Actions:\n"
                      << "1. Fight a Mob\n"
                      << "2. Loot Treasure\n"
                      << "3. Rest\n"
                      << "4. Spend Currency\n"
                      << "5. Level up\n"
                      << "98. Save Game State\n"
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
                    fightMob(player);
                    break;
                case 2:
                    lootTreasure(player);
                    break;
                case 3:
                    rest(player);
                    break;
                case 4:
                    spendCurrency(player);
                    break;
                case 5:
                    levelUp(player);
                    break;
                case 98:
                    savePlayerData(player, saveFile);
                    break;
                case 99:
                    std::cout << "Exiting simulator.\n";
                    return;
                default:
                    std::cout << "Invalid choice. Please enter a number between 1 and 9.\n";
            }
        }
    }

    void fightMob(rpg::Player& player)
    {
        static bool seeded = false;
        if (!seeded) {
            std::srand(std::time(0));
            seeded = true;
        }

        int currentHealth = player.getVitals().get(healthName).getCurrent();
        int currentMana = player.getVitals().get(manaName).getCurrent();

        std::cout << "Player encounters a hostile mob...\n";

        // Number of attack rounds (1–3)
        int rounds = 1 + (std::rand() % 3);
        for (int i = 1; i <= rounds; ++i)
        {
            std::cout << "\n-- Round " << i << " --\n";

            int mobDamage = 5 + (std::rand() % 16); // 5–20
            std::cout << "Mob attacks! Player takes " << mobDamage << " damage.\n";

            if (player.getVitals().has(healthName, mobDamage)) 
            {
                currentHealth -= mobDamage;
                player.getVitals().update(healthName, stats::VitalDataTarget::CURRENT, currentHealth);
                std::cout << "Player survives with " << currentHealth << " health.\n";
            } 
            else 
            {
                player.getVitals().update(healthName, stats::VitalDataTarget::CURRENT, 0);
                std::cout << "Player takes lethal damage and dies.\n";
                return;
            }

            // 50% chance to cast a spell
            if ((std::rand() % 2) == 0) 
            {
                int spellCost = 10 + (std::rand() % 16); // 10–25
                std::cout << "Player attempts to cast a spell (cost " << spellCost << " mana).\n";

                if (player.getVitals().has(manaName, spellCost)) 
                {
                    currentMana -= spellCost;
                    player.getVitals().update(manaName, stats::VitalDataTarget::CURRENT, currentMana);
                    std::cout << "Spell cast successfully. Remaining mana: " << currentMana << ".\n";

                    std::cout << "The spell hits! Mob is damaged severely.\n";
                    if ((std::rand() % 2) == 0)
                    {
                        std::cout << "Mob is defeated!\n";
                        break;
                    }
                    else
                    {
                        std::cout << "Mob is still standing!\n";
                    }
                } 
                else 
                {
                    std::cout << "Not enough mana to cast the spell.\n";
                }
            } 
            else 
            {
                std::cout << "Player chooses not to cast a spell.\n";
            }

            // Random chance mob flees after any round
            if ((std::rand() % 4) == 0)
            {
                std::cout << "The mob suddenly flees!\n";
                break;
            }
        }

        std::cout << "\nFight ends.\n";
    }

    void lootTreasure(rpg::Player& player)
    {
        std::cout << "Player loots a treasure chest...\n";

        // Random coin amounts
        int copperAmount = 10 + std::rand() % 91; // 10–100
        int silverAmount = 1 + std::rand() % 10;  // 1–10

        player.getWallet().add(copper, copperAmount);
        player.getWallet().add(silver, silverAmount);

        std::cout << "Player gains " << copperAmount << " Copper Coin" << (copperAmount > 1 ? "s" : "") << " and "
                  << silverAmount << " Silver Coin" << (silverAmount > 1 ? "s" : "") << ".\n";
    }

    void rest(rpg::Player& player)
    {
        std::cout << "Player rests to recover vitals...\n";

        int currentHealth = player.getVitals().get(healthName).getCurrent();
        int currentMana   = player.getVitals().get(manaName).getCurrent();
        int maxHealth     = player.getVitals().get(healthName).getCurrentMax();
        int maxMana       = player.getVitals().get(manaName).getCurrentMax();

        int healthDelta = maxHealth - currentHealth;
        int manaDelta = maxMana - currentMana;

        int healthRestore = (healthDelta > 0) ? (5 + std::rand() % healthDelta) : 0;
        int manaRestore   = (manaDelta > 0)   ? (5 + std::rand() % manaDelta)   : 0;

        currentHealth += healthRestore;
        currentMana += manaRestore;

        if (currentHealth > maxHealth) currentHealth = maxHealth;
        if (currentMana > maxMana) currentMana = maxMana;

        player.getVitals().update(healthName, stats::VitalDataTarget::CURRENT, currentHealth);
        player.getVitals().update(manaName, stats::VitalDataTarget::CURRENT, currentMana);

        std::cout << "Recovered " << healthRestore << " health (now at " << currentHealth << ").\n";
        std::cout << "Recovered " << manaRestore << " mana (now at " << currentMana << ").\n";
    }

    void spendCurrency(rpg::Player& player)
    {
        std::cout << "Player spends currency at a vendor...\n";

        uint32_t copperOwned = player.getWallet().get(copper).getQuantity();
        uint32_t silverOwned = player.getWallet().get(silver).getQuantity();

        // Random spend amounts: Copper (5–50), Silver (1–5)
        uint32_t copperSpend = 5 + (std::rand() % 46);
        uint32_t silverSpend = 1 + (std::rand() % 5);

        bool spentAnything = false;

        if (copperOwned >= copperSpend)
        {
            player.getWallet().update(copper, copperOwned - copperSpend);
            std::cout << "Spent " << copperSpend << " Copper Coin" << (copperSpend > 1 ? "s" : "") << ".\n";
            spentAnything = true;
        }
        else if (copperOwned > 0)
        {
            player.getWallet().update(copper, copperOwned - copperOwned);
            std::cout << "Only had " << copperOwned << " Copper Coin" << (copperOwned > 1 ? "s" : "") << ", all spent.\n";
            spentAnything = true;
        }

        if (silverOwned >= silverSpend)
        {
            player.getWallet().update(silver, silverOwned - silverSpend);
            std::cout << "Spent " << silverSpend << " Silver Coin" << (silverSpend > 1 ? "s" : "") << ".\n";
            spentAnything = true;
        }
        else if (silverOwned > 0)
        {
            player.getWallet().update(silver, silverOwned - silverOwned);
            std::cout << "Only had " << silverOwned << " Silver Coin" << (silverOwned > 1 ? "s" : "") << ", all spent.\n";
            spentAnything = true;
        }

        if (!spentAnything)
        {
            std::cout << "Player has no currency to spend.\n";
        }
    }
    
    void levelUp(rpg::Player& player)
    {
        int intIncrease = 1 + (std::rand() % 3);         // +1-3
        int conIncrease = 1 + (std::rand() % 3);         // +1-3

        int currentInt = player.getAttributes().get(intelligenceName).getCurrent();
        int currentCon = player.getAttributes().get(constitutionName).getCurrent();

        player.getAttributes().update(intelligenceName, currentInt + intIncrease);
        player.getAttributes().update(constitutionName, currentCon + conIncrease);

        std::cout << "Leveled up! Intelligence increased by " << intIncrease
                  << ", Constitution increased by " << conIncrease << ".\n";
          
        // Update cross-modifiers.
        helper_methods::updateModifiers(player);
    }

    void savePlayerData(rpg::Player& player, std::string& saveFile)
    {
        if (saveFile == "")
        {
            std::cout << "Could not Save game state. No file given.\n";
            return;
        }
        
        player.saveToFile(saveFile);
        std::cout << "Saved game state.\n";
    }
} // namespace rpg_sim

