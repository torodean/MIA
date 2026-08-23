/**
 * @file FinancialCalculations.cpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file implements various financial calculations.
 */
 
// Associated header file.
#include "FinancialCalculations.hpp"

#include "MoneyHandler.hpp"
#include "TaxConstants.hpp"

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
        
        returns.initialized = true;
        return returns;
    }


    void printTaxOperationReturns(const TaxOperationReturns& returns,
                                  std::ostream& stream)
    {
        stream << "-----------------------------------" << std::endl;
        stream << "YEARLY TAX INFORMATION" << std::endl;
        stream << "Gross income: " << returns.grossIncome << std::endl;
        stream << "Pre-tax income: " << returns.preTaxIncome << std::endl;
        stream << "Total Income: " << returns.totalIncomeEarned << std::endl;
        stream << "Deductible: " << returns.deductible << std::endl;
        stream << "Taxable Income: " << returns.taxableIncome << std::endl;
        stream << "Total Taxes Owed: " << returns.totalTaxesOwed << std::endl;
        stream << " -- Federal Taxes Owed: " << returns.federalTaxesOwed << std::endl;
        stream << " -- Medicare Taxes Owed: " << returns.medicareTaxOwed << std::endl;
        stream << " -- OASDI Taxes Owed: " << returns.oasdiTaxOwed << std::endl;
        stream << " -- State Taxes Owed: " << returns.stateTaxesOwed << std::endl;
        stream << "Take-home Pay After Taxes: " << returns.totalTakeHomeIncome << std::endl;
        stream << "Yearly expenses: " << returns.yearlyExpenses << std::endl;
        stream << "Estimated Spending Money left: " << returns.estimatedSpedingMoney << std::endl;
        stream << "Estimated Sales Tax on spending money: " << returns.estimatedSalesTaxToSpend << std::endl;
        stream << "-----------------------------------" << std::endl;
    
    }
} // namespace maple
