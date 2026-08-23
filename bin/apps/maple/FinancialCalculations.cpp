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
    
    
    void calculateTaxesOperation(const MoneyHandler& income,
                                 const TaxRateConstants& federalConstants,
                                 const TaxRateConstants& stateConstants)
    {
        std::cout << "STILL IN DEV" << std::endl;
        
        // this will default to standard when getTaxesFromTaxableIncome is called.
        double deductible = 0.0; // TODO - get configurable value from config file.
        
        /* TODO - get filing status from config file.
        switch(status)
        {
            case FilingStatus::Single:
                deductible = constants.standardDeductibleSingle;
                break;
            case FilingStatus::Married:
                deductible = constants.standardDeductibleMarried;
                break;
            case FilingStatus::HeadOfHousehold:
                deductible = constants.standardDeductibleHeadOfHousehold;
                break;
            default:
                // TODO - maybe throw here.
                deductible = 0.0;
                break;
        }
        */ 
        deductible = federalConstants.standardDeductibleMarried; // Hard-coding married for now.
        
        double grossIncome = getTotalMoney(income, "all");
        double preTaxIncome = getTotalMoney(income, "all", {"pretax"});
        double taxableIncome = grossIncome - deductible;
        // TODO - Add optional deductions and filing status input here from maple config.
        double federalTaxes = getTaxesFromTaxableIncome(grossIncome, 
                                                        deductible, 
                                                        FilingStatus::Married, 
                                                        federalConstants);
        double stateTaxes = getTaxesFromTaxableIncome(grossIncome, 
                                                      deductible, 
                                                      FilingStatus::Married, 
                                                      stateConstants);
        double medicareTax = math::finance::getFlatRateTax(taxableIncome, federalConstants.medicareTaxRate);
        double oasdiTax = math::finance::getFlatRateTax(taxableIncome, federalConstants.oasdiTaxRate);
        
        double totalIncome = grossIncome + preTaxIncome;
        double totalTaxes = federalTaxes + stateTaxes + medicareTax + oasdiTax;
        double takeHome = grossIncome - totalTaxes;
        double estSalesTax = math::finance::getFlatRateTax(takeHome, stateConstants.salesTax);
        
        std::cout << "Gross income: " << grossIncome << std::endl;
        std::cout << "Pre-tax income: " << preTaxIncome << std::endl;
        std::cout << "Total Income: " << totalIncome << std::endl;
        std::cout << "Taxable Income: " << taxableIncome << std::endl; // TODO
        std::cout << "Total Taxes Owed: " << totalTaxes << std::endl;
        std::cout << " -- Federal Taxes Owed: " << federalTaxes << std::endl;
        std::cout << " -- Medicare Taxes Owed: " << medicareTax << std::endl;
        std::cout << " -- OASDI Taxes Owed: " << oasdiTax << std::endl;
        std::cout << " -- State Taxes Owed: " << stateTaxes << std::endl;
        std::cout << "Take-home Pay After Taxes: " << takeHome << std::endl;
        std::cout << "Estimated Sales Tax on take-home: " << estSalesTax << std::endl;
    }
} // namespace maple
