/**
 * @file MoneyHandler.hpp
 * @author Antonius Torode
 * @date 08/19/2026
 * @brief Defines a storage struct for monthly income sources and helpers to total them.
 */
#pragma once

#include <string>
#include <list>
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
     * 
     * Each MoneyHandlerSource object also has an optionl tags container for tagging
     * the data with anything extra - such as preTax.
     */
    struct MoneyHandlerSource
    {
        std::string name{};              ///< The name of this MoneyHandler source
        double value{0.0};               ///< The monthly amount.
        std::string scope{ALL_SCOPE};    ///< The scenario this handler applies to.
        std::vector<std::string> tags{}; ///< Any additional tags to give this source.
    };
    
    /**
     * @brief Turns a MoneyHandlerSource object into a string via an std::ostream.
     * 
     * @param stream The stream to output the string data to.
     * @param source The MoneyHandlerSource object to feed into the stream.
     * @return The stream with the source data piped into it.
     */
    std::ostream& operator<<(std::ostream& stream, const MoneyHandlerSource& source);

    /**
     * @brief Stores monthly money sources as named values.
     *
     * This is a simple storage class: it holds the values parsed from the
     * config and offers basic accessors. Totalling is handled by the free
     * function getTotal().
     */
    struct MoneyHandler
    {
        /// The stored money sources, keyed by name.
        std::list<MoneyHandlerSource> sources;

        /**
         * @brief Adds or replaces a money source by name.
         *
         * @param name The money source name.
         * @param value The monthly amount.
         * @param scope The scenario this handler applies to.
         * @param tags Any additional tags to attach to the data.
         */
        void addSource(const std::string& name, 
                       double value,
                       const std::string& scope = ALL_SCOPE,
                       const std::vector<std::string>& tags = {});

        /**
         * @brief Returns whether a money source with the given name is stored.
         *
         * @param name The money source name to look up.
         * @return True if a money source with that name exists.
         */
        bool hasSource(const std::string& name) const;

        /**
         * @brief Returns the number of stored money sources.
         *
         * @return The money source count.
         */
        size_t size() const;
    }; // struct MoneyHandler
    
    /**
     * @brief Turns a MoneyHandler object into a string via an std::ostream.
     * 
     * @param stream The stream to output the string data to.
     * @param money The MoneyHandler object to feed into the stream.
     * @return The stream with the money data piped into it.
     */
    std::ostream& operator<<(std::ostream& stream, const MoneyHandler& money);

    /**
     * @brief Returns the total of money sources that apply in a given scenario.
     *
     * Sums every money source whose scope matches the requested scenario.
     * This is the single-scenario form of the list overload below. The scope
     * comparison is case-insensitive.
     *
     * @param money The stored money sources to total.
     * @param scope The scenario to total for.
     * @param constrainingTags Additional tags to optionally constrain the data with.
     * @return The sum of the matching money source values.
     */
    double getTotalMoney(const MoneyHandler& money,
                          const std::string& scope,
                          const std::vector<std::string>& constrainingTags = {});

    /**
     * @brief Returns the total of money sources that apply in any of the given scenarios.
     *
     * An money source is counted if its scope is in the list. This supports comparing or
     * combining scenarios, for example totaling house and rv income by passing {"house"}
     * and {"rv"}. Scope comparisons are case-insensitive.
     *
     * @param money The stored money sources to total.
     * @param scopes The scenarios to total for.
     * @param constrainingTags Additional tags to optionally constrain the data with.
     * @return The sum of the matching income source values.
     */
    double getTotalMoney(const MoneyHandler& money,
                          const std::vector<std::string>& scopes,
                          const std::vector<std::string>& constrainingTags = {});

    /**
     * @brief Returns the total of every stored income source regardless of scope.
     *
     * @param income The stored income sources to total.
     * @return The sum of all stored income source values.
     */
    double getTotalMoney(const MoneyHandler& income);

    /**
     * @brief Constructs an MoneyHandler object from a configuration object.
     *
     * Expects the configuration object to be in the
     * constants::ConfigType::KEY_VALUE format.
     *
     * MoneyHandler entries are read by key prefix. The key format is
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
     * @param prefix The prefix attached to this handler's config objects (e.g., "income_").
     * @param printWarnings Whether to print warnings for unparseable values.
     * @return A constructed MoneyHandler with values from the config object.
     */
    MoneyHandler createMoneyHandlerFromConfig(const config::MIAConfig& config,
                                              const std::string& prefix,
                                              bool printWarnings = false);
} // namespace maple
