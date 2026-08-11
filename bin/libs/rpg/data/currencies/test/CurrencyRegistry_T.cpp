/**
 * @file CurrencyRegistry_T.cpp
 * @author Antonius Torode
 * @date 07/08/2025
 * @brief Unit tests for the currency::CurrencyRegistry class using Google Test.
 *
 * The tests exercise loading currency definitions from a JSON file and retrieving
 * them by ID and by name, plus the dump output. The JSON fixture file is written
 * once per suite; the registry is reloaded before each test.
 */

#include <gtest/gtest.h>
#include <sstream>
#include <nlohmann/json.hpp>

#include "CurrencyRegistry.hpp"

namespace currency
{
    /**
     * @brief Test fixture for the CurrencyRegistry tests.
     *
     * SetUpTestSuite() writes a fixed JSON payload to a temp file once for the whole
     * suite; SetUp() reloads it into the shared CurrencyRegistry before each test so
     * every test starts from the same known set of currency definitions (Gold, Blue
     * Amethyst) regardless of run order. TearDownTestSuite() removes the temp file.
     */
    class CurrencyRegistry_T : public ::testing::Test
    {
    protected:
        /**
         * @brief Writes the test_currency.json file once for the test suite.
         */
        static void SetUpTestSuite()
        {
            nlohmann::json data = R"({ "CURRENCY" : [
                {
                    "id": 1,
                    "name": "Gold",
                    "description": "Primary currency",
                    "type": "coin",
                    "tradeable": true,
                    "icon": "gold.png"
                },
                {
                    "id": 2,
                    "name": "Blue Amethyst",
                    "type": "gem",
                    "tradeable": false
                }
            ]})"_json;
    
            std::ofstream file("test_currency.json");
            file << data.dump(4);
            file.close();
        }
    
        /**
         * @brief Removes the temp file after the test suite finishes.
         */
        static void TearDownTestSuite()
        {
            std::remove("test_currency.json");
        }

        /**
         * @brief Reloads the shared CurrencyRegistry from the fixture file before each test.
         */
        void SetUp() override
        {
            registry = &dynamic_cast<CurrencyRegistry&>(CurrencyRegistry::getInstance());
            registry->loadFromFile("test_currency.json");
        }
    
        CurrencyRegistry* registry;
    }; // class CurrencyRegistry_T
    
    /**
     * @brief Verifies loadFromFile populates the registry and getByID returns the full currency definition.
     */
    TEST_F(CurrencyRegistry_T, LoadFromFileAndRetrieveById) 
    {
        const Currency* gold = registry->getByID(1);
        ASSERT_NE(gold, nullptr);
        EXPECT_EQ(gold->getName(), "Gold");
        EXPECT_EQ(gold->getDescription(), "Primary currency");
        EXPECT_EQ(gold->getCurrencyType(), CurrencyType::COIN);
        EXPECT_TRUE(gold->isTradeable());
        EXPECT_EQ(gold->getIconArt(), "gold.png");
    }
    
    /**
     * @brief Verifies getByName resolves a currency and reports its ID, type, tradeable flag, and description.
     */
    TEST_F(CurrencyRegistry_T, RetrieveByName) 
    {
        const Currency* gem = registry->getByName("Blue Amethyst");
        ASSERT_NE(gem, nullptr);
        EXPECT_EQ(gem->getID(), 2);
        EXPECT_EQ(gem->getCurrencyType(), CurrencyType::GEM);
        EXPECT_FALSE(gem->isTradeable());
        EXPECT_EQ(gem->getDescription(), "");
    }
    
    /**
     * @brief Verifies dump writes the name and tradeable flag of each loaded currency to the stream.
     */
    TEST_F(CurrencyRegistry_T, DumpOutputsCorrectString) 
    {
        std::ostringstream oss;
        registry->dump(oss);
        std::string output = oss.str();
    
        EXPECT_NE(output.find("Name: Gold"), std::string::npos);
        EXPECT_NE(output.find("Name: Blue Amethyst"), std::string::npos);
        EXPECT_NE(output.find("Tradeable: true"), std::string::npos);
        EXPECT_NE(output.find("Tradeable: false"), std::string::npos);
    }
} // namespace currency
