/**
 * File: Currencies.cpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: Implementation of predefined currency references.
 */
#include "Currencies.hpp"

namespace currency
{
    const Currency* copperCoin = nullptr;
    const Currency* silverCoin = nullptr;
    const Currency* goldCoin = nullptr;
    const Currency* platinumCoin = nullptr;
    const Currency* credits = nullptr;

    void initializeCurrencies()
    {
        CurrencyRegistry& registry = CurrencyRegistry::getInstance();
        copperCoin = registry.getCurrencyByName("Copper Coin");
        silverCoin = registry.getCurrencyByName("Silver Coin");
        goldCoin = registry.getCurrencyByName("Gold Coin");
        platinumCoin = registry.getCurrencyByName("Platinum Coin");
        credits = registry.getCurrencyByName("Credits");
    }
} // namespace currency
