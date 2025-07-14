/**
 * @file VitalRegistry_T.cpp
 * @author Antonius Torode
 * @date 07/13/2025
 * Description: Google Test file for testing the VitalRegistry class functionality.
 */
#include <gtest/gtest.h>
#include <sstream>
#include "VitalRegistry.hpp"

namespace stats
{
    // Test fixture for VitalRegistry tests, setting up common JSON data.
    class VitalRegistry_T : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            // Sample JSON data for testing, containing two vital entries.
            jsonData = R"({
                "VITAL": [
                    {
                        "id": 1,
                        "name": "Health",
                        "description": "Represents the character's life.",
                        "type": "DEPLETIVE",
                        "baseMax": 100,
                        "baseMin": 0
                    },
                    {
                        "id": 2,
                        "name": "Mana",
                        "description": "Used to cast spells.",
                        "type": "DEPLETIVE",
                        "baseMax": 100,
                        "baseMin": 0
                    }
                ]
            })";
        }

        std::string jsonData; ///< JSON string used for testing VitalRegistry loading.
    };

    // Tests that VitalRegistry is a singleton by ensuring multiple calls to getInstance return the same object.
    TEST_F(VitalRegistry_T, Singleton)
    {
        VitalRegistry& instance1 = VitalRegistry::getInstance();
        VitalRegistry& instance2 = VitalRegistry::getInstance();
        EXPECT_EQ(&instance1, &instance2) << "VitalRegistry should return the same singleton instance";
    }

    // Tests loading valid JSON data into VitalRegistry and verifies that vitals can be retrieved by name.
    TEST_F(VitalRegistry_T, LoadFromString)
    {
        VitalRegistry& registry = VitalRegistry::getInstance();
        ASSERT_NO_THROW(registry.loadFromString(jsonData)) << "Loading valid JSON should not throw";

        const Vital* health = registry.getByName("Health");
        ASSERT_NE(health, nullptr) << "Health vital should be found";
        EXPECT_EQ(health->getID(), 1) << "Health ID should be 1";
        EXPECT_EQ(health->getName(), "Health") << "Health name should match";
        EXPECT_EQ(health->getDescription(), "Represents the character's life.") << "Health description should match";
        EXPECT_EQ(health->getType(), VitalType::DEPLETIVE) << "Health type should be DEPLETIVE";
        EXPECT_EQ(health->getBaseMin(), 0) << "Health baseMin should be 0";
        EXPECT_EQ(health->getBaseMax(), 100) << "Health baseMax should be 100";

        const Vital* mana = registry.getByName("Mana");
        ASSERT_NE(mana, nullptr) << "Mana vital should be found";
        EXPECT_EQ(mana->getID(), 2) << "Mana ID should be 2";
        EXPECT_EQ(mana->getName(), "Mana") << "Mana name should match";
    }

    // Tests retrieving vitals by ID, including handling of non-existent IDs.
    TEST_F(VitalRegistry_T, GetById)
    {
        VitalRegistry& registry = VitalRegistry::getInstance();
        registry.loadFromString(jsonData);

        const Vital* vital = registry.getById(1);
        ASSERT_NE(vital, nullptr) << "Vital with ID 1 should be found";
        EXPECT_EQ(vital->getName(), "Health") << "Vital with ID 1 should be Health";

        vital = registry.getById(999);
        EXPECT_EQ(vital, nullptr) << "Non-existent ID should return nullptr";
    }

    // Tests the dump functionality to ensure all loaded vitals are correctly output as strings.
    TEST_F(VitalRegistry_T, Dump)
    {
        VitalRegistry& registry = VitalRegistry::getInstance();
        registry.loadFromString(jsonData);

        std::ostringstream oss;
        registry.dump(oss);
        std::string output = oss.str();

        EXPECT_TRUE(output.find("Name: Health, Type: DEPLETIVE, BaseMin: 0, BaseMax: 100, Description: Represents the character's life.") != std::string::npos)
            << "Dump should contain Health vital details";
        EXPECT_TRUE(output.find("Name: Mana, Type: DEPLETIVE, BaseMin: 0, BaseMax: 100, Description: Used to cast spells.") != std::string::npos)
            << "Dump should contain Mana vital details";
    }

    // Tests handling of invalid JSON inputs, ensuring appropriate exceptions are thrown.
    TEST_F(VitalRegistry_T, InvalidJson)
    {
        VitalRegistry& registry = VitalRegistry::getInstance();
        std::string invalidJson = R"({"VITAL": "not_an_array"})";
        EXPECT_THROW(registry.loadFromString(invalidJson), std::runtime_error) << "Invalid JSON should throw runtime_error";

        std::string missingKeyJson = R"({"WRONG_KEY": []})";
        EXPECT_THROW(registry.loadFromString(missingKeyJson), std::runtime_error) << "Missing VITAL key should throw runtime_error";
    }
} // namespace stats
