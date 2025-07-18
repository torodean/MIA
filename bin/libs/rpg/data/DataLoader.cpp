/**
 * @file DataLoader.cpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A singleton class to manage loading of all game data registries from config files.
 */

#include <stdexcept>
#include "DataLoader.hpp"
#include "CurrencyRegistry.hpp"
#include "VitalRegistry.hpp"
#include "AttributeRegistry.hpp"

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
            // Load Registries
            currency::CurrencyRegistry::getInstance().loadFromFile(configDir + "currencies/currencies.json");
            stats::VitalRegistry::getInstance().loadFromFile(configDir + "stats/stats.json");
            stats::AttributeRegistry::getInstance().loadFromFile(configDir + "stats/stats.json");
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "Error loading Data Registries: " << e.what() << std::endl;
            success = false;
        }

        return success;
    }


    void DataLoader::dump(std::ostream& os) const
    {
        os << "=== DataLoader Contents ===\n";
        
        os << "CurrencyRegistry:\n";
        currency::CurrencyRegistry::getInstance().dump(os);
        
        os << "VitalRegistry:\n";
        stats::VitalRegistry::getInstance().dump(os);
        
        os << "AttributeRegistry:\n";
        stats::AttributeRegistry::getInstance().dump(os);
        
        os << "==========================\n";
    }

} // namespace rpg
