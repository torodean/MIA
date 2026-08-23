/**
 * @file FinancialCalculations.hpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file defines various financial calculations.
 */
#pragma once

#include "TaxConstants.hpp"
#include "MoneyHandler.hpp"
#include "MapleMisc.hpp"

namespace maple
{
    /**
     * This will take the appropriate values available to Maple and calculate 
     * the taxes based off of them. This is essentially a wrapper which will
     * handle converting objects and calling the appropriate FinanceUtils methods.
     * 
     * @param taxableIncome The taxable pre-deduction income.
     * @param deductible Any deductible to use for the calculation. 
     * @param status The filing status
     * @param constants The tax constants which determine calculation parameters.
     * @return The total calculated taxes to pay.
     */
    double getTaxesFromTaxableIncome(double taxableIncome,
                                     double deductible,
                                     FilingStatus status,
                                     const TaxRateConstants& constants);
    /**
     * The cadence of the values in various data sets.
     */
    enum Cadence
    {
        Annual,   ///< Yearly.
        Monthly,  ///< By month.
        Unknown   ///< Unset.
    };
    
    /**
     * @brief Converts a Cadence value to its string representation.
     *
     * @param cadence The Cadence value to convert.
     * @return The string representation of the cadence.
     */
    std::string cadenceToString(Cadence cadence);
    
    /**
     * This is an object which stores values calculated in the calculateTaxesOperation
     * method. This allows these values to be accessible elsewhere without having to
     * re-perform the calculations (if needed). 
     *
     */
    struct TaxOperationReturns
    {
        /// Set to true once the values are set.
        bool initialized{false};
        
        /// The cadence appropriate for these constants.
        Cadence cadence{Cadence::Unknown};
    
        double grossIncome{0.0};
        double deductible{0.0};
        double preTaxIncome{0.0};
        double taxableIncome{0.0};
        double federalTaxesOwed{0.0};
        double stateTaxesOwed{0.0};
        double medicareTaxOwed{0.0};
        double oasdiTaxOwed{0.0};
        double totalTaxesOwed{0.0};
        double totalIncome{0.0};
        double totalIncomeEarned{0.0};
        double totalTakeHomeIncome{0.0};
        double yearlyExpenses{0.0};
        double estimatedSpedingMoney{0.0};
        double estimatedSalesTaxToSpend{0.0};
    };
    
    /**
     * @brief Converts annual tax operation return values to monthly values.
     *
     * Creates a copy of the provided annual values, converts all monetary values
     * from annual to monthly by dividing them by 12, and sets the cadence to
     * Cadence::Monthly. The original object is not modified.
     *
     * @param annual The annual tax operation return values to convert.
     * @return A copy of the values converted to a monthly cadence.
     */
    TaxOperationReturns annualToMonthly(const TaxOperationReturns& annual);
    
    /**
     * @brief Calculates various tax values based on the input income and tax constants.
     *
     * @param income The income values to use.
     * @param expenses The expenses to use.
     * @param federalConstants Federal tax constants.
     * @param stateConstants State tax constants.
     * @param deductible A deductible to use for the calculation.
     */
    TaxOperationReturns calculateTaxesOperation(const MoneyHandler& income,
                                                const MoneyHandler& expenses,
                                                const TaxRateConstants& federalConstants,
                                                const TaxRateConstants& stateConstants,
                                                const MapleMiscTaxValues& miscTaxValues);
                                 
     /**
      * Dumps/prints the data contained in a TaxOperationReturns object.
      * @param return The values to print.
      * @param stream The stream to dump the data to.
      */
     void printTaxOperationReturns(const TaxOperationReturns& returns,
                                   std::ostream& stream = std::cout);
} // namespace maple
