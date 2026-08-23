/**
 * @file FinancialCalculations.cpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file implements various financial calculations.
 */
 
// Associated header file.
#include "FinancialCalculations.hpp"

#include <iomanip>

#include "MoneyHandler.hpp"
#include "TaxConstants.hpp"
#include "MIAException.hpp"
#include "StringUtils.hpp"

namespace maple
{
    double getTaxesFromTaxableIncome(double taxableIncome,
                                     double deductible,
                                     FilingStatus status,
                                     const TaxRateConstants& constants)
    {        
        return math::finance::getFederalTaxesFromTaxableIncome(taxableIncome,
                                                               toTaxBrackets(constants, status),
                                                               deductible);
    }
    
    
    std::string cadenceToString(Cadence cadence)
    {
        switch (cadence)
        {
            case Cadence::Annual:  return "Annual";
            case Cadence::Monthly: return "Monthly";
            case Cadence::Unknown:
            default:               return "Unknown";
        }
    }
    
    
    TaxOperationReturns annualToMonthly(const TaxOperationReturns& annual)
    {
        // Do nothing if it's already monthly.
        if (annual.cadence == Cadence::Monthly)
            return annual;
        else if (annual.cadence == Cadence::Unknown)
            MIA_THROW(error::ErrorCode::Invalid_Parameter, 
                      "Invalid cadence when converting TaxOperationReturns!");
            
        TaxOperationReturns monthly = annual;

        monthly.cadence = Cadence::Monthly;

        monthly.grossIncome /= 12.0;
        monthly.deductible /= 12.0;
        monthly.preTaxIncome /= 12.0;
        monthly.taxableIncome /= 12.0;
        monthly.federalTaxesOwed /= 12.0;
        monthly.stateTaxesOwed /= 12.0;
        monthly.medicareTaxOwed /= 12.0;
        monthly.oasdiTaxOwed /= 12.0;
        monthly.totalTaxesOwed /= 12.0;
        monthly.totalIncome /= 12.0;
        monthly.totalIncomeEarned /= 12.0;
        monthly.totalTakeHomeIncome /= 12.0;
        monthly.yearlyExpenses /= 12.0;
        monthly.estimatedSpedingMoney /= 12.0;
        monthly.estimatedSalesTaxToSpend /= 12.0;

        return monthly;
    }


    TaxOperationReturns calculateTaxesOperation(const MoneyHandler& income,
                                                const MoneyHandler& expenses,
                                                const TaxRateConstants& federalConstants,
                                                const TaxRateConstants& stateConstants,
                                                const MapleMiscTaxValues& miscTaxValues)
    {
        TaxOperationReturns returns;
        
        // Find the appropriate deductible value to use.
        double standardDeductible = 0.0;
        switch(miscTaxValues.filingStatus)
        {
            case FilingStatus::Single:
                standardDeductible = federalConstants.standardDeductibleSingle;
                break;
            case FilingStatus::Married:
                standardDeductible = federalConstants.standardDeductibleMarried;
                break;
            case FilingStatus::HeadOfHousehold:
                standardDeductible = federalConstants.standardDeductibleHeadOfHousehold;
                break;
            default:
                // TODO - maybe throw here.
                standardDeductible = 0.0;
                break;
        }        
        if (standardDeductible > miscTaxValues.deductions)
            returns.deductible = standardDeductible;
        else
            returns.deductible = miscTaxValues.deductions;            
        
        returns.grossIncome = getTotalMoney(income, "all");
        returns.preTaxIncome = getTotalMoney(income, "all", {"pretax"});
        returns.taxableIncome = returns.grossIncome - returns.deductible;
        returns.federalTaxesOwed = getTaxesFromTaxableIncome(returns.grossIncome, 
                                                             returns.deductible, 
                                                             miscTaxValues.filingStatus, 
                                                             federalConstants);
        returns.stateTaxesOwed = getTaxesFromTaxableIncome(returns.grossIncome, 
                                                           returns.deductible, 
                                                           miscTaxValues.filingStatus, 
                                                           stateConstants);
        returns.medicareTaxOwed = math::finance::getFlatRateTax(returns.taxableIncome, 
                                                                federalConstants.medicareTaxRate);
        returns.oasdiTaxOwed = math::finance::getFlatRateTax(returns.taxableIncome, 
                                                             federalConstants.oasdiTaxRate);
        
        returns.totalIncomeEarned = returns.grossIncome + returns.preTaxIncome;
        returns.totalTaxesOwed = returns.federalTaxesOwed + returns.stateTaxesOwed + 
                                 returns.medicareTaxOwed + returns.oasdiTaxOwed;
        returns.totalTakeHomeIncome = returns.grossIncome - returns.totalTaxesOwed;
        
        std::vector<std::string> taxExpenseScopes = miscTaxValues.addedTaxExpenses;
        taxExpenseScopes.push_back("all");
        returns.yearlyExpenses = getTotalMoney(expenses, taxExpenseScopes) * 12.0;
        returns.estimatedSpedingMoney = returns.totalTakeHomeIncome - returns.yearlyExpenses;
        returns.estimatedSalesTaxToSpend = math::finance::getFlatRateTax(returns.estimatedSpedingMoney, 
                                                                         stateConstants.salesTax);
        
        returns.cadence = Cadence::Annual;
        returns.initialized = true;
        return returns;
    }


    void printTaxOperationReturns(const TaxOperationReturns& returns,
                                  std::ostream& stream)
    {
        std::string cadence = StringUtils::toUpper(cadenceToString(returns.cadence));
        
        // Sets the doubles at 2 decimal places.
        stream << std::fixed << std::setprecision(2);

        stream << "==================================================" << std::endl;
        stream << StringUtils::centerText(cadence + " TAX INFORMATION", 50) << std::endl;
        stream << "--------------------------------------------------" << std::endl;

        stream << "INCOME:" << std::endl;
        stream << std::left  << std::setw(38) << "  Gross Income:"
               << std::right << std::setw(12) << returns.grossIncome << std::endl;
        stream << std::left  << std::setw(38) << "  Pre-tax Income:"
               << std::right << std::setw(12) << returns.preTaxIncome << std::endl;
        stream << std::left  << std::setw(38) << "  Total Income Earned:"
               << std::right << std::setw(12) << returns.totalIncomeEarned << std::endl;
        stream << std::left  << std::setw(38) << "  Deductible:"
               << std::right << std::setw(12) << returns.deductible << std::endl;
        stream << std::left  << std::setw(38) << "  Taxable Income:"
               << std::right << std::setw(12) << returns.taxableIncome << std::endl;
               
        stream << "TAXES:" << std::endl;
        stream << std::left  << std::setw(38) << "  Total Taxes Owed:"
               << std::right << std::setw(12) << returns.totalTaxesOwed << std::endl;
        stream << std::left  << std::setw(38) << "   - Federal Taxes Owed:"
               << std::right << std::setw(12) << returns.federalTaxesOwed << std::endl;
        stream << std::left  << std::setw(38) << "   - Medicare Taxes Owed:"
               << std::right << std::setw(12) << returns.medicareTaxOwed << std::endl;
        stream << std::left  << std::setw(38) << "   - OASDI Taxes Owed:"
               << std::right << std::setw(12) << returns.oasdiTaxOwed << std::endl;
        stream << std::left  << std::setw(38) << "   - State Taxes Owed:"
               << std::right << std::setw(12) << returns.stateTaxesOwed << std::endl;
               
        stream << "SUMMARY:" << std::endl;
        stream << std::left  << std::setw(38) << "  Take-home Pay After Taxes:"
               << std::right << std::setw(12) << returns.totalTakeHomeIncome << std::endl;
        
        stream << "SPENDING/SAVINGS:" << std::endl;
        stream << std::left  << std::setw(38) << "  Total Expenses:"
               << std::right << std::setw(12) << returns.yearlyExpenses << std::endl;
        stream << std::left  << std::setw(38) << "  Estimated Spending Money Left:"
               << std::right << std::setw(12) << returns.estimatedSpedingMoney << std::endl;
        stream << std::left  << std::setw(38) << "  Estimated Sales Tax on Spending:"
               << std::right << std::setw(12) << returns.estimatedSalesTaxToSpend << std::endl;

        stream << "==================================================" << std::endl;
    }
} // namespace maple
