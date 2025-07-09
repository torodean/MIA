/**
 * File: DataLoader.cpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A singleton class to manage loading of all game data registries from config files.
 */

#include <stdexcept>
#include "DataLoader.hpp"
#include "CurrencyRegistry.hpp"
#include "Currencies.hpp"

namespace rpg
{

    DataLoader& DataLoader::getInstance()
    {
        static DataLoader instance;
        return instance;
    }


    bool DataLoader::initialize(const std::string& configDir)
    {
        bool success = true;
        try 
        {
            // Load CurrencyRegistry
            currency::CurrencyRegistry::getInstance().loadFromFile(configDir + "currencies/currencies.json");
            //currency::initializeCurrencies(); // Initialize predefined currencies
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "Error loading CurrencyRegistry: " << e.what() << std::endl;
            success = false;
        }

        // Add other registries here (e.g., ItemRegistry, QuestRegistry)
        // Example:
        // try {
        //     ItemRegistry::getInstance().loadFromFile(configDir + "items/items.json");
        //     std::cout << "Loaded ItemRegistry from " << configDir + "items/items.json" << std::endl;
        // } catch (const std::exception& e) {
        //     std::cerr << "Error loading ItemRegistry: " << e.what() << std::endl;
        //     success = false;
        // }

        return success;
    }


    void DataLoader::dump(std::ostream& os) const
    {
        os << "=== DataLoader Contents ===\n";
        os << "CurrencyRegistry:\n";
        currency::CurrencyRegistry::getInstance().dump(os);
        // Add other registries here
        os << "==========================\n";
    }

} // namespace rpg
