/**
 * File: Wallet.cpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A container class for managing multiple currencies and their quantities.
 */

#include <sstream>
#include "Wallet.hpp"
#include "CurrencyRegistry.hpp"


namespace currency
{

    void Wallet::addCurrency(const Currency& currency, uint32_t quantity)
    {
        auto id = currency.getID();
        auto it = currencies.find(id);
        if (it == currencies.end()) {
            currencies.emplace(id, CurrencyQuantity(currency, quantity));
        } else {
            it->second.quantity += quantity;
        }
    }    
    
    void Wallet::addCurrency(const std::string& name, uint32_t quantity)
    {
        auto it = nameToId.find(name);
        if (it != nameToId.end())
            addCurrency(it->second, quantity);
    }
    
    void Wallet::addCurrency(uint32_t id, uint32_t quantity)
    {
        auto it = currencies.find(id);
        if (it != currencies.end()) {
            it->second.quantity += quantity;
        } 
        else 
        {
            // Cannot create CurrencyQuantity without Currency, so do nothing or handle error.
            // Optionally, throw or log an error here.
        }
    }


    bool Wallet::removeCurrency(const Currency& currency, uint32_t quantity)
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
    
    bool Wallet::removeCurrency(uint32_t id, uint32_t quantity)
    {
        auto it = currencies.find(id);
        if (it == currencies.end() || it->second.quantity < quantity)
            return false;

        it->second.quantity -= quantity;
        return true;
    }

    bool Wallet::removeCurrency(const std::string& name, uint32_t quantity)
    {
        auto it = nameToId.find(name);
        return it != nameToId.end() && removeCurrency(it->second, quantity);
    }


    uint32_t Wallet::getQuantity(const Currency& currency) const
    {
        auto it = currencies.find(currency.getID());
        return it != currencies.end() ? it->second.quantity : 0;
    }
    
    uint32_t Wallet::getQuantity(uint32_t id) const
    {
        auto it = currencies.find(id);
        return it != currencies.end() ? it->second.quantity : 0;
    }

    uint32_t Wallet::getQuantity(const std::string& name) const
    {
        auto it = nameToId.find(name);
        return it != nameToId.end() ? getQuantity(it->second) : 0;
    }
      

    bool Wallet::hasCurrency(const Currency& currency, uint32_t quantity) const
    {
        return getQuantity(currency) >= quantity;
    }
    
    bool Wallet::hasCurrency(uint32_t id, uint32_t quantity) const
    {
        auto it = currencies.find(id);
        return it != currencies.end() && it->second.quantity >= quantity;
    }

    bool Wallet::hasCurrency(const std::string& name, uint32_t quantity) const
    {
        auto it = nameToId.find(name);
        return it != nameToId.end() && hasCurrency(it->second, quantity);
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
        oss << "[CC_BEGIN]";
        for (const auto& [id, cq] : currencies) {
            oss << id << ':' << cq.quantity << ';';
        }
        oss << "[CC_END]";
        return oss.str();
    }
    
    
    Wallet Wallet::deserialize(const std::string& data) 
    {
        size_t start = data.find("[CC_BEGIN]");
        size_t end = data.find("[CC_END]", start);

        if (start == std::string::npos || end == std::string::npos) 
        {
            throw std::invalid_argument("Wallet block not found.");
        }

        start += std::string("[CC_BEGIN]").length();
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
                container.addCurrency(*curr, qty);
            }
        }

        return container;
    }

} // namespace currency
