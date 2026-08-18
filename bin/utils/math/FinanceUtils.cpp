/**
 * @file FinanceUtils.cpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief Implements the finance calculations declared in the associated header file.
 */

#include "FinanceUtils.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

namespace math
{
namespace finance
{
    double getFlatRateTax(double totalIncome, double rate)
    {
        return totalIncome * rate;
    }


    double getFederalTaxesFromTaxableIncome(double grossIncome,
                                            const TaxBrackets& brackets,
                                            double deduction)
    {
        double taxableIncome = grossIncome - deduction;
        if (taxableIncome <= 0.0 || brackets.empty())
            return 0.0;

        double total = 0.0;
        // Walk each bracket and tax the income that falls within it at that bracket's rate.
        for (size_t i = 0; i < brackets.size(); ++i)
        {
            double lower = brackets[i].lowerBound;
            double upper = (i + 1 < brackets.size()) ? 
                           brackets[i + 1].lowerBound : std::numeric_limits<double>::infinity();

            if (taxableIncome <= lower)
                break;

            // Income taxed in this bracket is capped by the bracket's upper bound.
            double taxedInBracket = std::min(taxableIncome, upper) - lower;
            total += taxedInBracket * brackets[i].rate;
        }

        return total;
    }


    double getMortgagePayment(double principal,
                              double monthlyInterestRate,
                              int numberOfPayments)
    {
        // A zero interest loan is paid off in equal installments.
        if (monthlyInterestRate == 0.0)
            return principal / numberOfPayments;

        double factor = std::pow(1.0 + monthlyInterestRate, numberOfPayments);
        return principal * (monthlyInterestRate * factor) / (factor - 1.0);
    }


    double getMonthlyMortgageCost(double principal,
                                  double monthlyInterestRate,
                                  int numberOfPayments,
                                  double houseCost,
                                  double propertyTaxRate,
                                  double insuranceMonthly)
    {
        double paymentMonthly = getMortgagePayment(principal, monthlyInterestRate, numberOfPayments);
        double propertyTaxesMonthly = houseCost * propertyTaxRate / 12.0;
        return paymentMonthly + propertyTaxesMonthly + insuranceMonthly;
    }


    std::vector<std::vector<double>> generateInflationModel(double startingRate,
                                                            double variability,
                                                            int months)
    {
        std::vector<double> low;
        std::vector<double> base;
        std::vector<double> high;

        if (months <= 0)
            return {low, base, high};

        low.reserve(months);
        base.reserve(months);
        high.reserve(months);

        low.push_back(startingRate);
        base.push_back(startingRate);
        high.push_back(startingRate);

        // Base stays flat; low and high drift by the variability each month.
        for (int i = 1; i < months; ++i)
        {
            low.push_back(low[i - 1] * (1.0 - variability));
            base.push_back(base[i - 1]);
            high.push_back(high[i - 1] * (1.0 + variability));
        }

        return {low, base, high};
    }
} // namespace finance
} // namespace math
