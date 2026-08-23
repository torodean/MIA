/**
 * @file MapleMisc.cpp
 * @author Antonius Torode
 * @date 08/22/2026
 * @brief Implementation for the methods in the MapleMisc header.
 */

// The associated header file.
#include "MapleMisc.hpp"

#include <iostream>

// Used for getAllConfigPairs() and the KeyValuePair type.
#include "Constants.hpp"
// Used for catching config access failures.
#include "MIAException.hpp"
// Used for toLower().
#include "StringUtils.hpp"
#include "TaxConstants.hpp"

namespace maple
{

    MapleMiscTaxValues createMapleMiscTaxFromConfig(const config::MIAConfig& config,
                                                    const std::string& prefix,
                                                    bool printWarnings)
    {
        MapleMiscTaxValues taxValues;
        
        std::vector<constants::KeyValuePair> pairs;
        try
        {
            pairs = config.getAllConfigPairs();
        }
        catch (const error::MIAException&)
        {
            if (printWarnings)
                std::cerr << "Maple: could not read config pairs for misc values."
                          << std::endl;
            return taxValues;
        }
        
        
        for (const constants::KeyValuePair& pair : pairs)
        {
            std::string key = StringUtils::toLower(pair.first);
            if (key.rfind(prefix + "_", 0) != 0)
                continue;

            /* 
             * The key after the prefix is the "name". 
             */ 
            std::string name = StringUtils::getAfterChar(key, '_');

            if (name.empty())
            {
                if (printWarnings)
                    std::cerr << "Maple: misc value key '" << pair.first
                              << "' has no name, skipping." << std::endl;
                continue;
            }
            
            if (name == "deductions")
            {
                try
                {
                    taxValues.deductions = std::stod(pair.second);
                }
                catch (const std::invalid_argument&)
                {
                    if (printWarnings)
                        std::cerr << "Maple: misc value '" << pair.first
                                  << "' has unparseable value '" << pair.second
                                  << "', skipping." << std::endl;
                    continue;
                }
                catch (const std::out_of_range&)
                {
                    if (printWarnings)
                        std::cerr << "Maple: misc value '" << pair.first
                                  << "' has out-of-range value '" << pair.second
                                  << "', skipping." << std::endl;
                    continue;
                }
            }
            else if (name == "filingstatus")
            {
                taxValues.filingStatus = stringToFilingStatus(pair.second);
            }
            else if (name == "addedexpenses")
            {
                taxValues.addedTaxExpenses = StringUtils::delimiterString(pair.second, ",");
            }
        }
        
        return taxValues;
    }
} // namespace maple
