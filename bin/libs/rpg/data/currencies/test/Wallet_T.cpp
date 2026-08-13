/**
 * @file Wallet_T.cpp
 * @author Antonius Torode
 * @date 07/09/2025
 * @brief Unit tests for the currency::Wallet class using Google Test.
 *
 * The tests exercises Wallet operations across all three lookup styles (by object, by name, and by ID).
 *
 * Currency definitions are loaded into the shared CurrencyRegistry in SetUp(); wallet quantities live 
 * in the per-test `container` member, which gtest reconstructs fresh for each TEST_F.
 */

#include <gtest/gtest.h>

#include "Wallet.hpp"
#include "CurrencyRegistry.hpp"
#include "MIAException.hpp"


namespace currency
{
    /**
     * @brief Test fixture for the Wallet tests.
     *
     * Holds the per-test `container` Wallet plus two sample Currency definitions
     * (coin, gem) used as add/get/has targets. SetUp() loads those definitions
     * into the shared CurrencyRegistry so name and ID based lookups resolve
     * during each test; `container` itself is reconstructed fresh by gtest for
     * every TEST_F, so wallet quantities never carry over between cases.
     */
    class Wallet_T : public ::testing::Test 
    {
    protected:
        /**
         * @brief Populates the shared CurrencyRegistry before each test.
         *
         * CurrencyRegistry is a process-wide singleton, so without resetting it here a
         * test's contents would leak into whichever test runs next. Reloading from a
         * fixed JSON payload in SetUp() guarantees every test starts from the same known
         * registry state regardless of run order. This only governs the currency
         * *definitions* (the coin/gem lookups used by add/get/has by name or ID); the
         * Wallet's own quantities live in the per-test `container` member, which gtest
         * reconstructs fresh for each TEST_F.
         */
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

    /**
     * @brief Verifies add by Currency object accumulates quantity across successive calls.
     */
    TEST_F(Wallet_T, AddByCurrencyIncreasesQuantity) 
    {
        container.add(coin, 100);
        EXPECT_EQ(container.get(coin).getQuantity(), 100) 
            << "add(coin, 100) should set quantity to 100.";
        
        container.add(coin, 50);
        EXPECT_EQ(container.get(coin).getQuantity(), 150) 
            << "add(coin, 50) should increase quantity to 150.";
    }

    /**
     * @brief Verifies add by name accumulates quantity and throws for a non-existent currency name.
     */
    TEST_F(Wallet_T, AddByNameIncreasesQuantity) 
    {
        container.add("Coin", 100);
        EXPECT_EQ(container.get("Coin").getQuantity(), 100) 
            << "add('Coin', 100) should set quantity to 100.";
        
        container.add("Coin", 50);
        EXPECT_EQ(container.get("Coin").getQuantity(), 150) 
            << "add('Coin', 50) should increase quantity to 150.";
        
        EXPECT_THROW(container.add("NonExistent", 10), error::MIAException)
            << "add for non-existent name should throw MIAException.";
    }

    /**
     * @brief Verifies add by ID accumulates quantity and throws for a non-existent currency ID.
     */
    TEST_F(Wallet_T, AddByIdIncreasesQuantity) 
    {
        container.add(1, 100);
        EXPECT_EQ(container.get(1).getQuantity(), 100) 
            << "add(1, 100) should set quantity to 100.";
        
        container.add(1, 50);
        EXPECT_EQ(container.get(1).getQuantity(), 150) 
            << "add(1, 50) should increase quantity to 150.";
        
        EXPECT_THROW(container.add(999, 10), error::MIAException)
            << "add for non-existent ID should throw MIAException.";
    }

    /**
     * @brief Verifies adding past the unsigned int maximum throws std::overflow_error.
     */
    TEST_F(Wallet_T, AddOverflowThrows) 
    {
        container.add(coin, std::numeric_limits<unsigned int>::max());
        EXPECT_THROW(container.add(coin, 1), std::overflow_error)
            << "Adding beyond max unsigned int should throw overflow_error.";
    }

    /**
     * @brief Verifies remove by Currency object clears the entry, leaving get at zero.
     */
    TEST_F(Wallet_T, RemoveByCurrencyRemovesEntry) 
    {
        container.add(coin, 80);
        container.remove(coin);
        EXPECT_EQ(container.get(coin).getQuantity(), 0) 
            << "remove(coin) should remove entry, get returns 0.";
    }

    /**
     * @brief Verifies remove by name clears the entry and throws for a non-existent currency name.
     */
    TEST_F(Wallet_T, RemoveByNameRemovesEntry) 
    {
        container.add("Coin", 80);
        container.remove("Coin");
        EXPECT_EQ(container.get("Coin").getQuantity(), 0) 
            << "remove('Coin') should remove entry, get returns 0.";
        
        EXPECT_THROW(container.remove("NonExistent"), error::MIAException)
            << "remove for non-existent name should throw MIAException.";
    }

    /**
     * @brief Verifies remove by ID clears the entry and throws for a non-existent currency ID.
     */
    TEST_F(Wallet_T, RemoveByIdRemovesEntry) 
    {
        container.add(1, 80);
        container.remove(1);
        EXPECT_EQ(container.get(1).getQuantity(), 0) 
            << "remove(1) should remove entry, get returns 0.";
        
        EXPECT_THROW(container.remove(999), error::MIAException)
            << "remove for non-existent ID should throw MIAException.";
    }

    /**
     * @brief Verifies CurrencyQuantity::remove decreases quantity and clamps to zero when exceeded.
     */
    TEST_F(Wallet_T, CurrencyQuantityRemoveDecreasesQuantity) 
    {
        container.add(gem, 100);
        container.get(gem).remove(30);
        EXPECT_EQ(container.get(gem).getQuantity(), 70) 
            << "CurrencyQuantity::remove(30) should decrease quantity to 70.";
        
        container.get(gem).remove(100); // Exceeds quantity
        EXPECT_EQ(container.get(gem).getQuantity(), 0) 
            << "CurrencyQuantity::remove(100) should set quantity to 0.";
    }

    /**
     * @brief Verifies update by Currency object overwrites quantity and adds the entry when absent.
     */
    TEST_F(Wallet_T, UpdateByCurrencySetsQuantity) 
    {
        container.add(coin, 100);
        container.update(coin, 50);
        EXPECT_EQ(container.get(coin).getQuantity(), 50) 
            << "update(coin, 50) should set quantity to 50.";
        
        container.update(gem, 25); // Non-existent, should add
        EXPECT_EQ(container.get(gem).getQuantity(), 25) 
            << "update(gem, 25) should add and set quantity to 25.";
    }

    /**
     * @brief Verifies update by name overwrites quantity and throws for a non-existent currency name.
     */
    TEST_F(Wallet_T, UpdateByNameSetsQuantity) 
    {
        container.add("Coin", 100);
        container.update("Coin", 50);
        EXPECT_EQ(container.get("Coin").getQuantity(), 50) 
            << "update('Coin', 50) should set quantity to 50.";
        
        EXPECT_THROW(container.update("NonExistent", 25), error::MIAException)
            << "update for non-existent name should throw MIAException.";
    }

    /**
     * @brief Verifies update by ID overwrites quantity and throws for a non-existent currency ID.
     */
    TEST_F(Wallet_T, UpdateByIdSetsQuantity) 
    {
        container.add(1, 100);
        container.update(1, 50);
        EXPECT_EQ(container.get(1).getQuantity(), 50) 
            << "update(1, 50) should set quantity to 50.";
        
        EXPECT_THROW(container.update(999, 25), error::MIAException)
            << "update for non-existent ID should throw MIAException.";
    }

    /**
     * @brief Verifies has by Currency object reports sufficient funds and rejects absent currencies.
     */
    TEST_F(Wallet_T, HasByCurrencyWorks) 
    {
        container.add(coin, 75);
        EXPECT_TRUE(container.has(coin, 50)) 
            << "has(coin, 50) should return true.";
        EXPECT_FALSE(container.has(coin, 100)) 
            << "has(coin, 100) should return false.";
        EXPECT_FALSE(container.has(gem, 1)) 
            << "has(gem, 1) should return false for non-existent currency.";
    }

    /**
     * @brief Verifies has by name reports sufficient funds and throws for a non-existent currency name.
     */
    TEST_F(Wallet_T, HasByNameWorks) 
    {
        container.add("Coin", 75);
        EXPECT_TRUE(container.has("Coin", 50)) 
            << "has('Coin', 50) should return true.";
        EXPECT_FALSE(container.has("Coin", 100)) 
            << "has('Coin', 100) should return false.";
        EXPECT_THROW(container.has("NonExistent", 1), error::MIAException)
            << "has for non-existent name should throw MIAException.";
    }

    /**
     * @brief Verifies has by ID reports sufficient funds and throws for a non-existent currency ID.
     */
    TEST_F(Wallet_T, HasByIdWorks) 
    {
        container.add(1, 75);
        EXPECT_TRUE(container.has(1, 50)) 
            << "has(1, 50) should return true.";
        EXPECT_FALSE(container.has(1, 100)) 
            << "has(1, 100) should return false.";
        EXPECT_THROW(container.has(999, 1), error::MIAException)
            << "has for non-existent ID should throw MIAException.";
    }

    /**
     * @brief Verifies get returns zero quantity for currencies not present in the Wallet.
     */
    TEST_F(Wallet_T, GetForUnknownCurrencyReturnsZero) 
    {
        EXPECT_EQ(container.get(gem).getQuantity(), 0) 
            << "get(gem) should return quantity 0 for non-existent currency.";
        EXPECT_EQ(container.get(2).getQuantity(), 0) 
            << "get(2) should return quantity 0 for non-existent ID.";
    }

    /**
     * @brief Verifies serialize and deserialize round-trip preserves currency quantities.
     */
    TEST_F(Wallet_T, SerializeAndDeserializeRoundTrip) 
    {
        Wallet attr;
        attr.add(coin, 25);
        attr.add(gem, 40);

        std::string serialized = attr.serialize();
        Wallet deserialized = Wallet::deserialize(serialized);

        EXPECT_EQ(deserialized.get(coin).getQuantity(), 25) 
            << "Deserialized coin quantity should be 25.";
        EXPECT_EQ(deserialized.get(gem).getQuantity(), 40) 
            << "Deserialized gem quantity should be 40.";
    }

    /**
     * @brief Verifies deserialize rejects malformed input and yields an empty Wallet for empty content.
     */
    TEST_F(Wallet_T, DeserializeInvalidInput) 
    {
        EXPECT_THROW(Wallet::deserialize("BAD_FORMAT"), std::invalid_argument)
            << "Deserialize with missing markers should throw invalid_argument.";
        
        Wallet deserialized = Wallet::deserialize("[WALLET_BEGIN][WALLET_END]");
        EXPECT_EQ(deserialized.get(coin).getQuantity(), 0) 
            << "Deserialize with empty content should return empty Wallet.";
    }
} // namespace currency
