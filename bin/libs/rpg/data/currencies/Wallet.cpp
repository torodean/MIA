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

    void Wallet::addCurrency(const Currency& currency, uint32_t quantity)
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
    
    void Wallet::addCurrency(const std::string& name, uint32_t quantity)
    {
        const currency::Currency* curr = currency::CurrencyRegistry::getInstance().getByName(name);
        addCurrency(*curr, quantity);
    }
    
    void Wallet::addCurrency(uint32_t id, uint32_t quantity)
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
            addCurrency(*curr, quantity);
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

    bool Wallet::removeCurrency(const std::string& name, uint32_t quantity)
    {
        const currency::Currency* curr = currency::CurrencyRegistry::getInstance().getByName(name);
        if (!curr) 
        {
            throw std::invalid_argument("Currency name " + name + " not found in registry.");
        }
        return removeCurrency(*curr, quantity);
    }
    
    bool Wallet::removeCurrency(uint32_t id, uint32_t quantity)
    {
        auto it = currencies.find(id);
        if (it == currencies.end() || it->second.quantity < quantity)
            return false;

        it->second.quantity -= quantity;
        return true;
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
        const currency::Currency* curr = currency::CurrencyRegistry::getInstance().getByName(name);
        if (!curr) 
        {
            throw std::invalid_argument("Currency name " + name + " not found in registry.");
        }
        return getQuantity(*curr);
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
        const currency::Currency* curr = currency::CurrencyRegistry::getInstance().getByName(name);
        if (!curr) 
        {
            throw std::invalid_argument("Currency name " + name + " not found in registry.");
        }
        return hasCurrency(*curr, quantity);
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
