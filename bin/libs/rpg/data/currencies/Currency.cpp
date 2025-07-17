/**
 * @file Currency.cpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: This is the base currency class for all currencies.
 */
#pragma once

#include "Currency.hpp"

namespace currency
{
    Currency::Currency(uint32_t id,
             const std::string& name, 
             const std::string& description,
             CurrencyType type,
             bool trade,
             const std::string& icon) : 
        BaseDataObject(id, name, description),
        type(type),  
        tradeable(trade), 
        iconArt(icon)
    { }


    std::string Currency::getIconArt() const { return iconArt; }
    bool Currency::isTradeable() const { return tradeable; }
    CurrencyType Currency::getCurrencyType() const { return type; }
    

    nlohmann::json Currency::toJson() const
    {
        nlohmann::json json = BaseDataObject::toJson();
        json["type"] = currency::currencyTypeToString(type);
        json["tradeable"] = tradeable;
        json["icon"] = iconArt;
        return json;
    }


    Currency Currency::fromJson(const nlohmann::json& json)
    {
        BaseDataObject base = BaseDataObject::fromJson(json);
        CurrencyType type = stringToCurrencyType(json.value("type", "UNKNOWN"));
        bool isTradeable = json.value("tradeable", true);
        std::string icon = json.value("icon", "");

        Currency currency(base.getID(), base.getName(), base.getDescription(), 
                          type, isTradeable, icon);
        return currency;
    }

} // namespace currency
