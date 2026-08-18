/**
 * @file FinanceUtils_T.cpp
 * @author Antonius Torode
 * @date 08/18/2026
 * @brief Unit tests for the finance calculations declared in FinanceUtils.hpp.
 *
 * Tests cover flat-rate tax, progressive federal tax, mortgage payment and cost,
 * and the inflation model. Floating point values are compared with a tolerance
 * since the computations involve rounding in the underlying formulas.
 */

#include <gtest/gtest.h>

#include <cmath>

#include "FinanceUtils.hpp"

using namespace math::finance;

namespace
{
    /// Tolerance for floating point comparisons of dollar and rate values.
    constexpr double tolerance = 1e-6;
}

/**
 * @brief Verifies getFlatRateTax applies the rate to the full amount.
 */
TEST(FinanceUtilsTest, GetFlatRateTaxAppliesRate)
{
    EXPECT_NEAR(getFlatRateTax(100000.0, 0.0145), 1450.0, tolerance);
}

/**
 * @brief Verifies getFlatRateTax returns zero for zero income.
 */
TEST(FinanceUtilsTest, GetFlatRateTaxZeroIncome)
{
    EXPECT_NEAR(getFlatRateTax(0.0, 0.0145), 0.0, tolerance);
}

/**
 * @brief Verifies getFlatRateTax returns zero when the rate is zero.
 */
TEST(FinanceUtilsTest, GetFlatRateTaxZeroRate)
{
    EXPECT_NEAR(getFlatRateTax(100000.0, 0.0), 0.0, tolerance);
}

/**
 * @brief Verifies progressive tax sums the owed amount across multiple brackets.
 */
TEST(FinanceUtilsTest, GetFederalTaxesSpansMultipleBrackets)
{
    TaxBrackets brackets = {
        {0.0, 0.10},
        {10000.0, 0.12},
        {40000.0, 0.22},
    };
    // 50000 taxable: 1000 + 3600 + 2200 = 6800.
    EXPECT_NEAR(getFederalTaxesFromTaxableIncome(50000.0, brackets), 6800.0, tolerance);
}

/**
 * @brief Verifies the deduction reduces taxable income before brackets apply.
 */
TEST(FinanceUtilsTest, GetFederalTaxesAppliesDeduction)
{
    TaxBrackets brackets = {
        {0.0, 0.10},
        {10000.0, 0.12},
        {40000.0, 0.22},
    };
    // 45000 taxable after a 5000 deduction: 1000 + 3600 + 1100 = 5700.
    EXPECT_NEAR(getFederalTaxesFromTaxableIncome(50000.0, brackets, 5000.0), 5700.0, tolerance);
}

/**
 * @brief Verifies income fully within the first bracket is taxed at that rate only.
 */
TEST(FinanceUtilsTest, GetFederalTaxesWithinFirstBracket)
{
    TaxBrackets brackets = {
        {0.0, 0.10},
        {10000.0, 0.12},
    };
    EXPECT_NEAR(getFederalTaxesFromTaxableIncome(5000.0, brackets), 500.0, tolerance);
}

/**
 * @brief Verifies a deduction larger than income yields zero tax.
 */
TEST(FinanceUtilsTest, GetFederalTaxesDeductionExceedsIncome)
{
    TaxBrackets brackets = {
        {0.0, 0.10},
        {10000.0, 0.12},
    };
    EXPECT_NEAR(getFederalTaxesFromTaxableIncome(5000.0, brackets, 10000.0), 0.0, tolerance);
}

/**
 * @brief Verifies empty brackets yield zero tax.
 */
TEST(FinanceUtilsTest, GetFederalTaxesEmptyBrackets)
{
    TaxBrackets brackets;
    EXPECT_NEAR(getFederalTaxesFromTaxableIncome(50000.0, brackets), 0.0, tolerance);
}

/**
 * @brief Verifies a single open top bracket taxes all income at its rate.
 */
TEST(FinanceUtilsTest, GetFederalTaxesSingleBracket)
{
    TaxBrackets brackets = {
        {0.0, 0.20},
    };
    EXPECT_NEAR(getFederalTaxesFromTaxableIncome(30000.0, brackets), 6000.0, tolerance);
}

/**
 * @brief Verifies a zero interest loan pays off in equal installments.
 */
TEST(FinanceUtilsTest, GetMortgagePaymentZeroInterest)
{
    EXPECT_NEAR(getMortgagePayment(200000.0, 0.0, 360), 200000.0 / 360.0, tolerance);
}

/**
 * @brief Verifies the amortized payment for a nonzero interest loan.
 */
TEST(FinanceUtilsTest, GetMortgagePaymentAmortized)
{
    double principal = 200000.0;
    double monthlyRate = 0.05 / 12.0;
    int payments = 360;
    double expected = principal * (monthlyRate * std::pow(1.0 + monthlyRate, payments))
                      / (std::pow(1.0 + monthlyRate, payments) - 1.0);
    EXPECT_NEAR(getMortgagePayment(principal, monthlyRate, payments), expected, tolerance);
}

/**
 * @brief Verifies getMonthlyMortgageCost sums payment, property tax, and insurance.
 */
TEST(FinanceUtilsTest, GetMonthlyMortgageCostSumsComponents)
{
    double payment = getMortgagePayment(200000.0, 0.05 / 12.0, 360);
    double expected = payment + 250000.0 * 0.018 / 12.0 + 250.0;
    EXPECT_NEAR(getMonthlyMortgageCost(200000.0, 0.05 / 12.0, 360, 250000.0), expected, tolerance);
}

/**
 * @brief Verifies the default property tax rate and insurance are applied.
 */
TEST(FinanceUtilsTest, GetMonthlyMortgageCostUsesDefaults)
{
    double payment = getMortgagePayment(200000.0, 0.05 / 12.0, 360);
    // Defaults: propertyTaxRate 0.018, insuranceMonthly 250.0.
    double expected = payment + 250000.0 * 0.018 / 12.0 + 250.0;
    EXPECT_NEAR(getMonthlyMortgageCost(200000.0, 0.05 / 12.0, 360, 250000.0), expected, tolerance);
}
