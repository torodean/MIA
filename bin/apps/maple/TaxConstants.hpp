/**
 * @file TaxConstants.hpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file defines tax constants and methods associated with them. 
 */
#pragma once

#include <string>
#include <vector>

namespace maple
{
    /**
     * This struct defines constants that are used for various tax calculations.
     * These constants are typically defined by states or governments.
     */
    struct TaxRateConstants
    {
        /// Various tax rates.
        double medicareTaxRate{0.145};   ///< The medicare tax rate.
        double oasdiTaxRate{0.062};      ///< The OASDI tax rate.
        double salesTax{0.0625};         ///< The state sales tax (default is TX).
        
        /**
         * This defines the tax brackets for someone filing as single.
         * Each value represents the lower bound of the indexed tax bracket, 
         * where the upper bound would be determined by the next value.
         */
        std::vector<double> taxBracketSingle{};
        
        /**
         * This defines the tax brackets for someone filing as married.
         * Each value represents the lower bound of the indexed tax bracket, 
         * where the upper bound would be determined by the next value.
         */
        std::vector<double> taxBracketMarried{};
        
        /**
         * This defines the tax rate in each tax bracket.
         */
        std::vector<double> taxRateBracket{};
        
        /// Various standard deductible values.
        double standardDeductibleSingle{12950.0};          ///< Single deductible. 
        double standardDeductibleHeadOfHousehold{19400.0}; ///< Head of Household deductible.
        double standardDeductibleMarried{25900.0};         ///< Married deductible.
    }; // struct TaxRateConstants
    
    /**
     * This method will take a TaxRateConstants object and update any value of it
     * (if applicable) based on an input line. The input line is expected to be a
     * line from a constants::ConfigType::KEY_VALUE formatted config file.
     * @param constants The object to update.
     * @param lineToParse The line of the config file to parse.
     * @return true if constants was updated with information from the line.
     * @return false if constants was not updated with information from the line.
     */
    bool updateTaxRateConstantsFromLine(TaxRateConstants& constants,
                                        const std::string& lineToParse); 
    
    /**
     * Constructs a TaxRateConstants object from data from a configuration file.
     * This method expects the configuration file to be formatted in the 
     * constants::ConfigType::KEY_VALUE format. For values that are not found in
     * the config file, the default values are kept and an optional warning is produced.
     * 
     * @param fileName The full file path and name of the configuration file to use.
     * @param printWarnings Whether or not to print warnings for missing values.
     * @return A constructed TaxRateConstants with values from the config file. 
     */
    TaxRateConstants loadTaxRateConstantsFromFile(const std::string& fileName, 
                                                  bool printWarnings = false);
} // namespace maple
