/**
 * @file FinanceUtils.hpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief Declares general-purpose finance calculations for taxes, mortgages, and inflation.
 */
#pragma once

#include <vector>

namespace math
{
namespace finance
{
    /**
     * @brief A single progressive tax bracket boundary paired with its marginal rate.
     *
     * A bracket list is ordered by income. Each entry's lowerBound is where its rate
     * starts applying, and the next entry's lowerBound is where it stops.
     */
    struct TaxBracket
    {
        /// The income value where this bracket's rate begins to apply.
        double lowerBound;

        /// The marginal tax rate that applies to income within this bracket.
        double rate;
    };

    /**
     * @brief An ordered list of progressive tax brackets.
     *
     * The list is ordered by income and ends with an open top bracket. The caller is
     * responsible for this ordering; the type itself is a plain vector alias.
     */
    using TaxBrackets = std::vector<TaxBracket>;

    /**
     * @brief Calculates a flat-rate tax owed on a value.
     *
     * Applies a single rate to the full amount with no deductions or brackets. This is the
     * shape of Medicare and OASDI taxes; the caller supplies the appropriate rate. The
     * library does not hardcode any specific tax rate.
     *
     * @param totalIncome The amount to tax.
     * @param rate The tax rate to apply, as a decimal.
     * @return The tax owed.
     */
    double getFlatRateTax(double totalIncome, double rate);

    /**
     * @brief Calculates federal income tax owed on taxable income using progressive brackets.
     *
     * Subtracts the deduction from gross income, then taxes the remaining amount bracket by
     * bracket at each marginal rate, up to the final open-ended bracket. The caller decides
     * which deduction to supply; the library does not pick a standard deduction.
     *
     * @param grossIncome The gross income to calculate tax from.
     * @param brackets The ordered progressive tax brackets, ending with an open top bracket.
     * @param deduction The deduction to subtract from gross income (default 0.0).
     * @return The total federal income tax owed.
     */
    double getFederalTaxesFromTaxableIncome(double grossIncome,
                                            const TaxBrackets& brackets,
                                            double deduction = 0.0);

    /**
     * @brief Calculates the required monthly payment for a fixed-rate amortizing loan.
     *
     * Uses the standard amortization formula on the monthly interest rate and total
     * number of monthly payments.
     *
     * @param principal The amount borrowed.
     * @param monthlyInterestRate The monthly interest rate as a decimal.
     * @param numberOfPayments The total number of monthly payments.
     * @return The fixed monthly payment that pays off the loan over the term.
     */
    double getMortgagePayment(double principal,
                              double monthlyInterestRate,
                              int numberOfPayments);

    /**
     * @brief Calculates the total monthly cost of carrying a mortgage.
     *
     * Adds the amortized loan payment, prorated property taxes, and insurance.
     *
     * @param principal The amount borrowed.
     * @param monthlyInterestRate The monthly interest rate as a decimal.
     * @param numberOfPayments The total number of monthly payments.
     * @param houseCost The total property value, used to compute property taxes.
     * @param propertyTaxRate The annual property tax rate as a decimal (default 0.018).
     * @param insuranceMonthly The monthly insurance cost (default 250.0).
     * @return The total monthly cost of the mortgage, property taxes, and insurance.
     */
    double getMonthlyMortgageCost(double principal,
                                  double monthlyInterestRate,
                                  int numberOfPayments,
                                  double houseCost,
                                  double propertyTaxRate = 0.018,
                                  double insuranceMonthly = 250.0);

    /**
     * @brief Builds a monthly inflation model over a number of months.
     *
     * Produces three cumulative multiplier series: a base series held flat at the
     * starting rate, a low series decayed by the variability each month, and a high
     * series grown by the variability each month. Each returned value is a multiplier
     * relative to the prior month.
     *
     * @param startingRate The starting inflation rate.
     * @param variability The per-month variability applied to the low and high series.
     * @param months The number of months to project.
     * @return A vector of three vectors: low, base, high multipliers for each month.
     *
     * @note STILL EXPERIMENTAL
     */
    std::vector<std::vector<double>> generateInflationModel(double startingRate,
                                                            double variability,
                                                            int months);
} // namespace finance
} // namespace math
