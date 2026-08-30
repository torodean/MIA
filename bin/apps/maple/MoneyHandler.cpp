/**
 * @file MoneyHandler.cpp
 * @author Antonius Torode
 * @date 08/19/2026
 * @brief Implementations for the API defined in the associated header file.
 */
 
// The associated header file.
#include "MoneyHandler.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

// Used for getAllConfigPairs() and the KeyValuePair type.
#include "Constants.hpp"
// Used for catching config access failures.
#include "MIAException.hpp"
// Used for toLower().
#include "StringUtils.hpp"

namespace maple
{
    std::ostream& operator<<(std::ostream& stream, const MoneyHandlerSource& source)
    {
        stream << "name:" << source.name << ", "
               << "value:" << source.value << ", " 
               << "scope:" << source.scope;
        for (size_t i=0; i< source.tags.size();i++)
            stream << ", tag:" << source.tags[i];
        return stream;
    }


    void MoneyHandler::addSource(const std::string& name, 
                           double value,
                           const std::string& scope,
                           const std::vector<std::string>& tags)
    {
        sources.push_back({name, value, string_utils::toLower(scope), tags});
    }


    bool MoneyHandler::hasSource(const std::string& name) const
    {
        return std::find_if(sources.begin(), sources.end(),
                            [&](const MoneyHandlerSource& source)
                            { return source.name == name; }) != sources.end();
    }


    size_t MoneyHandler::size() const
    {
        return sources.size();
    }
    

    std::ostream& operator<<(std::ostream& stream, const MoneyHandler& money)
    {
        bool first = true;
        
        for (const auto& source : money.sources)
        {
            if (first)
                first = false;
            else            
                stream << ", ";
            stream << "[" << source << "]";
        }
        return stream;
    }


    double getTotalMoney(const MoneyHandler& money,
                         const std::string& scope,
                         const std::vector<std::string>& constrainingTags)
    {
        std::string want = string_utils::toLower(scope);
        double total = 0.0;
        
        for (const auto& source : money.sources)
        {
            if (source.scope != want)
                continue;
                
            if (std::all_of(constrainingTags.begin(), constrainingTags.end(),
                [&](const std::string& tag)
                {
                    return std::find(source.tags.begin(), source.tags.end(), tag)
                           != source.tags.end();
                }))
            {
                total += source.value;
            }
        }
        return total;
    }


    double getTotalMoney(const MoneyHandler& money,
                         const std::vector<std::string>& scopes,
                         const std::vector<std::string>& constrainingTags)
    {
        std::vector<std::string> want;
        want.reserve(scopes.size());
        for (const std::string& scope : scopes)
            want.push_back(string_utils::toLower(scope));

        double total = 0.0;
        for (const auto& source : money.sources)
        {
            if (std::find(want.begin(), want.end(), source.scope) == want.end())
                continue;

            if (std::all_of(constrainingTags.begin(), constrainingTags.end(),
                [&](const std::string& tag)
                {
                    return std::find(source.tags.begin(), source.tags.end(), tag)
                           != source.tags.end();
                }))
            {
                total += source.value;
            }
        }
        return total;
    }


    double getTotalMoney(const MoneyHandler& money)
    {
        double total = 0.0;
        for (const auto& source : money.sources)
            total += source.value;
        return total;
    }


    MoneyHandler createMoneyHandlerFromConfig(const config::MIAConfig& config,
                                              const std::string& prefix,
                                              bool printWarnings)
    {
        MoneyHandler money;

        std::vector<constants::KeyValuePair> pairs;
        try
        {
            pairs = config.getAllConfigPairs();
        }
        catch (const error::MIAException&)
        {
            if (printWarnings)
                std::cerr << "Maple: could not read config pairs for money handler."
                          << std::endl;
            return money;
        }

        for (const constants::KeyValuePair& pair : pairs)
        {
            std::string key = string_utils::toLower(pair.first);
            if (key.rfind(prefix + "_", 0) != 0)
                continue;

            /* 
             * The key after the prefix is "scope_name" or just "name". With no
             * underscore, the whole token is the name and the scope is
             * ALL_SCOPE. With an underscore, the first token is the scope and
             * the remainder is the name. The tag parsing assume the scope exists.
             */ 
            std::string afterPrefix = string_utils::getAfterChar(key, '_');
            size_t underscore = afterPrefix.find('_');

            std::string scope;
            std::string name;
            std::vector<std::string> tags;
            
            if (underscore == std::string::npos)
            {
                scope = ALL_SCOPE;
                name = afterPrefix;
            }
            else
            {
                std::vector<std::string> elements = string_utils::delimiterString(afterPrefix, "_");
                scope = elements[0];
                name = elements[1];
                for (size_t i=2; i< elements.size(); i++)
                    tags.push_back(elements[i]);
            }

            if (name.empty())
            {
                if (printWarnings)
                    std::cerr << "Maple: money handler key '" << pair.first
                              << "' has no name, skipping." << std::endl;
                continue;
            }

            double value = 0.0;
            try
            {
                value = config.getDouble(pair.first);
            }
            catch (const error::MIAException&)
            {
                if (printWarnings)
                    std::cerr << "Maple: money handler '" << pair.first
                              << "' has invalid value '" << pair.second
                              << "', skipping." << std::endl;
                continue;
            }

            money.addSource(name, value, scope, tags);
        }

        return money;
    }
} // namespace maple
