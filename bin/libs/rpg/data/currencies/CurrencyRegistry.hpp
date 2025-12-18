/**
 * @file CurrencyRegistry.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: A singleton class to manage static Currency objects loaded from a config file.
 */
#pragma once

#include "Registry.hpp"
#include "Currency.hpp"

namespace currency
{
    class CurrencyRegistry : public rpg::Registry<CurrencyRegistry, Currency>
    {        
    protected:
    
        /// Returns the JSON key for the class's data array.
        std::string getJsonKey() const override { return "CURRENCY"; }
    
        /**
         * Parses a JSON object into a Currency instance.
         * Uses the Currency constructor to set all fields, preserving encapsulation.
         * @param json The JSON object representing a currency.
         * @return A Currency object initialized with the JSON data.
         */
        Currency parseJson(const nlohmann::json& json) override
        {
            auto id = json.at("id").get<uint32_t>(); // use at() to require id
            auto name = json.at("name").get<std::string>();
            auto description = json.value("description", "");
            auto type = stringToCurrencyType(json.value("type", "UNKNOWN"));
            auto tradeable = json.value("tradeable", true);
            auto icon = json.value("icon", "");

            Currency currency(id, name, description, type, tradeable, icon);
            return currency;
        }
        
        /**
         * Converts a Currency object into a human-readable string.
         * Uses public getter methods to access Currency fields.
         * @param currency The Currency object to convert.
         * @return A string representation of the Currency.
         */
        std::string toString(const Currency& currency) const override
        {
            return "Name: " + currency.getName() +
                   ", Type: " + currencyTypeToString(currency.getCurrencyType()) +
                   ", Tradeable: " + (currency.isTradeable() ? "true" : "false") +
                   ", Description: " + currency.getDescription() +
                   ", Icon: " + currency.getIconArt();
        }
        
    };
} // namespace currency
