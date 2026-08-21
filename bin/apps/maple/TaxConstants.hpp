/**
 * @file TaxConstants.hpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file defines tax constants and methods associated with them. 
 */
#pragma once

#include <limits>
#include <string>
#include <vector>
#include <ostream>

// Used for the configuration file parsing.
#include "MIAConfig.hpp"
// Used for the TaxBrackets type returned by the conversion helper.
#include "FinanceUtils.hpp"

namespace maple
{
    /**
     * @brief Filing status used to select tax brackets and standard deductions.
     */
    enum class FilingStatus
    {
        Single,           ///< Filing as an individual.
        Married,          ///< Filing jointly as a married couple.
        HeadOfHousehold   ///< Filing as head of household.
    };

    /**
     * This struct defines constants that are used for various tax calculations.
     * These constants are typically defined by states or governments.
     */
    struct TaxRateConstants
    {
        /// Default constructor.
        TaxRateConstants() = default;
        
        /// Various tax rates.
        double medicareTaxRate{0.0145};  ///< The medicare tax rate.
        double oasdiTaxRate{0.062};      ///< The OASDI tax rate.
        double salesTax{0.0625};         ///< The state sales tax (default is TX).
        
        /**
         * This defines the tax brackets for someone filing as single.
         * Each value represents the lower bound of the indexed tax bracket,
         * where the upper bound would be determined by the next value. The final
         * value is infinity, marking the open top bracket.
         */
        std::vector<double> taxBracketSingle{
            0.0, 10275.0, 41775.0, 89075.0, 170050.0, 215950.0, 539900.0, 
            std::numeric_limits<double>::infinity()
        };

        /**
         * This defines the tax brackets for someone filing as married.
         * Each value represents the lower bound of the indexed tax bracket,
         * where the upper bound would be determined by the next value. The final
         * value is infinity, marking the open top bracket.
         */
        std::vector<double> taxBracketMarried{
            0.0, 20550.0, 83550.0, 178150.0, 340100.0, 431900.0, 647850.0,
            std::numeric_limits<double>::infinity()
        };

        /**
         * This defines the tax rate in each tax bracket.
         */
        std::vector<double> taxRateBracket{
            0.0, 0.10, 0.12, 0.22, 0.24, 0.32, 0.35, 0.37
        };
        
        /// Various standard deductible values.
        double standardDeductibleSingle{12950.0};          ///< Single deductible.
        double standardDeductibleHeadOfHousehold{19400.0}; ///< Head of Household deductible.
        double standardDeductibleMarried{25900.0};         ///< Married deductible.
    }; // struct TaxRateConstants
    
    /**
     * @brief Turns a TaxRateConstants object into a string via an std::ostream.
     * 
     * @param stream The stream to output the string data to.
     * @param constants The TaxRateConstants object to feed into the stream.
     * @return The stream with the constants data piped into it.
     */
    std::ostream& operator<<(std::ostream& stream, const TaxRateConstants& constants);

    /**
     * @brief Builds a TaxBrackets list from a TaxRateConstants object for a filing status.
     *
     * Pairs each income lower bound with the corresponding marginal rate. Single and
     * head-of-household filers use the single brackets; married filers use the married
     * brackets. Both share the same rate list. The resulting list ends with an open top
     * bracket, as expected by math::finance::getFederalTaxesFromTaxableIncome.
     *
     * @param constants The tax rate constants holding the bracket boundaries and rates.
     * @param status The filing status, used to choose single or married brackets.
     * @return A TaxBrackets list ordered by income with an open top bracket.
     */
    math::finance::TaxBrackets toTaxBrackets(const TaxRateConstants& constants,
                                             FilingStatus status);


    /**
     * Constructs a TaxRateConstants object from data from a configuration object.
     * This method expects the configuration object to be formatted in the
     * constants::ConfigType::KEY_VALUE format. For values that are not found in
     * the config, the default values are kept and an optional warning is produced.
     *
     * Bracket lists are comma-separated. The token "inf" marks the open top bracket
     * and is parsed as positive infinity.
     *
     * The key's needed for this object in the config file are pre-determined and hard
     * coded. In order to allow for multiple configurable objects (such as one for
     * federal taxes, and one for state taxes), and optionalSuffix parameter is
     * provided which allows for custom suffixes appended to these keys. 
     *
     * @param config The configuration object to use.
     * @param optionalSuffix When scanning the config values, this is an optional suffix
     *                       appended to the key names.
     * @param printWarnings Whether or not to print warnings for missing values.
     * @return A constructed TaxRateConstants with values from the config object.
     */
    TaxRateConstants createTaxRateConstantsFromConfig(const config::MIAConfig& config,
                                                      const std::string& optionalSuffix = "",  
                                                      bool printWarnings = false);
} // namespace maple
