/**
 * @file CurrencyQuantity.cpp
 * @author Antonius Torode
 * @date 10/09/2026
 * @brief Implements the currency quantity container declared in CurrencyQuantity.hpp.
 */

// Include associated header file.
#include "CurrencyQuantity.hpp"

#include <limits>
#include <string>

// Used for exception and error handling.
#include "MIAException.hpp"
#include "Error.hpp"

namespace currency
{
    void CurrencyQuantity::set(unsigned int value)
    {
        quantity = value;
    }


    void CurrencyQuantity::add(unsigned int value)
    {
        if (quantity > std::numeric_limits<unsigned int>::max() - value)
        {
            MIA_THROW(error::ErrorCode::Exceeded_RPG_Quantity,
                      "Currency quantity overflow from " + std::to_string(quantity) +
                      " adding " + std::to_string(value) + ".");
        }
        quantity += value;
    }


    void CurrencyQuantity::remove(unsigned int value)
    {
        if (value > quantity)
        {
            MIA_THROW(error::ErrorCode::Insufficient_RPG_Quantity,
                      "Cannot remove " + std::to_string(value) +
                      " from a quantity of " + std::to_string(quantity) + ".");
        }
        quantity -= value;
    }
} // namespace currency
