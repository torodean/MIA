/**
 * File: CurrencyRegistry_T.cpp
 * Author: Antonius Torode
 * Created on: 07/08/2025
 * Description: Unit tests for the CurrencyRegistry class.
 */
#include <gtest/gtest.h>
#include <sstream>
#include <nlohmann/json.hpp>
#include "CurrencyRegistry.hpp"

using namespace currency;

class CurrencyRegistryTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        jsonData = R"([
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
        ])"_json;

        std::ofstream file("test_currency.json");
        file << jsonData.dump(4);
        file.close();

        registry = &dynamic_cast<CurrencyRegistry&>(CurrencyRegistry::getInstance());
        registry->loadFromFile("test_currency.json");
    }

    void TearDown() override 
    {
        std::remove("test_currency.json");
    }

    nlohmann::json jsonData;
    CurrencyRegistry* registry;
};

TEST_F(CurrencyRegistryTest, LoadFromFileAndRetrieveById) 
{
    const Currency* gold = registry->getById(1);
    ASSERT_NE(gold, nullptr);
    EXPECT_EQ(gold->getName(), "Gold");
    EXPECT_EQ(gold->getDescription(), "Primary currency");
    EXPECT_EQ(gold->getCurrencyType(), CurrencyType::COIN);
    EXPECT_TRUE(gold->isTradeable());
    EXPECT_EQ(gold->getIconArt(), "gold.png");
}

TEST_F(CurrencyRegistryTest, RetrieveByName) 
{
    const Currency* gem = registry->getByName("Blue Amethyst");
    ASSERT_NE(gem, nullptr);
    EXPECT_EQ(gem->getID(), 2);
    EXPECT_EQ(gem->getCurrencyType(), CurrencyType::GEM);
    EXPECT_FALSE(gem->isTradeable());
    EXPECT_EQ(gem->getDescription(), "");
}

TEST_F(CurrencyRegistryTest, DumpOutputsCorrectString) 
{
    std::ostringstream oss;
    registry->dump(oss);
    std::string output = oss.str();

    EXPECT_NE(output.find("Name: Gold"), std::string::npos);
    EXPECT_NE(output.find("Name: Blue Amethyst"), std::string::npos);
    EXPECT_NE(output.find("Tradeable: true"), std::string::npos);
    EXPECT_NE(output.find("Tradeable: false"), std::string::npos);
}

