/**
 * @file FinancialCalculations.hpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file defines various financial calculations.
 */
#pragma once

#include "TaxConstants.hpp"
#include "MoneyHandler.hpp"

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
     * @brief Calculates various tax values based on the input income and tax constants.
     *
     * @param income The income values to use.
     * @param federalConstants Federal tax constants.
     * @param stateConstants State tax constants.
     * @param deductible A deductible to use for the calculation.
     */
    void calculateTaxesOperation(const MoneyHandler& income,
                                 const TaxRateConstants& federalConstants,
                                 const TaxRateConstants& stateConstants,
                                 FilingStatus status = FilingStatus::Married,
                                 double deductible = 0);
} // namespace maple
