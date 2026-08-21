/**
 * @file TaxConstants.cpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file defines implementations for the API defined in the associated header file. 
 */
#include "TaxConstants.hpp"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

// Used for catching config access failures on missing keys.
#include "MIAException.hpp"
// Used for converting vectors to strings.
#include "VectorUtils.hpp"

namespace maple
{
    namespace
    {
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
         * @brief Reads a comma-separated list from the config as doubles, keeping the
         *        default list on a missing key.
         *
         * Catches the MIAException thrown when the key is absent so the caller's default
         * list is preserved. Optionally warns on stderr about the missing key.
         *
         * @param config The config object to read from.
         * @param key The key to look up.
         * @param defaultValue The list to keep if the key is missing.
         * @param printWarnings If true, prints a warning for a missing key.
         * @return The config values, or defaultValue if the key is missing.
         */
        std::vector<double> readBracketListOrKeepDefault(const config::MIAConfig& config,
                                                         const std::string& key,
                                                         std::vector<double> defaultValue,
                                                         bool printWarnings)
        {
            try
            {
                std::vector<std::string> strings = config.getVector(key, ',');
                std::vector<double> values;
                values.reserve(strings.size());
                std::transform(strings.begin(), strings.end(),
                               std::back_inserter(values),
                               [](const std::string& s) { return std::stod(s); });
                return values;
            }
            catch (const error::MIAException&)
            {
                if (printWarnings)
                    std::cerr << "Maple: missing config key '" << key
                              << "', keeping default list" << std::endl;
                return defaultValue;
            }
        }
    } // namespace
    
    
    std::ostream& operator<<(std::ostream &stream, const TaxRateConstants& constants)
    {
        stream << "medicareTaxRate: " << constants.medicareTaxRate
               << ", oasdiTaxRate: " << constants.oasdiTaxRate
               << ", salesTax: " << constants.salesTax
               << ", standardDeductibleSingle: " << constants.standardDeductibleSingle
               << ", standardDeductibleHeadOfHousehold: " << constants.standardDeductibleHeadOfHousehold
               << ", standardDeductibleMarried: " << constants.standardDeductibleMarried
               << ", taxBracketSingle: [" << VectorUtils::vectorToString(constants.taxBracketSingle)
               << "], taxBracketMarried: [" << VectorUtils::vectorToString(constants.taxBracketMarried)
               << "], taxRateBracket: [" << VectorUtils::vectorToString(constants.taxRateBracket) << "]";
       return stream;
               
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
                                                      const std::string& optionalSuffix, 
                                                      bool printWarnings)
    {
        TaxRateConstants constants;

        // Rates and deductions: keep the struct default when a key is missing.
        constants.medicareTaxRate =
            readDoubleOrKeepDefault(config, "medicare_tax_rate" + optionalSuffix,
                                    constants.medicareTaxRate, printWarnings);
        constants.oasdiTaxRate =
            readDoubleOrKeepDefault(config, "oasdi_tax_rate" + optionalSuffix,
                                    constants.oasdiTaxRate, printWarnings);
        constants.salesTax =
            readDoubleOrKeepDefault(config, "sales_tax" + optionalSuffix,
                                    constants.salesTax, printWarnings);
        constants.standardDeductibleSingle =
            readDoubleOrKeepDefault(config, "standard_deductible_single" + optionalSuffix,
                                    constants.standardDeductibleSingle, printWarnings);
        constants.standardDeductibleHeadOfHousehold =
            readDoubleOrKeepDefault(config, "standard_deductible_head_of_household" + optionalSuffix,
                                    constants.standardDeductibleHeadOfHousehold, printWarnings);
        constants.standardDeductibleMarried =
            readDoubleOrKeepDefault(config, "standard_deductible_married" + optionalSuffix,
                                    constants.standardDeductibleMarried, printWarnings);

        // Bracket lists: comma-separated, with "inf" marking the open top bracket.
        constants.taxBracketSingle =
            readBracketListOrKeepDefault(config, "tax_bracket_single" + optionalSuffix,
                                         constants.taxBracketSingle, printWarnings);
        constants.taxBracketMarried =
            readBracketListOrKeepDefault(config, "tax_bracket_married" + optionalSuffix,
                                         constants.taxBracketMarried, printWarnings);
        constants.taxRateBracket =
            readBracketListOrKeepDefault(config, "tax_rate_bracket" + optionalSuffix,
                                         constants.taxRateBracket, printWarnings);

        return constants;
    }
} // namespace maple
