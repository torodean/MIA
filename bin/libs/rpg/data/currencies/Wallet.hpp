/**
 * @file Wallet.hpp
 * @author Antonius Torode
 * @date 07/06/2025
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
    class Wallet
    {
    public:
        /**
         * Default constructor.
         */
        Wallet() = default;

        /**
         * Adds a specified quantity of a currency to the container.
         * Overloads allow adding by Currency object, currency ID, or currency name.
         * If the currency already exists, the quantity is incremented.
         *
         * @param currency[const Currency&] - The currency (Currency object)
         *        currencyId[uint32_t] - The ID (uint32_t)
         *        name[const std::string&] - The name (std::string)
         * @param quantity[uint32_t] - The amount to add.
         */
        void add(const Currency& currency, uint32_t quantity);
        void add(uint32_t currencyId, uint32_t quantity);
        void add(const std::string& name, uint32_t quantity);

        /**
         * Removes a specified quantity of a currency from the container.
         * Overloads allow removing by Currency object, currency ID, or currency name.
         *
         * @param currency[const Currency&] - The currency (Currency object)
         *        currencyId[uint32_t] - The ID (uint32_t)
         *        name[const std::string&] - The name (std::string)
         * @param quantity[uint32_t] - The amount to remove.
         * @return True if successful; false if insufficient quantity or currency not found.
         */
        bool remove(const Currency& currency, uint32_t quantity);
        bool remove(uint32_t currencyId, uint32_t quantity);
        bool remove(const std::string& name, uint32_t quantity);

        /**
         * Gets the quantity of a specific currency in the container.
         * Overloads allow querying by Currency object, currency ID, or currency name.
         *
         * @param currency[const Currency&] - The currency (Currency object)
         *        currencyId[uint32_t] - The ID (uint32_t)
         *        name[const std::string&] - The name (std::string)
         * @return The quantity, or 0 if the currency is not found.
         */
        uint32_t get(const Currency& currency) const;
        uint32_t get(uint32_t currencyId) const;
        uint32_t get(const std::string& name) const;

        /**
         * Checks if the container has at least the specified quantity of a currency.
         * Overloads allow checking by Currency object, currency ID, or currency name.
         *
         * @param currency[const Currency&] - The currency (Currency object)
         *        currencyId[uint32_t] - The ID (uint32_t)
         *        name[const std::string&] - The name (std::string)
         * @param quantity[uint32_t] - The required amount.
         * @return True if the container has enough; false otherwise.
         */
        bool has(const Currency& currency, uint32_t quantity) const;
        bool has(uint32_t currencyId, uint32_t quantity) const;
        bool has(const std::string& name, uint32_t quantity) const;

        /**
         * Dumps the container's contents to the provided output stream.
         * @param os The output stream to write to (defaults to std::cout).
         */
        void dump(std::ostream& os = std::cout) const;
        
        /**
         * Serializes the contents of the Wallet to a compact string.
         * The serialized format is enclosed between unique [WALLET_BEGIN] and [WALLET_END] markers
         * for easy identification within a larger data stream.
         *
         * Format: [CC_BEGIN]id1:qty1;id2:qty2;...[CC_END]
         *
         * @return A string representing the serialized state of the container.
         */
        std::string serialize() const;
        
        /**
         * Deserializes a Wallet from a string containing serialized data.
         * The method searches for a block enclosed between [WALLET_BEGIN] and [WALLET_END],
         * then reconstructs the container from the currency ID and quantity pairs.
         *
         * @param data A string containing the serialized container, possibly among other data.
         * @return A reconstructed Wallet instance.
         * @throws std::invalid_argument if no valid serialized block is found.
         */
        static Wallet deserialize(const std::string& data);

    private:
        std::unordered_map<uint32_t, CurrencyQuantity> currencies;  ///< Map of currency ID to currency instance.
    }; // class Wallet
    
} // namespace currency
