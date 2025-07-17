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
        /// Default constructor.
        CurrencyQuantity() = default;
        
        /// The main constructor.
        CurrencyQuantity(uint32_t qty) : quantity(qty) {}
        
        /// Getter for the quantity value.
        unsigned int getQuantity() const { return quantity; }
        
        /// Adjusters for the quantity.
        void set(unsigned int value) { quantity = value; }
        void add(unsigned int value) { quantity += value; }
        void remove(unsigned int value)
        {
            if (value > quantity)
            {
                // TODO - throw exception here, this isn't allowed.
                quantity = 0;
            }
            else
                quantity -= value;
        }
            
    private:        
        unsigned int quantity{0};  ///< The quantity of this currency.
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
         * Gets the CurrencyQuantity associated with the given identifier. If the data object is not found
         * in the objects map, a default constructed data object will be returned.
         * Overloads allow querying by currency name, currency ID, or Currency object (by reference).
         *
         * @param name[const std::string&] - The name of the currency (e.g., "gold", "silver").
         *        id[uint32_t] - The ID of the currency.
         *        currency[const Currency&] - The Currency object.
         * @return The Currency associated with the identifier, or a default Currency if not found.
         */
        CurrencyQuantity& get(const std::string& name);
        CurrencyQuantity& get(uint32_t id);
        CurrencyQuantity& get(const Currency& currency);
        
        /**
         * Adds a specified currency to the container.
         * Overloads allow adding by Currency object, currency ID, or currency name.
         * If the currency is already added, this will increment the value.
         *
         * @param name[const std::string&] - The name of the currency (e.g., "gold", "silver").
         *        id[uint32_t] - The ID of the currency.
         *        currency[const Currency&] - The Currency object.
         * @param quantity[uint32_t] - The amount to add.
         */
        void add(const std::string& name, uint32_t quantity);
        void add(uint32_t id, uint32_t quantity);
        void add(const Currency& currency, uint32_t quantity);

        /**
         * Removes a specified currency from the container.
         * Overloads allow removing by Currency object, currency ID, or currency name.
         *
         * @param name[const std::string&] - The name of the currency (e.g., "gold", "silver").
         *        id[uint32_t] - The ID of the currency.
         *        currency[const Currency&] - The Currency object.
         */
        void remove(const std::string& name);
        void remove(uint32_t id);
        void remove(const Currency& currency);
        
        /**
         * Updates a specified quantity of a currency.
         * Overloads allow updating by Currency object, currency ID, or currency name.
         *
         * @param name[const std::string&] - The name of the currency (e.g., "gold", "silver").
         *        id[uint32_t] - The ID of the currency.
         *        currency[const Currency&] - The Currency object.
         * @param amount[int] - The signed amount to update (positive = add, negative = remove).
         */
        void update(const std::string& name, unsigned int amount);
        void update(uint32_t id, unsigned int amount);
        void update(const Currency& currency, unsigned int amount);

        /**
         * Checks if the container has at least the specified quantity of a currency.
         * Overloads allow checking by Currency object, currency ID, or currency name.
         *
         * @param name[const std::string&] - The name of the currency (e.g., "gold", "silver").
         *        id[uint32_t] - The ID of the currency.
         *        currency[const Currency&] - The Currency object.
         * @param quantity[uint32_t] - The required amount.
         * @return True if the container has enough; false otherwise.
         */
        bool has(const std::string& name, unsigned int quantity) const;
        bool has(uint32_t id, unsigned int quantity) const;
        bool has(const Currency& currency, unsigned int quantity) const;

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
         * Format: [WALLET_BEGIN]id1:qty1;id2:qty2;...[WALLET_END]
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
