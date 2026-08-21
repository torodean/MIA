/**
 * @file FinancialCalculations.cpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file implements various financial calculations.
 */
 
// Associated header file.
#include "FinancialCalculations.hpp"

namespace maple
{
    double getTaxesFromTaxableIncome(double taxableIncome,
                                     double deduction,
                                     FilingStatus status,
                                     const TaxRateConstants& constants)
    {
        return math::finance::getFederalTaxesFromTaxableIncome(
            taxableIncome,
            toTaxBrackets(constants, status),
            deduction);
    }
} // namespace maple
