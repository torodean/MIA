/**
 * @file Currency.hpp
 * @author Antonius Torode
 * @date 07/06/2025
 * Description: This is the base currency class for all currencies.
 */
#pragma once

#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>

#include "CurrencyType.hpp"
#include "BaseDataObject.hpp"

namespace currency
{
    /**
     * This is a base class for all currencies. It contains common information which
     * all currencies use (or should use).
     */
    class Currency : public rpg::BaseDataObject
    {
    public:
        /// Default constructor.
        Currency() = default;
        
        /**
         * The main constructor for a Currency object.
         * @param name[const std::string&] - The name of the currency.
         * @param description[const std::string&] - The desciption of the currency.
         * @param type[CurrencyType] - The type for this currency.
         * @param trade[bool] - Whether or not this currency is tradeable.
         * @param icon[const std::string&] - The icon art for this currency.
         */
        Currency(uint32_t id,
                 const std::string& name, 
                 const std::string& description,
                 CurrencyType type,
                 bool trade = true,
                 const std::string& icon = "");
    
        /// Getters for the various data mambers.
        std::string getIconArt() const;
        bool isTradeable() const;
        CurrencyType getCurrencyType() const;
        
        /**
         * Serializes the Currency object into a JSON representation.
         *
         * @return A nlohmann::json object containing the currency's ID, name,
         *         description, type (as a string), and tradeable flag.
         */
        nlohmann::json toJson() const override;
        
        /**
         * Deserializes a Currency object from JSON.
         *
         * @param json The JSON object containing Currency properties.
         * @return The constructed Currency object.
         */
        static Currency fromJson(const nlohmann::json& json);
        
    protected:

        CurrencyType type;        ///< The type of currency.
        bool tradeable{true};     ///< Whether this currency is tradeable to other players.
        std::string iconArt{};    ///< The path to the icon art for this currency.
        
    }; // class Currency
} // namespace currency
