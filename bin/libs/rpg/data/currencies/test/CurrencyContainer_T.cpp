/**
 * File: CurrencyContainer_T.cpp
 * Author: Antonius Torode
 * Created on: 07/09/2025
 * Description: Google Test suite for the CurrencyContainer class.
 */

#include <gtest/gtest.h>
#include "CurrencyContainer.hpp"
#include "CurrencyRegistry.hpp"

using namespace currency;

/**
 * Test fixture for CurrencyContainer tests.
 */
class CurrencyContainerTest : public ::testing::Test 
{
protected:
    void SetUp() override
    {
        nlohmann::json jsonArray = { coin.toJson(), gem.toJson() };
        std::string jsonData = jsonArray.dump();
        
        // Load currencies into registry
        currency::CurrencyRegistry::getInstance().loadFromString(jsonData);
    }
    
    CurrencyContainer container;

    Currency coin {1, "Coin", "Standard in-game coin", CurrencyType::COIN};
    Currency gem  {2, "Gem", "Premium currency", CurrencyType::GEM};
};

/**
 * @test Verify currency is added and quantity is tracked correctly.
 */
TEST_F(CurrencyContainerTest, AddCurrencyIncreasesQuantity) 
{
    container.addCurrency(coin, 100);
    EXPECT_EQ(container.getQuantity(coin), 100);

    container.addCurrency(coin, 50);
    EXPECT_EQ(container.getQuantity(coin), 150);
}

/**
 * @test Removing currency decreases quantity correctly.
 */
TEST_F(CurrencyContainerTest, RemoveCurrencyDecreasesQuantity) 
{
    container.addCurrency(coin, 80);
    EXPECT_TRUE(container.removeCurrency(coin, 30));
    EXPECT_EQ(container.getQuantity(coin), 50);
}

/**
 * @test Cannot remove more currency than is present.
 */
TEST_F(CurrencyContainerTest, RemoveFailsIfInsufficientQuantity) 
{
    container.addCurrency(gem, 10);
    EXPECT_FALSE(container.removeCurrency(gem, 20));
    EXPECT_EQ(container.getQuantity(gem), 10);
}

/**
 * @test Checking if container has enough currency.
 */
TEST_F(CurrencyContainerTest, HasCurrencyCheckWorks) 
{
    container.addCurrency(coin, 75);
    EXPECT_TRUE(container.hasCurrency(coin, 50));
    EXPECT_FALSE(container.hasCurrency(coin, 100));
}

/**
 * @test Getting quantity of unknown currency returns zero.
 */
TEST_F(CurrencyContainerTest, GetQuantityForUnknownCurrencyReturnsZero) 
{
    EXPECT_EQ(container.getQuantity(gem), 0);
}

/**
 * @test Serializing and deserializing preserves data.
 */
TEST_F(CurrencyContainerTest, SerializeAndDeserializeRoundTrip) 
{    
    container.addCurrency(coin, 25);
    container.addCurrency(gem, 40);

    std::string data = container.serialize();
    CurrencyContainer restored = CurrencyContainer::deserialize(data);

    // Must re-add same Currency objects since deserialization uses IDs
    restored.addCurrency(coin, 0); // ID match
    restored.addCurrency(gem, 0);

    EXPECT_EQ(restored.getQuantity(coin), 25);
    EXPECT_EQ(restored.getQuantity(gem), 40);
}

/**
 * @test Deserialization fails on malformed input.
 */
TEST(CurrencyContainerStandaloneTest, DeserializeInvalidFormatThrows) 
{
    std::string invalid = "BAD_FORMAT[CC_END]";
    EXPECT_THROW(CurrencyContainer::deserialize(invalid), std::invalid_argument);
}

