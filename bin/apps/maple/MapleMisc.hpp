/**
 * @file MapleMisc.hpp
 * @author Antonius Torode
 * @date 08/22/2026
 * @brief Defines a storage struct for misc Maple values.
 */
#pragma once

#include <string>
#include <vector>

#include "TaxConstants.hpp"

namespace maple
{
    /**
     * This defines storage for miscellaneous Maple values used throughout the app.
     */
    struct MapleMiscTaxValues
    {
        /// Defines the deductions to use for tax calculations.
        double deductions{0.0};
        
        /// Defines the tax filing status.
        FilingStatus filingStatus{FilingStatus::Single};
        
        /// Defines a list of expense scopes to consider when doing the tax calculations.
        std::vector<std::string> addedTaxExpenses;
    };
    
    /**
     * @brief Constructs a MapleMiscTaxValues object from a configuration object.
     *
     * Expects the configuration object to be in the constants::ConfigType::KEY_VALUE format.
     *
     * MapleMiscTaxValues entries are read by key prefix. The key format is "prefix_<name>" where 
     * the prefix is the first element and the name is everything after it. For example:
     * 
     * - "tax_filingStatus=single" is a prefix of "tax" and a name of "filingStatus".
     *
     * Different prefix values can define different misc objects.
     *
     * @param config The configuration object to use.
     * @param prefix The prefix attached to this handler's config objects (e.g., "tax").
     * @param printWarnings Whether to print warnings for unparseable values.
     * @return A constructed MapleMiscTaxValues with values from the config object.
     */
    MapleMiscTaxValues createMapleMiscTaxFromConfig(const config::MIAConfig& config,
                                                    const std::string& prefix,
                                                    bool printWarnings = false);
} // namespace maple
