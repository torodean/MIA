/**
 * @file MapleMisc.cpp
 * @author Antonius Torode
 * @date 08/22/2026
 * @brief Implementation for the methods in the MapleMisc header.
 */

// The associated header file.
#include "MapleMisc.hpp"

#include <iostream>

// Used for config parsing.
#include "MIAConfig.hpp"
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
        
        try
        {
            taxValues.deductions = config.getDouble(prefix + "_deductions");
            taxValues.filingStatus = stringToFilingStatus(config.getString(prefix + "_filingstatus"));
            taxValues.addedTaxExpenses = config.getVector(prefix + "_addedexpenses", ',');
        }
        catch (const error::MIAException&)
        {
            if (printWarnings)
                std::cerr << "Maple: missing config key (one of): " << std::endl
                          << " - " << prefix + "_deductions"
                          << " - " << prefix + "_filingstatus"
                          << " - " << prefix + "_addedexpenses"
                          << std::endl;
        }
        
        return taxValues;
    }
} // namespace maple
