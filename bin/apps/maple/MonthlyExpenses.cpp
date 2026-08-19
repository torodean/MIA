/**
 * @file MonthlyExpenses.cpp
 * @author Antonius Torode
 * @date 08/19/2026
 * @brief Implementations for the API defined in the associated header file.
 */
#include "MonthlyExpenses.hpp"

// Used for getAllConfigPairs() and the KeyValuePair type.
#include "Constants.hpp"
// Used for catching config access failures.
#include "MIAException.hpp"
// Used for toLower().
#include "StringUtils.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

namespace maple
{
    namespace
    {
        /// The config key prefix that marks an expense entry (lowercase).
        const std::string EXPENSE_PREFIX{"expense_"};
    }


    void MonthlyExpenses::addExpense(const std::string& name, double value,
                                     const std::string& scope)
    {
        expenses[name] = {value, StringUtils::toLower(scope)};
    }


    bool MonthlyExpenses::hasExpense(const std::string& name) const
    {
        return expenses.find(name) != expenses.end();
    }


    const Expense* MonthlyExpenses::getExpense(const std::string& name) const
    {
        auto it = expenses.find(name);
        if (it == expenses.end())
            return nullptr;
        return &it->second;
    }


    size_t MonthlyExpenses::size() const
    {
        return expenses.size();
    }


    double getTotalExpenses(const MonthlyExpenses& expenses,
                            const std::string& scope)
    {
        std::string want = StringUtils::toLower(scope);
        double total = 0.0;
        for (const auto& [name, expense] : expenses.expenses)
        {
            if (expense.scope == want)
                total += expense.value;
        }
        return total;
    }


    double getTotalExpenses(const MonthlyExpenses& expenses,
                            const std::vector<std::string>& scopes)
    {
        std::vector<std::string> want;
        want.reserve(scopes.size());
        for (const std::string& scope : scopes)
            want.push_back(StringUtils::toLower(scope));

        double total = 0.0;
        for (const auto& [name, expense] : expenses.expenses)
        {
            if (std::find(want.begin(), want.end(), expense.scope) != want.end())
                total += expense.value;
        }
        return total;
    }


    double getTotalExpenses(const MonthlyExpenses& expenses)
    {
        double total = 0.0;
        for (const auto& [name, expense] : expenses.expenses)
            total += expense.value;
        return total;
    }


    MonthlyExpenses createMonthlyExpensesFromConfig(const config::MIAConfig& config,
                                                    bool printWarnings)
    {
        MonthlyExpenses expenses;

        std::vector<constants::KeyValuePair> pairs;
        try
        {
            pairs = config.getAllConfigPairs();
        }
        catch (const error::MIAException&)
        {
            if (printWarnings)
                std::cerr << "Maple: could not read config pairs for monthly expenses."
                          << std::endl;
            return expenses;
        }

        for (const constants::KeyValuePair& pair : pairs)
        {
            std::string key = StringUtils::toLower(pair.first);
            if (key.rfind(EXPENSE_PREFIX, 0) != 0)
                continue;

            // The key after the prefix is "scope_name" or just "name". With no
            // underscore, the whole token is the name and the scope is
            // ALL_SCOPE. With an underscore, the first token is the scope and
            // the remainder is the name.
            std::string afterPrefix = key.substr(EXPENSE_PREFIX.size());
            size_t underscore = afterPrefix.find('_');

            std::string scope;
            std::string name;
            if (underscore == std::string::npos)
            {
                scope = ALL_SCOPE;
                name = afterPrefix;
            }
            else
            {
                scope = afterPrefix.substr(0, underscore);
                name = afterPrefix.substr(underscore + 1);
            }

            if (name.empty())
            {
                if (printWarnings)
                    std::cerr << "Maple: expense key '" << pair.first
                              << "' has no name, skipping." << std::endl;
                continue;
            }

            double value = 0.0;
            try
            {
                value = std::stod(pair.second);
            }
            catch (const std::invalid_argument&)
            {
                if (printWarnings)
                    std::cerr << "Maple: expense '" << pair.first
                              << "' has unparseable value '" << pair.second
                              << "', skipping." << std::endl;
                continue;
            }
            catch (const std::out_of_range&)
            {
                if (printWarnings)
                    std::cerr << "Maple: expense '" << pair.first
                              << "' has out-of-range value '" << pair.second
                              << "', skipping." << std::endl;
                continue;
            }

            expenses.addExpense(name, value, scope);
        }

        return expenses;
    }
} // namespace maple
