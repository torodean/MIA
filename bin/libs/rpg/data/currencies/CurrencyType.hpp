/**
 * @file CurrencyType.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: The types defined for the currency objects.
 */
#pragma once

#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>

#include "BaseDataObject.hpp"

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
} // namespace currency
