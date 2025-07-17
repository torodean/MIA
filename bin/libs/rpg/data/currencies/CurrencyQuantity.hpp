/**
 * @file CurrencyQuantity.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A container class for managing an amount of a currency.
 */
#pragma once

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
} // namespace currency
