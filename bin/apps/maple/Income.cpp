/**
 * @file Income.cpp
 * @author Antonius Torode
 * @date 08/19/2026
 * @brief Implementations for the API defined in the associated header file.
 */
#include "Income.hpp"

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
        /// The config key prefix that marks an income entry (lowercase).
        const std::string INCOME_PREFIX{"income_"};
    }


    void Income::addSource(const std::string& name, double value,
                           const std::string& scope)
    {
        sources[name] = {value, StringUtils::toLower(scope)};
    }


    bool Income::hasSource(const std::string& name) const
    {
        return sources.find(name) != sources.end();
    }


    const IncomeSource* Income::getSource(const std::string& name) const
    {
        auto it = sources.find(name);
        if (it == sources.end())
            return nullptr;
        return &it->second;
    }


    size_t Income::size() const
    {
        return sources.size();
    }


    double getTotalIncome(const Income& income,
                          const std::string& scope)
    {
        std::string want = StringUtils::toLower(scope);
        double total = 0.0;
        for (const auto& [name, source] : income.sources)
        {
            if (source.scope == want)
                total += source.value;
        }
        return total;
    }


    double getTotalIncome(const Income& income,
                          const std::vector<std::string>& scopes)
    {
        std::vector<std::string> want;
        want.reserve(scopes.size());
        for (const std::string& scope : scopes)
            want.push_back(StringUtils::toLower(scope));

        double total = 0.0;
        for (const auto& [name, source] : income.sources)
        {
            if (std::find(want.begin(), want.end(), source.scope) != want.end())
                total += source.value;
        }
        return total;
    }


    double getTotalIncome(const Income& income)
    {
        double total = 0.0;
        for (const auto& [name, source] : income.sources)
            total += source.value;
        return total;
    }


    Income createIncomeFromConfig(const config::MIAConfig& config,
                                  bool printWarnings)
    {
        Income income;

        std::vector<constants::KeyValuePair> pairs;
        try
        {
            pairs = config.getAllConfigPairs();
        }
        catch (const error::MIAException&)
        {
            if (printWarnings)
                std::cerr << "Maple: could not read config pairs for income."
                          << std::endl;
            return income;
        }

        for (const constants::KeyValuePair& pair : pairs)
        {
            std::string key = StringUtils::toLower(pair.first);
            if (key.rfind(INCOME_PREFIX, 0) != 0)
                continue;

            // The key after the prefix is "scope_name" or just "name". With no
            // underscore, the whole token is the name and the scope is
            // ALL_SCOPE. With an underscore, the first token is the scope and
            // the remainder is the name.
            std::string afterPrefix = key.substr(INCOME_PREFIX.size());
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
                    std::cerr << "Maple: income key '" << pair.first
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
                    std::cerr << "Maple: income '" << pair.first
                              << "' has unparseable value '" << pair.second
                              << "', skipping." << std::endl;
                continue;
            }
            catch (const std::out_of_range&)
            {
                if (printWarnings)
                    std::cerr << "Maple: income '" << pair.first
                              << "' has out-of-range value '" << pair.second
                              << "', skipping." << std::endl;
                continue;
            }

            income.addSource(name, value, scope);
        }

        return income;
    }
} // namespace maple
