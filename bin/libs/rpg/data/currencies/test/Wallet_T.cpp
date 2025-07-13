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
TEST_F(Wallet_T, AddCurrencyIncreasesQuantity) 
{
    container.addCurrency(coin, 100);
    EXPECT_EQ(container.getQuantity(coin), 100);

    container.addCurrency(coin, 50);
    EXPECT_EQ(container.getQuantity(coin), 150);
}

/**
 * @test Removing currency decreases quantity correctly.
 */
TEST_F(Wallet_T, RemoveCurrencyDecreasesQuantity) 
{
    container.addCurrency(coin, 80);
    EXPECT_TRUE(container.removeCurrency(coin, 30));
    EXPECT_EQ(container.getQuantity(coin), 50);
}

/**
 * @test Cannot remove more currency than is present.
 */
TEST_F(Wallet_T, RemoveFailsIfInsufficientQuantity) 
{
    container.addCurrency(gem, 10);
    EXPECT_FALSE(container.removeCurrency(gem, 20));
    EXPECT_EQ(container.getQuantity(gem), 10);
}

/**
 * @test Checking if container has enough currency.
 */
TEST_F(Wallet_T, HasCurrencyCheckWorks) 
{
    container.addCurrency(coin, 75);
    EXPECT_TRUE(container.hasCurrency(coin, 50));
    EXPECT_FALSE(container.hasCurrency(coin, 100));
}

/**
 * @test Getting quantity of unknown currency returns zero.
 */
TEST_F(Wallet_T, GetQuantityForUnknownCurrencyReturnsZero) 
{
    EXPECT_EQ(container.getQuantity(gem), 0);
}

/**
 * @test Serializing and deserializing preserves data.
 */
TEST_F(Wallet_T, SerializeAndDeserializeRoundTrip) 
{    
    container.addCurrency(coin, 25);
    container.addCurrency(gem, 40);

    std::string data = container.serialize();
    Wallet restored = Wallet::deserialize(data);

    // Must re-add same Currency objects since deserialization uses IDs
    restored.addCurrency(coin, 0); // ID match
    restored.addCurrency(gem, 0);

    EXPECT_EQ(restored.getQuantity(coin), 25);
    EXPECT_EQ(restored.getQuantity(gem), 40);
}

/**
 * @test Deserialization fails on malformed input.
 */
TEST(WalletStandaloneTest, DeserializeInvalidFormatThrows) 
{
    std::string invalid = "BAD_FORMAT[CC_END]";
    EXPECT_THROW(Wallet::deserialize(invalid), std::invalid_argument);
}

