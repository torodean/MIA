/**
 * File: Currency.hpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: This is the base currency class for all currencies.
 */
#pragma once

#include <stdint.h>
#include <string>
#include <algorithm>
#include <nlohmann/json.hpp>

namespace currency
{
    /**
     * This enum represents the type of currency.  
     */
     enum class CurrencyType : uint8_t
     {
        UNKNOWN = 0,
        COIN,
        FIAT,
        TOKEN,
        EVENT,
        GEM,
     };
     
     /**
     * Converts a string to a CurrencyType enum.
     * @param typeStr[const std::string&] - The string representation of the type.
     * @return The corresponding CurrencyType, or UNKNOWN if invalid.
     */
    inline CurrencyType stringToCurrencyType(const std::string& typeStr)
    {
        std::string str = typeStr;
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    
        if (str == "COIN") return CurrencyType::COIN;
        if (str == "FIAT") return CurrencyType::FIAT;
        if (str == "TOKEN") return CurrencyType::TOKEN;
        if (str == "EVENT") return CurrencyType::EVENT;
        if (str == "GEM") return CurrencyType::GEM;
        return CurrencyType::UNKNOWN;
    }
    
     /**
     * Converts a CurrencyType to an std::string.
     * @param type[const CurrencyType&] - The string representation of the type.
     * @return The corresponding CurrencyType, or UNKNOWN if invalid.
     */
    inline std::string currencyTypeToString(const CurrencyType& type)
    {
        switch(type)
        {
            case CurrencyType::COIN:  return "COIN";
            case CurrencyType::FIAT:  return "FIAT";
            case CurrencyType::TOKEN: return "TOKEN";
            case CurrencyType::EVENT: return "EVENT";
            case CurrencyType::GEM:   return "GEM";
            default:                  return "UNKNOWN";
        }
    }
    
    /**
     * This is a base class for all currencies. It contains common information which
     * all currencies use (or should use).
     */
    class Currency
    {
    public:
        /// Default constructor.
        Currency() = default;
        
        /**
         * The main constructor for a Currency object.
         * @param name[const std::string&] - The name of the currency.
         * @param desc[const std::string&] - The desciption of the currency.
         * @param type[CurrencyType] - The type for this currency.
         * @param trade[bool] - Whether or not this currency is tradeable.
         * @param icon[const std::string&] - The icon art for this currency.
         */
        Currency(uint32_t id,
                 const std::string& name, 
                 const std::string& desc,
                 CurrencyType type,
                 bool trade = true,
                 const std::string& icon = "") : 
            id(id), 
            name(name), 
            description(desc), 
            type(type),  
            tradeable(trade), 
            iconArt(icon)
        { }
    
        /// Getters for the various data mambers.
        uint32_t getID() const  { return id; }
        std::string getName() const  { return name; }
        std::string getDescription() const  { return description; }
        std::string getIconArt() const  { return iconArt; }
        bool isTradeable() const { return tradeable; }
        CurrencyType getCurrencyType() const  { return type; }
        
        /**
         * Serializes the Currency object into a JSON representation.
         *
         * @return A nlohmann::json object containing the currency's ID, name,
         *         description, type (as a string), and tradeable flag.
         */
        nlohmann::json toJson()
        {
            return 
            {
                {"id", id},
                {"name", name},
                {"description", description},
                {"type", currency::currencyTypeToString(type)},
                {"tradeable", tradeable}
            };
        }
        
    protected:
        
        uint32_t id;              ///< A unique identifier for this item.
        std::string name;         ///< The name of this currency.
        std::string description;  ///< A description for this currency.
        CurrencyType type;        ///< The type of currency.
        bool tradeable{true};     ///< Whether this currency is tradeable to other players.
        std::string iconArt{};    ///< The path to the icon art for this currency.
        
    }; // class Currency
} // namespace currency
