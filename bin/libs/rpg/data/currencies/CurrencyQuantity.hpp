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
        CurrencyQuantity(int32_t qty) : quantity(qty) {}

        /// Getter for the quantity value.
        int32_t getQuantity() const { return quantity; }

        /// Adjusters for the quantity.
        void set(int32_t value);
        void add(int32_t value);

        /**
         * Removes an amount from the quantity.
         * @param value The amount to remove.
         * @throws MIAException with Insufficient_RPG_Quantity if value exceeds the quantity.
         */
        void remove(int32_t value);

    private:
        int32_t quantity{0};  ///< The quantity of this currency.
    };
} // namespace currency
