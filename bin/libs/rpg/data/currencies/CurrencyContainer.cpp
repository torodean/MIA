/**
 * File: CurrencyContainer.cpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A container class for managing multiple currencies and their quantities.
 */

#include <sstream>
#include "CurrencyContainer.hpp"
#include "CurrencyRegistry.hpp"


namespace currency
{

    void CurrencyContainer::addCurrency(const Currency& currency, uint32_t quantity)
    {
        auto id = currency.getID();
        auto it = currencies.find(id);
        if (it == currencies.end()) {
            currencies.emplace(id, CurrencyQuantity(currency, quantity));
        } else {
            it->second.quantity += quantity;
        }
    }


    bool CurrencyContainer::removeCurrency(const Currency& currency, uint32_t quantity)
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


    uint32_t CurrencyContainer::getQuantity(const Currency& currency) const
    {
        auto it = currencies.find(currency.getID());
        return it != currencies.end() ? it->second.quantity : 0;
    }


    bool CurrencyContainer::hasCurrency(const Currency& currency, uint32_t quantity) const
    {
        return getQuantity(currency) >= quantity;
    }


    void CurrencyContainer::dump(std::ostream& os) const
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
    
    
    std::string CurrencyContainer::serialize() const 
    {
        std::ostringstream oss;
        oss << "[CC_BEGIN]";
        for (const auto& [id, cq] : currencies) {
            oss << id << ':' << cq.quantity << ';';
        }
        oss << "[CC_END]";
        return oss.str();
    }
    
    
    CurrencyContainer CurrencyContainer::deserialize(const std::string& data) 
    {
        size_t start = data.find("[CC_BEGIN]");
        size_t end = data.find("[CC_END]", start);

        if (start == std::string::npos || end == std::string::npos) 
        {
            throw std::invalid_argument("CurrencyContainer block not found.");
        }

        start += std::string("[CC_BEGIN]").length();
        std::string payload = data.substr(start, end - start);

        CurrencyContainer container;
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
