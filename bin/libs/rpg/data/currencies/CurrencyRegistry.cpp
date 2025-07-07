/**
 * File: CurrencyRegistry.cpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A singleton class to manage static Currency objects loaded from a config file.
 */

#include <stdexcept>
#include "nlohmann/json.hpp"
#include "CurrencyRegistry.hpp"

namespace currency
{
    CurrencyRegistry& CurrencyRegistry::getInstance()
    {
        static CurrencyRegistry instance;
        return instance;
    }
    
    
    void CurrencyRegistry::loadFromFile(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open config file: " + filename);
        }

        nlohmann::json json;
        file >> json;
        file.close();

        for (const auto& item : json["currencies"]) 
        {
            std::string name = item.value("name", "");
            std::string description = item.value("description", "");
            std::string typeStr = item.value("type", "UNKNOWN");
            bool tradeable = item.value("tradeable", true);
            std::string icon = item.value("icon", "");

            // Map string type to currencyType enum
            currencyType type = stringToCurrencyType(typeStr);

            // Create and store Currency object
            Currency currency(name, description, type, tradeable, icon);
            currencies.emplace(currency.getID(), currency);
            nameToId.emplace(name, currency.getID());
        }
    }


    const Currency* CurrencyRegistry::getCurrencyById(uint32_t id) const
    {
        auto it = currencies.find(id);
        return it != currencies.end() ? &it->second : nullptr;
    }


    const Currency* CurrencyRegistry::getCurrencyByName(const std::string& name) const
    {
        auto it = nameToId.find(name);
        if (it == nameToId.end()) 
        {
            return nullptr;
        }
        return getCurrencyById(it->second);
    }


    void CurrencyRegistry::dump(std::ostream& os) const
    {
        for (const auto& [id, currency] : currencies) 
        {
            os << "Currency: " << currency.getName()
               << ", Type: " << static_cast<int>(currency.getCurrencyType())
               << ", Tradeable: " << currency.isTradeable()
               << ", Icon: " << currency.getIconArt()
               << "\n";
        }
    }

} // namespace currency
