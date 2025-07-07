/**
 * File: Currencies.hpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: Predefined currency references for easy access.
 */
#pragma once

#include "CurrencyRegistry.hpp"

namespace currency
{
    // Predefined currency pointers, initialized in Currencies.cpp
    extern const Currency* copperCoin;
    extern const Currency* silverCoin;
    extern const Currency* goldCoin;
    extern const Currency* platinumCoin;
    extern const Currency* credits;

    // Initialize all currency pointers (call after CurrencyRegistry::loadFromFile)
    void initializeCurrencies();
} // namespace currency
