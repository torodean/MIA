/**
 * File: CurrencyContainer.hpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A container class for managing multiple currencies and their quantities.
 */
#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include "Currency.hpp"

namespace currency
{
    /**
     * A struct to hold a currency and its quantity.
     */
    struct CurrencyQuantity
    {
        Currency currency;  ///< The currency object.
        uint32_t quantity;  ///< The quantity of this currency.

        CurrencyQuantity(const Currency& curr, uint32_t qty = 0)
            : currency(curr), quantity(qty) {}
    };

    /**
     * A container class to manage a set of currencies and their quantities.
     */
    class CurrencyContainer
    {
    public:
        /**
         * Default constructor.
         */
        CurrencyContainer() = default;

        /**
         * Adds a specified quantity of a currency to the container.
         * If the currency already exists, the quantity is incremented.
         * @param currency The currency to add.
         * @param quantity The amount to add.
         */
        void addCurrency(const Currency& currency, uint32_t quantity);

        /**
         * Removes a specified quantity of a currency from the container.
         * @param currency The currency to remove.
         * @param quantity The amount to remove.
         * @return True if successful, false if insufficient quantity or currency not found.
         */
        bool removeCurrency(const Currency& currency, uint32_t quantity);

        /**
         * Gets the quantity of a specific currency in the container.
         * @param currency The currency to query.
         * @return The quantity, or 0 if the currency is not found.
         */
        uint32_t getQuantity(const Currency& currency) const;

        /**
         * Checks if the container has at least the specified quantity of a currency.
         * @param currency The currency to check.
         * @param quantity The required amount.
         * @return True if the container has enough, false otherwise.
         */
        bool hasCurrency(const Currency& currency, uint32_t quantity) const;

        /**
         * Dumps the container's contents to the provided output stream.
         * @param os The output stream to write to (defaults to std::cout).
         */
        void dump(std::ostream& os = std::cout) const;

    private:
        std::unordered_map<uint32_t, CurrencyQuantity> currencies;  ///< Map of currency ID to currency instance.
    }; // class CurrencyContainer
    
} // namespace currency
