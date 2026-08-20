/**
 * @file Income.hpp
 * @author Antonius Torode
 * @date 08/19/2026
 * @brief Defines a storage struct for monthly income sources and helpers to total them.
 */
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// Used for the configuration file parsing.
#include "MIAConfig.hpp"
// Used for the shared ALL_SCOPE constant.
#include "MapleConstants.hpp"

namespace maple
{
    /**
     * @brief A single named monthly income source and the scenario it applies to.
     *
     * The scope is a free-form string (for example "house", "rent", or "rv")
     * so new scenarios can be introduced from the config without code changes.
     * The special scope ALL_SCOPE marks an income source that applies in every
     * scenario. Scope comparisons are case-insensitive.
     */
    struct IncomeSource
    {
        double value{0.0};               ///< The monthly amount.
        std::string scope{ALL_SCOPE};    ///< The scenario this income applies to.
    };
    
    /**
     * @brief Turns a IncomeSource object into a string via an std::ostream.
     * 
     * @param stream The stream to output the string data to.
     * @param source The IncomeSource object to feed into the stream.
     * @return The stream with the source data piped into it.
     */
    std::ostream& operator<<(std::ostream& stream, const IncomeSource& source);

    /**
     * @brief Stores monthly income sources as named values.
     *
     * This is a simple storage class: it holds the values parsed from the
     * config and offers basic accessors. Totalling is handled by the free
     * function getTotalIncome().
     */
    struct Income
    {
        /// The stored income sources, keyed by name.
        std::unordered_map<std::string, IncomeSource> sources;

        /**
         * @brief Adds or replaces an income source by name.
         *
         * @param name The income source name.
         * @param value The monthly amount.
         * @param scope The scenario this income applies to.
         */
        void addSource(const std::string& name, double value,
                       const std::string& scope = ALL_SCOPE);

        /**
         * @brief Returns whether an income source with the given name is stored.
         *
         * @param name The income source name to look up.
         * @return True if an income source with that name exists.
         */
        bool hasSource(const std::string& name) const;

        /**
         * @brief Returns the income source with the given name.
         *
         * @param name The income source name to look up.
         * @return Pointer to the income source, or nullptr if not found.
         */
        const IncomeSource* getSource(const std::string& name) const;

        /**
         * @brief Returns the number of stored income sources.
         *
         * @return The income source count.
         */
        size_t size() const;
    }; // struct Income
    
    /**
     * @brief Turns a Income object into a string via an std::ostream.
     * 
     * @param stream The stream to output the string data to.
     * @param income The Income object to feed into the stream.
     * @return The stream with the income data piped into it.
     */
    std::ostream& operator<<(std::ostream& stream, const Income& income);

    /**
     * @brief Returns the total of income sources that apply in a given scenario.
     *
     * Sums every income source whose scope matches the requested scenario.
     * This is the single-scenario form of the list overload below. The scope
     * comparison is case-insensitive.
     *
     * @param income The stored income sources to total.
     * @param scope The scenario to total for.
     * @return The sum of the matching income source values.
     */
    double getTotalIncome(const Income& income,
                          const std::string& scope);

    /**
     * @brief Returns the total of income sources that apply in any of the given scenarios.
     *
     * An income source is counted if its scope is in the list. This supports comparing or
     * combining scenarios, for example totaling house and rv income by passing {"house"}
     * and {"rv"}. Scope comparisons are case-insensitive.
     *
     * @param income The stored income sources to total.
     * @param scopes The scenarios to total for.
     * @return The sum of the matching income source values.
     */
    double getTotalIncome(const Income& income,
                          const std::vector<std::string>& scopes);

    /**
     * @brief Returns the total of every stored income source regardless of scope.
     *
     * @param income The stored income sources to total.
     * @return The sum of all stored income source values.
     */
    double getTotalIncome(const Income& income);

    /**
     * @brief Constructs an Income object from a configuration object.
     *
     * Expects the configuration object to be in the
     * constants::ConfigType::KEY_VALUE format.
     *
     * Income entries are read by key prefix. The key format is
     * "Income_<scope>_<name>" where the scope is the first token after the
     * prefix and the name is everything after it. For example:
     *
     * - "Income_primary = 7451.99" is scope ALL_SCOPE, name "primary".
     * - "Income_house_rental = 1500.0" is scope "house", name "rental".
     * - "Income_rv_side = 200.0" is scope "rv", name "side".
     *
     * A key with only one token after the prefix (no scope token) is treated
     * as scope ALL_SCOPE. The prefix and scope are matched case-insensitively.
     * New scenarios can be introduced from the config without code changes.
     *
     * Values that cannot be parsed as a double are skipped. If printWarnings is
     * true, a warning is printed for each skipped value.
     *
     * @param config The configuration object to use.
     * @param printWarnings Whether to print warnings for unparseable values.
     * @return A constructed Income with values from the config object.
     */
    Income createIncomeFromConfig(const config::MIAConfig& config,
                                  bool printWarnings = false);
} // namespace maple
