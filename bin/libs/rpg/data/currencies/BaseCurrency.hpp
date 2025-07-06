/**
 * File: BaseCurrency.hpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: This is the base currency class for all currencies.
 */
#pragma once

#include <string>


namespace currency
{
    /**
     * This enum represents the type of currency.  
     */
     enum class currencyType : uint8_t
     {
        UNKNOWN = 0,
        STANDARD,
        TOKEN,
        EVENT,
        GEM,
     };
    
    /**
     * This is a base class for all other currencies. It contains common information which
     * all other currencies use (or should use).
     */
    class BaseCurrency
    {
    public:
        /// Default virtual destructor.
        virtual ~BaseCurrency() = default;
    
        /// Gets the name of this currency.
        std::string getName() const 
        { return name; }
        
        /// Gets the description of this currency.
        std::string getDescription() const 
        { return name; }
        
        /// Gets the icon art of this currency.
        std::string getDescription() const 
        { return iconArt; }
        
        /// Get whether or not this currency is tradeable.
        bool isTradeable() const
        { return tradeable; }
        
    protected:
        std::string name;         ///< The name of this currency. 
        std::string description;  ///< A description for this currency.
        std::string iconArt;      ///< The path to the icon art for this currency.
        
        bool tradeable{true};     ///< Whether this currency is tradeable to other players.
        currencyType type;        ///< The type of currency.
    }; // class BaseCurrency
} // namespace currency
