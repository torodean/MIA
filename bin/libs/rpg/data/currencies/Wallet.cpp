/**
 * @file Wallet.cpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A container class for managing multiple currencies and their quantities.
 */

#include <sstream>
#include "Wallet.hpp"
#include "CurrencyRegistry.hpp"


namespace currency
{

    void Wallet::add(const Currency& currency, uint32_t quantity)
    {
        auto id = currency.getID();
        auto it = currencies.find(id);
        if (it == currencies.end()) {
            currencies.emplace(id, CurrencyQuantity(currency, quantity));
        } 
        else 
        {
            if (it->second.quantity > UINT32_MAX - quantity) 
            {
                throw std::overflow_error("Quantity overflow for currency ID " + std::to_string(id));
            }
            it->second.quantity += quantity;
        }
    }    
    
    void Wallet::add(const std::string& name, uint32_t quantity)
    {
        const currency::Currency* curr = currency::CurrencyRegistry::getInstance().getByName(name);
        add(*curr, quantity);
    }
    
    void Wallet::add(uint32_t id, uint32_t quantity)
    {
        auto it = currencies.find(id);
        if (it != currencies.end()) 
        {
            it->second.quantity += quantity;
        } 
        else 
        {
            const currency::Currency* curr = currency::CurrencyRegistry::getInstance().getById(id);
            if (!curr) 
            {
                throw std::invalid_argument("Currency ID " + std::to_string(id) + " not found in registry.");
            }
            add(*curr, quantity);
        }
    }


    bool Wallet::remove(const Currency& currency, uint32_t quantity)
    {
        auto id = currency.getID();
        auto it = currencies.find(id);
        if (it == currencies.end() || it->second.quantity < quantity) 
        {
            return false;
        }
        it->second.quantity -= quantity;
        if (it->second.quantity == 0) 
        {
            currencies.erase(it);
        }
        return true;
    }

    bool Wallet::remove(const std::string& name, uint32_t quantity)
    {
        const currency::Currency* curr = currency::CurrencyRegistry::getInstance().getByName(name);
        if (!curr) 
        {
            throw std::invalid_argument("Currency name " + name + " not found in registry.");
        }
        return remove(*curr, quantity);
    }
    
    bool Wallet::remove(uint32_t id, uint32_t quantity)
    {
        auto it = currencies.find(id);
        if (it == currencies.end() || it->second.quantity < quantity)
            return false;

        it->second.quantity -= quantity;
        return true;
    }


    uint32_t Wallet::get(const Currency& currency) const
    {
        auto it = currencies.find(currency.getID());
        return it != currencies.end() ? it->second.quantity : 0;
    }
    
    uint32_t Wallet::get(uint32_t id) const
    {
        auto it = currencies.find(id);
        return it != currencies.end() ? it->second.quantity : 0;
    }

    uint32_t Wallet::get(const std::string& name) const
    {
        const currency::Currency* curr = currency::CurrencyRegistry::getInstance().getByName(name);
        if (!curr) 
        {
            throw std::invalid_argument("Currency name " + name + " not found in registry.");
        }
        return get(*curr);
    }
      

    bool Wallet::has(const Currency& currency, uint32_t quantity) const
    {
        return get(currency) >= quantity;
    }
    
    bool Wallet::has(uint32_t id, uint32_t quantity) const
    {
        auto it = currencies.find(id);
        return it != currencies.end() && it->second.quantity >= quantity;
    }

    bool Wallet::has(const std::string& name, uint32_t quantity) const
    {
        const currency::Currency* curr = currency::CurrencyRegistry::getInstance().getByName(name);
        if (!curr) 
        {
            throw std::invalid_argument("Currency name " + name + " not found in registry.");
        }
        return has(*curr, quantity);
    }


    void Wallet::dump(std::ostream& os) const
    {
        for (const auto& [id, instance] : currencies) 
        {
            os << "Currency: " << instance.currency.getName()
               << ", Type: " << static_cast<int>(instance.currency.getCurrencyType())
               << ", Quantity: " << instance.quantity
               << ", Tradeable: " << instance.currency.isTradeable()
               << "\n";
        }
    }

    
    std::string Wallet::serialize() const 
    {
        std::ostringstream oss;
        oss << "[WALLET_BEGIN]";
        for (const auto& [id, cq] : currencies) {
            oss << id << ':' << cq.quantity << ';';
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

            const currency::Currency* curr = currency::CurrencyRegistry::getInstance().getById(id);
            if (curr) 
            {
                container.add(*curr, qty);
            }
        }

        return container;
    }

} // namespace currency
