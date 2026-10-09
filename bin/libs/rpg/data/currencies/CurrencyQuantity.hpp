/**
 * @file CurrencyQuantity.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * @brief A container class for managing an amount of a currency.
 */
#pragma once

#include <cstdint>

namespace currency
{
    /**
     * A struct to hold a currency and its quantity.
     */
    struct CurrencyQuantity
    {
        /// Default constructor.
        CurrencyQuantity() = default;

        /**
         * The main constructor.
         * @param qty The initial quantity.
         */
        CurrencyQuantity(uint32_t qty) : quantity(qty) {}

        /// Getter for the quantity value.
        unsigned int getQuantity() const { return quantity; }

        /// Adjusters for the quantity.
        void set(unsigned int value);
        void add(unsigned int value);

        /**
         * Removes an amount from the quantity.
         * @param value The amount to remove.
         * @throws MIAException with Insufficient_RPG_Quantity if value exceeds the quantity.
         */
        void remove(unsigned int value);

    private:
        unsigned int quantity{0};  ///< The quantity of this currency.
    };
} // namespace currency
