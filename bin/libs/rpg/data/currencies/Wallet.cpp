/**
 * @file Wallet.cpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A container class for managing multiple currencies and their quantities.
 */

#include <sstream>
#include <limits>

#include "Wallet.hpp"
#include "CurrencyRegistry.hpp"
#include "RegistryHelper.hpp"


namespace currency
{
    namespace helper_methods 
    {
        /**
         * Resolves and retrieves a Currency object from the CurrencyRegistry based on a given identifier.
         *
         * This is a wrapper around the generic rpg::helper_methods::getFromRegistry, specialized
         * for the CurrencyRegistry and Currency types used within the stats namespace. It accepts an identifier
         * in the form of a name (std::string), ID (uint32_t), or a Currency object, and attempts to fetch
         * the corresponding Currency instance from the registry.
         *
         * @tparam T The type of identifier: std::string, uint32_t, or Currency.
         * @param identifier The identifier used to locate the Currency in the registry.
         * @return Pointer to the corresponding Currency object.
         * @throws MIA_THROW with Undefined_RPG_Value if the object is not found.
         */
        template<typename T>
        const Currency* getFromRegistry(const T& identifier)
        {
            return rpg::helper_methods::getFromRegistry<CurrencyRegistry, Currency, T>(identifier);
        }

    } // namespace helper_methods
    
    
    CurrencyQuantity& Wallet::get(const std::string& name)
    {
        const Currency* curr = helper_methods::getFromRegistry(name);
        return get(*curr);
    }
    CurrencyQuantity& Wallet::get(uint32_t id)
    {
        const Currency* curr = helper_methods::getFromRegistry(id);
        return get(*curr);
    }
    CurrencyQuantity& Wallet::get(const Currency& currency)
    {
        auto it = currencies.find(currency.getID());
        if (it == currencies.end())
        {
            // The data is not found so add a default one, then return the current.
            add(currency, 0);
            it = currencies.find(currency.getID());
            return it->second;
        }
        else 
            return it->second;
    }
    
    // add() methods...
    void Wallet::add(const std::string& name, uint32_t quantity)
    {
        const Currency* curr = helper_methods::getFromRegistry(name);
        add(*curr, quantity);
    }    
    void Wallet::add(uint32_t id, uint32_t quantity)
    {
        const Currency* curr = helper_methods::getFromRegistry(id);
        add(*curr, quantity);
    }
    void Wallet::add(const Currency& currency, uint32_t quantity)
    {
        auto it = currencies.find(currency.getID());
        if (it == currencies.end()) 
        {
            currencies.emplace(currency.getID(), CurrencyQuantity(quantity));
        }
        else
        {
            if (it->second.getQuantity() > std::numeric_limits<unsigned int>::max() - quantity) 
            {
                throw std::overflow_error("Quantity overflow for currency ID " + currency.getName());
            }
            unsigned int newQuantity = it->second.getQuantity() + quantity;
            it->second.set(newQuantity);
        }
    }    


    // remove() methods...
    void Wallet::remove(const std::string& name)
    {
        const Currency* curr = helper_methods::getFromRegistry(name);
        remove(*curr);
    }
    void Wallet::remove(uint32_t id)
    {
        const Currency* curr = helper_methods::getFromRegistry(id);
        remove(*curr);
    }
    void Wallet::remove(const Currency& currency)
    {
        auto it = currencies.find(currency.getID());
        if (it == currencies.end()) 
        {
            // It doesn't exist so no need to remove.
            return;
        }
        currencies.erase(it);
    }
    
    
    // update() methods...
    void Wallet::update(const std::string& name, unsigned int amount)
    {
        const Currency* curr = helper_methods::getFromRegistry(name);
        update(*curr, amount);
    }
    void Wallet::update(uint32_t id, unsigned int amount)
    {
        const Currency* curr = helper_methods::getFromRegistry(id);
        update(*curr, amount);
    }
    void Wallet::update(const Currency& currency, unsigned int amount)
    {
        auto it = currencies.find(currency.getID());
        if (it == currencies.end())
        {
            // The data is not found so add a default one, then return the current.
            add(currency, 0);
            it = currencies.find(currency.getID());
        }
        it->second.set(amount);
    }
      

    // has() methods...
    bool Wallet::has(const std::string& name, unsigned int quantity) const
    {
        const Currency* curr = helper_methods::getFromRegistry(name);
        return has(*curr, quantity);
    }
    bool Wallet::has(uint32_t id, unsigned int quantity) const
    {
        const Currency* curr = helper_methods::getFromRegistry(id);
        return has(*curr, quantity);
    }
    bool Wallet::has(const Currency& currency, unsigned int quantity) const
    {
        auto it = currencies.find(currency.getID());
        if (it == currencies.end())
        {
            // The data does not exist, so this is false.
            return false;
        }
        if (it->second.getQuantity() > quantity)
            return true;
        return false;
    }


    void Wallet::dump(std::ostream& os) const
    {
        for (const auto& [id, currencyContainer] : currencies) 
        {
            auto currency = CurrencyRegistry::getInstance().getById(id);
            os << "Currency: " << currency->getName()
               << ", Type: " << static_cast<int>(currency->getCurrencyType())
               << ", Quantity: " << currencyContainer.getQuantity()
               << ", Tradeable: " << currency->isTradeable()
               << "\n";
        }
    }

    
    std::string Wallet::serialize() const 
    {
        std::ostringstream oss;
        oss << "[WALLET_BEGIN]";
        for (const auto& [id, cq] : currencies) {
            oss << id << ':' << cq.getQuantity() << ';';
        }
        oss << "[WALLET_END]";
        return oss.str();
    }
    
    
    Wallet Wallet::deserialize(const std::string& data) 
    {
        size_t start = data.find("[WALLET_BEGIN]");
        size_t end = data.find("[WALLET_END]", start);

        if (start == std::string::npos || end == std::string::npos) 
        {
            throw std::invalid_argument("Wallet block not found.");
        }

        start += std::string("[WALLET_BEGIN]").length();
        std::string payload = data.substr(start, end - start);

        Wallet container;
        std::istringstream iss(payload);
        std::string token;

        while (std::getline(iss, token, ';')) 
        {
            if (token.empty()) continue;
            size_t sep = token.find(':');
            if (sep == std::string::npos) continue;

            uint32_t id = static_cast<uint32_t>(std::stoul(token.substr(0, sep)));
            uint32_t qty = static_cast<uint32_t>(std::stoul(token.substr(sep + 1)));

            const Currency* curr = CurrencyRegistry::getInstance().getById(id);
            if (curr) 
            {
                container.add(*curr, qty);
            }
        }

        return container;
    }

} // namespace currency
