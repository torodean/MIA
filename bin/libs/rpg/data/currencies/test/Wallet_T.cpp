/**
 * @file Wallet_T.cpp
 * @author Antonius Torode
 * @date 07/09/2025
 * Description: Google Test suite for the Wallet class.
 */

#include <gtest/gtest.h>

#include "Wallet.hpp"
#include "CurrencyRegistry.hpp"
#include "MIAException.hpp"


namespace currency
{
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
    }; // class Wallet_T
    
    // Test adding currency by Currency object increases quantity
    TEST_F(Wallet_T, AddByCurrencyIncreasesQuantity) 
    {
        container.add(coin, 100);
        EXPECT_EQ(container.get(coin).getQuantity(), 100) << "add(coin, 100) should set quantity to 100.";
        
        container.add(coin, 50);
        EXPECT_EQ(container.get(coin).getQuantity(), 150) << "add(coin, 50) should increase quantity to 150.";
    }

    // Test adding currency by name increases quantity
    TEST_F(Wallet_T, AddByNameIncreasesQuantity) 
    {
        container.add("Coin", 100);
        EXPECT_EQ(container.get("Coin").getQuantity(), 100) << "add('Coin', 100) should set quantity to 100.";
        
        container.add("Coin", 50);
        EXPECT_EQ(container.get("Coin").getQuantity(), 150) << "add('Coin', 50) should increase quantity to 150.";
        
        EXPECT_THROW(container.add("NonExistent", 10), error::MIAException)
            << "add for non-existent name should throw MIAException.";
    }

    // Test adding currency by ID increases quantity
    TEST_F(Wallet_T, AddByIdIncreasesQuantity) 
    {
        container.add(1, 100);
        EXPECT_EQ(container.get(1).getQuantity(), 100) << "add(1, 100) should set quantity to 100.";
        
        container.add(1, 50);
        EXPECT_EQ(container.get(1).getQuantity(), 150) << "add(1, 50) should increase quantity to 150.";
        
        EXPECT_THROW(container.add(999, 10), error::MIAException)
            << "add for non-existent ID should throw MIAException.";
    }

    // Test adding currency with overflow throws
    TEST_F(Wallet_T, AddOverflowThrows) 
    {
        container.add(coin, std::numeric_limits<unsigned int>::max());
        EXPECT_THROW(container.add(coin, 1), std::overflow_error)
            << "Adding beyond max unsigned int should throw overflow_error.";
    }

    // Test removing currency by Currency object
    TEST_F(Wallet_T, RemoveByCurrencyRemovesEntry) 
    {
        container.add(coin, 80);
        container.remove(coin);
        EXPECT_EQ(container.get(coin).getQuantity(), 0) << "remove(coin) should remove entry, get returns 0.";
    }

    // Test removing currency by name
    TEST_F(Wallet_T, RemoveByNameRemovesEntry) 
    {
        container.add("Coin", 80);
        container.remove("Coin");
        EXPECT_EQ(container.get("Coin").getQuantity(), 0) << "remove('Coin') should remove entry, get returns 0.";
        
        EXPECT_THROW(container.remove("NonExistent"), error::MIAException)
            << "remove for non-existent name should throw MIAException.";
    }

    // Test removing currency by ID
    TEST_F(Wallet_T, RemoveByIdRemovesEntry) 
    {
        container.add(1, 80);
        container.remove(1);
        EXPECT_EQ(container.get(1).getQuantity(), 0) << "remove(1) should remove entry, get returns 0.";
        
        EXPECT_THROW(container.remove(999), error::MIAException)
            << "remove for non-existent ID should throw MIAException.";
    }

    // Test CurrencyQuantity::remove decreases quantity
    TEST_F(Wallet_T, CurrencyQuantityRemoveDecreasesQuantity) 
    {
        container.add(gem, 100);
        container.get(gem).remove(30);
        EXPECT_EQ(container.get(gem).getQuantity(), 70) << "CurrencyQuantity::remove(30) should decrease quantity to 70.";
        
        container.get(gem).remove(100); // Exceeds quantity
        EXPECT_EQ(container.get(gem).getQuantity(), 0) << "CurrencyQuantity::remove(100) should set quantity to 0.";
    }

    // Test updating currency by Currency object
    TEST_F(Wallet_T, UpdateByCurrencySetsQuantity) 
    {
        container.add(coin, 100);
        container.update(coin, 50);
        EXPECT_EQ(container.get(coin).getQuantity(), 50) << "update(coin, 50) should set quantity to 50.";
        
        container.update(gem, 25); // Non-existent, should add
        EXPECT_EQ(container.get(gem).getQuantity(), 25) << "update(gem, 25) should add and set quantity to 25.";
    }

    // Test updating currency by name
    TEST_F(Wallet_T, UpdateByNameSetsQuantity) 
    {
        container.add("Coin", 100);
        container.update("Coin", 50);
        EXPECT_EQ(container.get("Coin").getQuantity(), 50) << "update('Coin', 50) should set quantity to 50.";
        
        EXPECT_THROW(container.update("NonExistent", 25), error::MIAException)
            << "update for non-existent name should throw MIAException.";
    }

    // Test updating currency by ID
    TEST_F(Wallet_T, UpdateByIdSetsQuantity) 
    {
        container.add(1, 100);
        container.update(1, 50);
        EXPECT_EQ(container.get(1).getQuantity(), 50) << "update(1, 50) should set quantity to 50.";
        
        EXPECT_THROW(container.update(999, 25), error::MIAException)
            << "update for non-existent ID should throw MIAException.";
    }

    // Test checking if Wallet has enough currency by Currency object
    TEST_F(Wallet_T, HasByCurrencyWorks) 
    {
        container.add(coin, 75);
        EXPECT_TRUE(container.has(coin, 50)) << "has(coin, 50) should return true.";
        EXPECT_FALSE(container.has(coin, 100)) << "has(coin, 100) should return false.";
        EXPECT_FALSE(container.has(gem, 1)) << "has(gem, 1) should return false for non-existent currency.";
    }

    // Test checking if Wallet has enough currency by name
    TEST_F(Wallet_T, HasByNameWorks) 
    {
        container.add("Coin", 75);
        EXPECT_TRUE(container.has("Coin", 50)) << "has('Coin', 50) should return true.";
        EXPECT_FALSE(container.has("Coin", 100)) << "has('Coin', 100) should return false.";
        EXPECT_THROW(container.has("NonExistent", 1), error::MIAException)
            << "has for non-existent name should throw MIAException.";
    }

    // Test checking if Wallet has enough currency by ID
    TEST_F(Wallet_T, HasByIdWorks) 
    {
        container.add(1, 75);
        EXPECT_TRUE(container.has(1, 50)) << "has(1, 50) should return true.";
        EXPECT_FALSE(container.has(1, 100)) << "has(1, 100) should return false.";
        EXPECT_THROW(container.has(999, 1), error::MIAException)
            << "has for non-existent ID should throw MIAException.";
    }

    // Test getting quantity of unknown currency returns zero
    TEST_F(Wallet_T, GetForUnknownCurrencyReturnsZero) 
    {
        EXPECT_EQ(container.get(gem).getQuantity(), 0) << "get(gem) should return quantity 0 for non-existent currency.";
        EXPECT_EQ(container.get(2).getQuantity(), 0) << "get(2) should return quantity 0 for non-existent ID.";
    }

    // Test serialization and deserialization preserves data
    TEST_F(Wallet_T, SerializeAndDeserializeRoundTrip) 
    {
        Wallet attr;
        attr.add(coin, 25);
        attr.add(gem, 40);

        std::string serialized = attr.serialize();
        Wallet deserialized = Wallet::deserialize(serialized);

        EXPECT_EQ(deserialized.get(coin).getQuantity(), 25) << "Deserialized coin quantity should be 25.";
        EXPECT_EQ(deserialized.get(gem).getQuantity(), 40) << "Deserialized gem quantity should be 40.";
    }

    // Test deserialization with invalid input
    TEST_F(Wallet_T, DeserializeInvalidInput) 
    {
        EXPECT_THROW(Wallet::deserialize("BAD_FORMAT"), std::invalid_argument)
            << "Deserialize with missing markers should throw invalid_argument.";
        
        Wallet deserialized = Wallet::deserialize("[WALLET_BEGIN][WALLET_END]");
        EXPECT_EQ(deserialized.get(coin).getQuantity(), 0) << "Deserialize with empty content should return empty Wallet.";
    }
} // namespace currency
