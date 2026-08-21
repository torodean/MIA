/**
 * @file FinancialCalculations.hpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file defines various financial calculations.
 */
#pragma once

#include "TaxConstants.hpp"

namespace maple
{
    double getTaxesFromTaxableIncome(double taxableIncome,
                                     double deduction = 0,
                                     FilingStatus status = FilingStatus::Single,
                                     const TaxRateConstants& constants = {});
} // namespace maple
