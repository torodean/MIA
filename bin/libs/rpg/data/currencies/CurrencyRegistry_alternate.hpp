/**
 * File: CurrencyRegistry.hpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A singleton class to manage static Currency objects loaded from a config file.
 */
#pragma once

#include "Registry.hpp"
#include "Currency.hpp"

namespace currency
{
    class CurrencyRegistry : public rpg::Registry<Currency>
    {
    protected:
        Currency parseJson(const nlohmann::json& json) override
        {
            Currency currency;
            currency.id = json["id"];
            currency.name = json["name"];
            currency.description = json.value("description", "");
            currency.type = stringToCurrencyType(json.value("type", "coin"));
            currency.tradeable = json.value("tradeable", true);
            currency.icon = json.value("icon", "");
            return currency;
        }

        std::string toString(const Currency& currency) const override
        {
            return "Name: " + currency.name + ", Type: " + currencyTypeToString(currency.type) +
                   ", Tradeable: " + (currency.tradeable ? "true" : "false") +
                   ", Description: " + currency.description + ", Icon: " + currency.icon;
        }
    };
} // namespace currency
