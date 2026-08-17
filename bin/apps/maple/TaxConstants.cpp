/**
 * @file TaxConstants.cpp
 * @author Antonius Torode
 * @date 08/16/2026
 * @brief This file defines implementations for the API defined in the associated header file. 
 */
#include "TaxConstants.hpp"

namespace maple
{
    namespace
    {
       
    }
    
    
    bool updateTaxRateConstantsFromLine(TaxRateConstants& constants,
                                        const std::string& lineToParse)
    {
        // TODO - this method shouldn't be needed - replace with parsing from config object.
        return true;
    }
    

    TaxRateConstants createTaxRateConstantsFromFile(const std::string& fileName, 
                                                  bool printWarnings)
    {
        // TODO - replace this with config object parsing.
        return {};
    }
    
    
    TaxRateConstants createTaxRateConstantsFromConfig(const config::MIAConfig& config, 
                                                    bool printWarnings)
    {
        TaxRateConstants constants;
        double parsedVal = 0.0; // Default value that KeyValueData returns.
        
        return {};
    }
} // namespace maple
