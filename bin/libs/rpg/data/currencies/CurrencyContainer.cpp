/**
 * File: CurrencyContainer.cpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A container class for managing multiple currencies and their quantities.
 */

#include "CurrencyContainer.hpp"

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

} // namespace currency
