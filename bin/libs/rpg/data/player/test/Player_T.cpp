/**
 * @file Player_T.cpp
 * @author Antonius Torode
 * @date 10/09/2026
 * @brief Unit tests for the rpg::Player class/features.
 *
 * The tests cover the accessor references and the file save/load behavior.
 * The registry loads in SetUp() provide the currency, vital, attribute, and
 * progress marker definitions that name and ID based lookups and the
 * deserializers resolve against.
 */

#include <cstdio>
#include <fstream>
#include <sstream>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// Include the associated header file for methods to test.
#include "Player.hpp"
// Used for the registries that deserialization resolves definitions against.
#include "CurrencyRegistry.hpp"
#include "VitalRegistry.hpp"
#include "AttributeRegistry.hpp"
#include "ProgressRegistry.hpp"

namespace rpg
{
    /**
     * @brief Test fixture for the Player tests.
     *
     * Holds one populated player (`player`) whose wallet, vitals, attributes,
     * and progress markers are filled in SetUp(). SetUp() also loads fixed
     * definitions into the four shared registries so name and ID based lookups
     * resolve and loadFromFile() can reconstruct containers from saved IDs.
     */
    class Player_T : public ::testing::Test
    {
    protected:
        /**
         * @brief Loads fixed definitions into the shared registries and populates the test player.
         */
        void SetUp() override
        {
            nlohmann::json currencyJson;
            currencyJson["CURRENCY"] = { coin.toJson(), gem.toJson() };
            currency::CurrencyRegistry::getInstance().loadFromString(currencyJson.dump());

            nlohmann::json vitalJson;
            vitalJson["VITAL"] = { health.toJson(), mana.toJson() };
            stats::VitalRegistry::getInstance().loadFromString(vitalJson.dump());

            nlohmann::json attributeJson;
            attributeJson["ATTRIBUTE"] = { strength.toJson(), wisdom.toJson() };
            stats::AttributeRegistry::getInstance().loadFromString(attributeJson.dump());

            nlohmann::json progressJson;
            progressJson["PROGRESS"] = { quest1.toJson(), quest2.toJson() };
            progress::ProgressRegistry::getInstance().loadFromString(progressJson.dump());

            populate(player);
        }

        /**
         * @brief Fills the player's wallet, vitals, attributes, and progress markers.
         * @param target The player to populate.
         */
        void populate(Player& target)
        {
            target.getWallet().add(coin, 100);
            target.getWallet().add(gem, 25);

            target.getVitals().add(health, 80, 0, 100);
            target.getVitals().add(mana, 30, 0, 50);

            target.getAttributes().add(strength, 10);
            target.getAttributes().add(wisdom, 7);

            target.getProgress().add(quest1, 5);
            target.getProgress().add(quest2, 12);
        }

        /// The player container used for testing.
        Player player;

        currency::Currency coin {1, "Coin", "Standard in-game coin", currency::CurrencyType::COIN};
        currency::Currency gem  {2, "Gem", "Premium currency", currency::CurrencyType::GEM};

        stats::Vital health {1, "Health", "The health of a player.", stats::VitalType::DEPLETIVE};
        stats::Vital mana   {2, "Mana", "The mana of a player.", stats::VitalType::DEPLETIVE};

        stats::Attribute strength {1, "Strength", "Physical power.", 0};
        stats::Attribute wisdom   {2, "Wisdom", "Mental acuity.", 0};

        progress::ProgressMarker quest1 {1, "Quest1", "First quest progress."};
        progress::ProgressMarker quest2 {2, "Quest2", "Second quest progress."};
    }; // class Player_T

    /**
     * @brief Verifies a default constructed player exposes empty, usable containers.
     */
    TEST_F(Player_T, DefaultConstructorState)
    {
        Player emptyPlayer;

        EXPECT_TRUE(emptyPlayer.getWallet().getMap().empty())
            << "A default player's wallet should be empty.";
        EXPECT_TRUE(emptyPlayer.getVitals().getMap().empty())
            << "A default player's vitals should be empty.";
        EXPECT_TRUE(emptyPlayer.getAttributes().getMap().empty())
            << "A default player's attributes should be empty.";
        EXPECT_TRUE(emptyPlayer.getProgress().getMap().empty())
            << "A default player's progress should be empty.";
    }

    /**
     * @brief Verifies the accessors return references to the player's own containers.
     *
     * changes through one accessor must be visible through the same container
     * checked by a different lookup style (object, name, and ID).
     */
    TEST_F(Player_T, AccessorsReturnReferences)
    {
        player.getWallet().add(coin, 50);
        EXPECT_EQ(player.getWallet().get(1).getQuantity(), 150)
            << "A wallet change through the accessor should be visible by ID lookup.";

        player.getVitals().update(health, 55);
        EXPECT_EQ(player.getVitals().get("Health").getCurrent(), 55)
            << "A vitals change through the accessor should be visible by name lookup.";

        player.getAttributes().update(wisdom, 9);
        EXPECT_EQ(player.getAttributes().get(2).getBaseValue(), 9)
            << "An attributes change through the accessor should be visible by ID lookup.";

        player.getProgress().update(quest1, 7);
        EXPECT_EQ(player.getProgress().get("Quest1").get(), 7)
            << "A progress change through the accessor should be visible by name lookup.";
    }

    /**
     * @brief Verifies saveToFile writes a file containing all four serialized blocks.
     */
    TEST_F(Player_T, SaveToFileWritesAllBlocks)
    {
        const std::string filename = "playerSaveBlocksTest.MIA";
        EXPECT_TRUE(player.saveToFile(filename))
            << "saveToFile should succeed for a writable path.";

        std::ifstream file(filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        std::string contents = buffer.str();

        EXPECT_NE(contents.find("[ATTRIBUTES_BEGIN]"), std::string::npos)
            << "The saved file should contain an attributes block.";
        EXPECT_NE(contents.find("[VITALS_BEGIN]"), std::string::npos)
            << "The saved file should contain a vitals block.";
        EXPECT_NE(contents.find("[WALLET_BEGIN]"), std::string::npos)
            << "The saved file should contain a wallet block.";
        EXPECT_NE(contents.find("[PROGRESS_BEGIN]"), std::string::npos)
            << "The saved file should contain a progress block.";

        std::remove(filename.c_str());
    }

    /**
     * @brief Verifies a save/load round trip restores all four containers.
     */
    TEST_F(Player_T, SaveLoadRoundTrip)
    {
        const std::string filename = "playerRoundTripTest.MIA";
        ASSERT_TRUE(player.saveToFile(filename))
            << "saveToFile should succeed for a writable path.";

        Player loaded;
        EXPECT_TRUE(loaded.loadFromFile(filename))
            << "loadFromFile should succeed for a file saved by saveToFile.";

        EXPECT_EQ(loaded.getWallet().get(coin).getQuantity(), 100)
            << "The wallet quantity should survive the round trip.";
        EXPECT_EQ(loaded.getWallet().get(gem).getQuantity(), 25)
            << "The wallet quantity should survive the round trip.";

        EXPECT_EQ(loaded.getVitals().get(health).getCurrent(), 80)
            << "The vital current value should survive the round trip.";
        EXPECT_EQ(loaded.getVitals().get(health).getCurrentMin(), 0)
            << "The vital minimum should survive the round trip.";
        EXPECT_EQ(loaded.getVitals().get(health).getCurrentMax(), 100)
            << "The vital maximum should survive the round trip.";
        EXPECT_EQ(loaded.getVitals().get(mana).getCurrent(), 30)
            << "The vital current value should survive the round trip.";

        EXPECT_EQ(loaded.getAttributes().get(strength).getBaseValue(), 10)
            << "The attribute base value should survive the round trip.";
        EXPECT_EQ(loaded.getAttributes().get(wisdom).getBaseValue(), 7)
            << "The attribute base value should survive the round trip.";

        EXPECT_EQ(loaded.getProgress().get(quest1).get(), 5)
            << "The progress value should survive the round trip.";
        EXPECT_EQ(loaded.getProgress().get(quest2).get(), 12)
            << "The progress value should survive the round trip.";

        std::remove(filename.c_str());
    }

    /**
     * @brief Verifies saveToFile returns false when the file cannot be opened.
     */
    TEST_F(Player_T, SaveToFileFailsForUnopenablePath)
    {
        EXPECT_FALSE(player.saveToFile("/no_such_directory/playerSaveTest.MIA"))
            << "saveToFile should fail for a path whose directory does not exist.";
    }

    /**
     * @brief Verifies loadFromFile returns false when the file does not exist.
     */
    TEST_F(Player_T, LoadFromFileFailsForMissingFile)
    {
        Player loaded;
        EXPECT_FALSE(loaded.loadFromFile("/no_such_directory/playerLoadTest.MIA"))
            << "loadFromFile should fail for a missing file.";
    }

    /**
     * @brief Verifies loadFromFile returns false for a file with no serialized blocks.
     */
    TEST_F(Player_T, LoadFromFileFailsForCorruptFile)
    {
        const std::string filename = "playerCorruptTest.MIA";
        {
            std::ofstream file(filename);
            file << "this file contains no serialized blocks" << std::endl;
        }

        Player loaded;
        EXPECT_FALSE(loaded.loadFromFile(filename))
            << "loadFromFile should fail when no serialized blocks are present.";

        std::remove(filename.c_str());
    }

    /**
     * @brief Verifies loadFromFile returns false when a later container block is missing.
     *
     * The file holds a valid vitals block, so deserialization fails in the wallet step.
     * This exercises the MIAException path for a partially valid save.
     */
    TEST_F(Player_T, LoadFromFileFailsForPartiallyValidFile)
    {
        const std::string filename = "playerPartiallyValidTest.MIA";
        {
            std::ofstream file(filename);
            file << player.getVitals().serialize() << std::endl;
        }

        Player loaded;
        EXPECT_FALSE(loaded.loadFromFile(filename))
            << "loadFromFile should fail when the wallet block is missing.";

        std::remove(filename.c_str());
    }

} // namespace rpg
