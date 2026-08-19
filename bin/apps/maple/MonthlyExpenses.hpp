/**
 * @file MonthlyExpenses.hpp
 * @author Antonius Torode
 * @date 08/19/2026
 * @brief Defines a storage struct for monthly expenses and helpers to total them.
 */
#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// Used for the configuration file parsing.
#include "MIAConfig.hpp"

namespace maple
{
    /// The scope value for an expense that applies in every scenario.
    inline const std::string ALL_SCOPE{"all"};

    /**
     * @brief A single named monthly expense and the scenario it applies to.
     *
     * The scope is a free-form string (for example "house", "rent", or "rv")
     * so new scenarios can be introduced from the config without code changes.
     * The special scope ALL_SCOPE marks an expense that applies in every
     * scenario. Scope comparisons are case-insensitive.
     */
    struct Expense
    {
        double value{0.0};               ///< The monthly amount.
        std::string scope{ALL_SCOPE};    ///< The scenario this expense applies to.
    };

    /**
     * @brief Stores monthly expenses as named values.
     *
     * This is a simple storage class: it holds the values parsed from the
     * config and offers basic accessors. Totalling is handled by the free
     * function getTotalExpenses().
     */
    struct MonthlyExpenses
    {
        /// The stored expenses, keyed by name.
        std::unordered_map<std::string, Expense> expenses;

        /**
         * @brief Adds or replaces an expense by name.
         *
         * @param name The expense name.
         * @param value The monthly amount.
         * @param scope The scenario this expense applies to.
         */
        void addExpense(const std::string& name, double value,
                        const std::string& scope = ALL_SCOPE);

        /**
         * @brief Returns whether an expense with the given name is stored.
         *
         * @param name The expense name to look up.
         * @return True if an expense with that name exists.
         */
        bool hasExpense(const std::string& name) const;

        /**
         * @brief Returns the expense with the given name.
         *
         * @param name The expense name to look up.
         * @return Pointer to the expense, or nullptr if not found.
         */
        const Expense* getExpense(const std::string& name) const;

        /**
         * @brief Returns the number of stored expenses.
         *
         * @return The expense count.
         */
        size_t size() const;
    }; // struct MonthlyExpenses

    /**
     * @brief Returns the total of expenses that apply in a given scenario.
     *
     * Sums every expense whose scope matches the requested scenario.
     * This is the single-scenario form of the list overload below. The scope
     * comparison is case-insensitive.
     *
     * @param expenses The stored expenses to total.
     * @param scope The scenario to total for.
     * @return The sum of the matching expense values.
     */
    double getTotalExpenses(const MonthlyExpenses& expenses, 
                            const std::string& scope);

    /**
     * @brief Returns the total of expenses that apply in any of the given scenarios.
     *
     * An expense is counted if its scope is in the list. This supports comparing or 
     * combining scenarios, for example totaling house and rv expenses by passing {"house"} 
     * and {"rv"}. Scope comparisons are case-insensitive.
     *
     * @param expenses The stored expenses to total.
     * @param scopes The scenarios to total for.
     * @return The sum of the matching expense values.
     */
    double getTotalExpenses(const MonthlyExpenses& expenses,
                            const std::vector<std::string>& scopes);

    /**
     * @brief Returns the total of every stored expense regardless of scope.
     *
     * @param expenses The stored expenses to total.
     * @return The sum of all stored expense values.
     */
    double getTotalExpenses(const MonthlyExpenses& expenses);

    /**
     * @brief Constructs a MonthlyExpenses object from a configuration object.
     *
     * Expects the configuration object to be in the
     * constants::ConfigType::KEY_VALUE format.
     *
     * Expense entries are read by key prefix. The key format is
     * "Expense_<scope>_<name>" where the scope is the first token after the
     * prefix and the name is everything after it. For example:
     *
     * - "Expense_electricity = 120.50" is scope ALL_SCOPE, name "electricity".
     * - "Expense_house_insurance = 100.0" is scope "house", name "insurance".
     * - "Expense_rv_fuel_tax = 80.0" is scope "rv", name "fuel_tax".
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
     * @return A constructed MonthlyExpenses with values from the config object.
     */
    MonthlyExpenses createMonthlyExpensesFromConfig(const config::MIAConfig& config,
                                                    bool printWarnings = false);
} // namespace maple
