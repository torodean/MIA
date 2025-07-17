/**
 * @file Wallet_T.cpp
 * @author Antonius Torode
 * @date 07/09/2025
 * Description: Google Test suite for the Wallet class.
 */

#include <gtest/gtest.h>
#include "Wallet.hpp"
#include "CurrencyRegistry.hpp"

using namespace currency;

/**
 * Test fixture for Wallet tests.
 */
class Wallet_T : public ::testing::Test 
{
protected:
    void SetUp() override
    {
    
        // Create a JSON object with the "currency" key containing the array of currencies
        nlohmann::json jsonObject;
        jsonObject["CURRENCY"] = { coin.toJson(), gem.toJson() };
        std::string jsonData = jsonObject.dump();
        
        // Load currencies into registry
        currency::CurrencyRegistry::getInstance().loadFromString(jsonData);
    }
    
    Wallet container;

    Currency coin {1, "Coin", "Standard in-game coin", CurrencyType::COIN};
    Currency gem  {2, "Gem", "Premium currency", CurrencyType::GEM};
};

/**
 * @test Verify currency is added and quantity is tracked correctly.
 */
TEST_F(Wallet_T, addIncreasesQuantity) 
{
    container.add(coin, 100);
    EXPECT_EQ(container.get(coin), 100);

    container.add(coin, 50);
    EXPECT_EQ(container.get(coin), 150);
}

/**
 * @test Removing currency decreases quantity correctly.
 */
TEST_F(Wallet_T, removeDecreasesQuantity) 
{
    container.add(coin, 80);
    EXPECT_TRUE(container.remove(coin, 30));
    EXPECT_EQ(container.get(coin), 50);
}

/**
 * @test Cannot remove more currency than is present.
 */
TEST_F(Wallet_T, RemoveFailsIfInsufficientQuantity) 
{
    container.add(gem, 10);
    EXPECT_FALSE(container.remove(gem, 20));
    EXPECT_EQ(container.get(gem), 10);
}

/**
 * @test Checking if container has enough currency.
 */
TEST_F(Wallet_T, hasCheckWorks) 
{
    container.add(coin, 75);
    EXPECT_TRUE(container.has(coin, 50));
    EXPECT_FALSE(container.has(coin, 100));
}

/**
 * @test Getting quantity of unknown currency returns zero.
 */
TEST_F(Wallet_T, getForUnknownCurrencyReturnsZero) 
{
    EXPECT_EQ(container.get(gem), 0);
}

/**
 * @test Serializing and deserializing preserves data.
 */
TEST_F(Wallet_T, SerializeAndDeserializeRoundTrip) 
{    
    container.add(coin, 25);
    container.add(gem, 40);

    std::string data = container.serialize();
    Wallet restored = Wallet::deserialize(data);

    // Must re-add same Currency objects since deserialization uses IDs
    restored.add(coin, 0); // ID match
    restored.add(gem, 0);

    EXPECT_EQ(restored.get(coin), 25);
    EXPECT_EQ(restored.get(gem), 40);
}

/**
 * @test Deserialization fails on malformed input.
 */
TEST(WalletStandaloneTest, DeserializeInvalidFormatThrows) 
{
    std::string invalid = "BAD_FORMAT[CC_END]";
    EXPECT_THROW(Wallet::deserialize(invalid), std::invalid_argument);
}

