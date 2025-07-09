/**
 * File: CurrencyRegistry.hpp
 * Author: Antonius Torode
 * Created on: 07/06/2025
 * Description: A singleton class to manage static Currency objects loaded from a config file.
 */
#pragma once

#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include "Currency.hpp"

namespace currency
{
    /**
     * A singleton class to manage a registry of static Currency objects.
     */
    class CurrencyRegistry
    {
    public:
        // Delete copy constructor and assignment operator to enforce singleton
        CurrencyRegistry(const CurrencyRegistry&) = delete;
        CurrencyRegistry& operator=(const CurrencyRegistry&) = delete;

        /**
         * Gets the singleton instance of the CurrencyRegistry.
         * @return Reference to the singleton instance.
         */
        static CurrencyRegistry& getInstance();

        /**
         * Loads currencies from a JSON configuration file.
         * @param filename The path to the JSON file.
         * @throws std::runtime_error if the file cannot be read or parsed.
         */
        void loadFromFile(const std::string& filename);

        /**
         * Gets a Currency by its ID.
         * @param id The unique ID of the currency.
         * @return Pointer to the Currency, or nullptr if not found.
         */
        const Currency* getCurrencyById(uint32_t id) const;

        /**
         * Gets a Currency by its name.
         * @param name The name of the currency.
         * @return Pointer to the Currency, or nullptr if not found.
         */
        const Currency* getCurrencyByName(const std::string& name) const;

        /**
         * Dumps all currencies to the provided output stream.
         * @param os The output stream to write to (defaults to std::cout).
         */
        void dump(std::ostream& os = std::cout) const;

    private:
        CurrencyRegistry() = default;

        std::unordered_map<uint32_t, Currency> currencies;  ///< Map of ID to Currency
        std::unordered_map<std::string, uint32_t> nameToId;  ///< Map of name to ID for quick lookup
    };
} // namespace currency
