/**
 * File: Currency.hpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: This is the base currency class for all currencies.
 */
#pragma once

#include <stdint.h>
#include <string>

namespace currency
{
    /**
     * This enum represents the type of currency.  
     */
     enum class currencyType : uint8_t
     {
        UNKNOWN = 0,
        COIN,
        FIAT,
        TOKEN,
        EVENT,
        GEM,
     };
     
     /**
     * Converts a string to a currencyType enum.
     * @param typeStr The string representation of the type.
     * @return The corresponding currencyType, or UNKNOWN if invalid.
     */
    inline currencyType stringToCurrencyType(const std::string& typeStr)
    {
        if (typeStr == "COIN") return currencyType::COIN;
        if (typeStr == "FIAT") return currencyType::FIAT;
        if (typeStr == "TOKEN") return currencyType::TOKEN;
        if (typeStr == "EVENT") return currencyType::EVENT;
        if (typeStr == "GEM") return currencyType::GEM;
        return currencyType::UNKNOWN;
    }
    
    /**
     * This is a base class for all currencies. It contains common information which
     * all currencies use (or should use).
     */
    class Currency
    {
    public:
        /**
         * The main constructor for a Currency object.
         * @param name[const std::string&] - The name of the currency.
         * @param desc[const std::string&] - The desciption of the currency.
         * @param type[currencyType] - The type for this currency.
         * @param trade[bool] - Whether or not this currency is tradeable.
         * @param icon[const std::string&] - The icon art for this currency.
         */
        Currency(const std::string& name, 
                     const std::string& desc,
                     currencyType type,
                     bool trade = true,
                     const std::string& icon = "") : 
            name(name), description(desc), type(type),  tradeable(trade), iconArt(icon)
        { 
            static uint32_t idCounter = 0;
            id = idCounter++; // Simple ID generation            
        }
    
        /// Getters for the various data mambers.
        std::string getName() const  { return name; }
        std::string getDescription() const  { return description; }
        std::string getIconArt() const  { return iconArt; }
        bool isTradeable() const { return tradeable; }
        currencyType getCurrencyType() const  { return type; }
        uint32_t getID() const  { return id; }
        
    protected:
        
        uint32_t id;              ///< A unique identifier for this item.
        std::string name;         ///< The name of this currency.
        std::string description;  ///< A description for this currency.
        currencyType type;        ///< The type of currency.
        bool tradeable{true};     ///< Whether this currency is tradeable to other players.
        std::string iconArt{};    ///< The path to the icon art for this currency.
        
    }; // class Currency
} // namespace currency
