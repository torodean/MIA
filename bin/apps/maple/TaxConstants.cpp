/**
 * @file TaxConstants.cpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file defines implementations for the API defined in the associated header file. 
 */
#include "TaxConstants.hpp"

// Used for catching config access failures on missing keys.
#include "MIAException.hpp"
// Used for string lower-casing.
#include "StringUtils.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace maple
{
    namespace
    {
        /**
         * @brief Converts a string to a double.
         *
         * The string "inf" (case-insensitive) becomes positive infinity, so the open top
         * tax bracket has no upper bound. Anything else is converted with std::stod.
         *
         * @param element The string to convert.
         * @return The double value.
         * @throws std::invalid_argument if the string is not a number and not "inf".
         */
        double parseBracketElement(const std::string& element)
        {
            // Lowercase the element so "inf", "INF", etc. all match.
            std::string lower = StringUtils::toLower(element);

            if (lower == "inf")
                return std::numeric_limits<double>::infinity();

            return std::stod(element);
        }

        /**
         * @brief Converts a list of string elements into doubles.
         *
         * Each element is converted by parseBracketElement, so "inf" is handled.
         *
         * @param elements The strings to convert.
         * @return The converted values.
         */
        std::vector<double> parseBracketList(const std::vector<std::string>& elements)
        {
            std::vector<double> values;
            values.reserve(elements.size());
            for (const std::string& element : elements)
                values.push_back(parseBracketElement(element));
            return values;
        }

        /**
         * @brief Reads a double from the config, keeping the default on a missing key.
         *
         * Catches the MIAException thrown when the key is absent so the caller's default
         * is preserved. Optionally warns on stderr about the missing key.
         *
         * @param config The config object to read from.
         * @param key The key to look up.
         * @param defaultValue The value to keep if the key is missing.
         * @param printWarnings If true, prints a warning for a missing key.
         * @return The config value, or defaultValue if the key is missing.
         */
        double readDoubleOrKeepDefault(const config::MIAConfig& config,
                                       const std::string& key,
                                       double defaultValue,
                                       bool printWarnings)
        {
            try
            {
                return config.getDouble(key);
            }
            catch (const error::MIAException&)
            {
                if (printWarnings)
                    std::cerr << "Maple: missing config key '" << key
                              << "', keeping default " << defaultValue << std::endl;
                return defaultValue;
            }
        }

        /**
         * @brief Reads a comma-separated list from the config as doubles, returning an
         *        empty list if the key is missing.
         *
         * @param config The config object to read from.
         * @param key The key to look up.
         * @param printWarnings If true, prints a warning for a missing key.
         * @return The converted values, or an empty vector if the key is missing.
         */
        std::vector<double> readBracketListOrKeepDefault(const config::MIAConfig& config,
                                                         const std::string& key,
                                                         bool printWarnings)
        {
            try
            {
                std::vector<std::string> strings = config.getVector(key, ',');
                return parseBracketList(strings);
            }
            catch (const error::MIAException&)
            {
                if (printWarnings)
                    std::cerr << "Maple: missing config key '" << key
                              << "', keeping default empty list" << std::endl;
                return {};
            }
        }
    }


    math::finance::TaxBrackets toTaxBrackets(const TaxRateConstants& constants,
                                             FilingStatus status)
    {
        // Single and head-of-household filers use the single bracket boundaries.
        const std::vector<double>& bounds = (status == FilingStatus::Married) ? 
            constants.taxBracketMarried : constants.taxBracketSingle;

        math::finance::TaxBrackets brackets;
        size_t count = std::min(bounds.size(), constants.taxRateBracket.size());
        brackets.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            brackets.push_back({bounds[i], constants.taxRateBracket[i]});
        }

        return brackets;
    }
    
    
    TaxRateConstants createTaxRateConstantsFromConfig(const config::MIAConfig& config,
                                                      bool printWarnings)
    {
        TaxRateConstants constants;

        // Rates and deductions: keep the struct default when a key is missing.
        constants.medicareTaxRate =
            readDoubleOrKeepDefault(config, "medicare_tax_rate",
                                    constants.medicareTaxRate, printWarnings);
        constants.oasdiTaxRate =
            readDoubleOrKeepDefault(config, "oasdi_tax_rate",
                                    constants.oasdiTaxRate, printWarnings);
        constants.salesTax =
            readDoubleOrKeepDefault(config, "sales_tax",
                                    constants.salesTax, printWarnings);
        constants.standardDeductibleSingle =
            readDoubleOrKeepDefault(config, "standard_deductible_single",
                                    constants.standardDeductibleSingle, printWarnings);
        constants.standardDeductibleHeadOfHousehold =
            readDoubleOrKeepDefault(config, "standard_deductible_head_of_household",
                                    constants.standardDeductibleHeadOfHousehold, printWarnings);
        constants.standardDeductibleMarried =
            readDoubleOrKeepDefault(config, "standard_deductible_married",
                                    constants.standardDeductibleMarried, printWarnings);

        // Bracket lists: comma-separated, with "inf" marking the open top bracket.
        constants.taxBracketSingle =
            readBracketListOrKeepDefault(config, "tax_bracket_single", printWarnings);
        constants.taxBracketMarried =
            readBracketListOrKeepDefault(config, "tax_bracket_married", printWarnings);
        constants.taxRateBracket =
            readBracketListOrKeepDefault(config, "tax_rate_bracket", printWarnings);

        return constants;
    }
} // namespace maple
