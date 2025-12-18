/**
 * @file ProgressRegistry_T.cpp
 * @author Antonius Torode
 * @date 07/20/2025
 * @brief Unit tests for the ProgressRegistry class using Google Test.
 */

#include <gtest/gtest.h>
#include <sstream>
#include "ProgressRegistry.hpp"

namespace progress
{
    // Test fixture for ProgressRegistry tests, setting up common JSON data.
    class ProgressRegistry_T : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            // Sample JSON data for testing, containing two progress marker entries.
            jsonData = R"({
                "PROGRESS": [
                    {
                        "id": 1,
                        "name": "Quest1",
                        "description": "First quest progress."
                    },
                    {
                        "id": 2,
                        "name": "Quest2",
                        "description": "Second quest progress."
                    }
                ]
            })";
        }

        std::string jsonData; ///< JSON string used for testing ProgressRegistry loading.
    };

    // Tests that ProgressRegistry is a singleton by ensuring multiple calls to getInstance return the same object.
    TEST_F(ProgressRegistry_T, Singleton)
    {
        ProgressRegistry& instance1 = ProgressRegistry::getInstance();
        ProgressRegistry& instance2 = ProgressRegistry::getInstance();
        EXPECT_EQ(&instance1, &instance2) << "ProgressRegistry should return the same singleton instance";
    }

    // Tests loading valid JSON data into ProgressRegistry and verifies progress markers can be retrieved by name.
    TEST_F(ProgressRegistry_T, LoadFromString)
    {
        ProgressRegistry& registry = ProgressRegistry::getInstance();
        ASSERT_NO_THROW(registry.loadFromString(jsonData)) << "Loading valid JSON should not throw";

        const ProgressMarker* quest1 = registry.getByName("Quest1");
        ASSERT_NE(quest1, nullptr) << "Quest1 progress marker should be found";
        EXPECT_EQ(quest1->getID(), 1) << "Quest1 ID should be 1";
        EXPECT_EQ(quest1->getName(), "Quest1") << "Quest1 name should match";
        EXPECT_EQ(quest1->getDescription(), "First quest progress.") << "Quest1 description should match";

        const ProgressMarker* quest2 = registry.getByName("Quest2");
        ASSERT_NE(quest2, nullptr) << "Quest2 progress marker should be found";
        EXPECT_EQ(quest2->getID(), 2) << "Quest2 ID should be 2";
        EXPECT_EQ(quest2->getName(), "Quest2") << "Quest2 name should match";
        EXPECT_EQ(quest2->getDescription(), "Second quest progress.") << "Quest2 description should match";
    }

    // Tests retrieving progress markers by ID, including handling of non-existent IDs.
    TEST_F(ProgressRegistry_T, getByID)
    {
        ProgressRegistry& registry = ProgressRegistry::getInstance();
        registry.loadFromString(jsonData);

        const ProgressMarker* marker = registry.getByID(1);
        ASSERT_NE(marker, nullptr) << "Progress marker with ID 1 should be found";
        EXPECT_EQ(marker->getName(), "Quest1") << "Progress marker with ID 1 should be Quest1";

        marker = registry.getByID(999);
        EXPECT_EQ(marker, nullptr) << "Non-existent ID should return nullptr";
    }

    // Tests the dump functionality to ensure all loaded progress markers are correctly output.
    TEST_F(ProgressRegistry_T, Dump)
    {
        ProgressRegistry& registry = ProgressRegistry::getInstance();
        registry.loadFromString(jsonData);

        std::ostringstream oss;
        registry.dump(oss);
        std::string output = oss.str();

        EXPECT_TRUE(output.find("Name: Quest1, Description: First quest progress.") != std::string::npos)
            << "Dump should contain Quest1 progress marker details";
        EXPECT_TRUE(output.find("Name: Quest2, Description: Second quest progress.") != std::string::npos)
            << "Dump should contain Quest2 progress marker details";
    }

    // Tests handling of invalid JSON inputs, ensuring appropriate exceptions are thrown.
    TEST_F(ProgressRegistry_T, InvalidJson)
    {
        ProgressRegistry& registry = ProgressRegistry::getInstance();
        std::string invalidJson = R"({"PROGRESS": "not_an_array"})";
        EXPECT_THROW(registry.loadFromString(invalidJson), std::runtime_error) << "Invalid JSON should throw runtime_error";

        std::string missingKeyJson = R"({"WRONG_KEY": []})";
        EXPECT_THROW(registry.loadFromString(missingKeyJson), std::runtime_error) << "Missing PROGRESS key should throw runtime_error";
    }

    // Tests loading JSON with missing description field to ensure default handling.
    TEST_F(ProgressRegistry_T, LoadFromStringMissingDescription)
    {
        std::string jsonDataMissingDesc = R"({
            "PROGRESS": [
                {
                    "id": 3,
                    "name": "Quest3"
                }
            ]
        })";
        ProgressRegistry& registry = ProgressRegistry::getInstance();
        ASSERT_NO_THROW(registry.loadFromString(jsonDataMissingDesc)) << "Loading JSON with missing description should not throw";

        const ProgressMarker* quest3 = registry.getByName("Quest3");
        ASSERT_NE(quest3, nullptr) << "Quest3 progress marker should be found";
        EXPECT_EQ(quest3->getID(), 3) << "Quest3 ID should be 3";
        EXPECT_EQ(quest3->getName(), "Quest3") << "Quest3 name should match";
        EXPECT_EQ(quest3->getDescription(), "") << "Quest3 description should be empty";
    }

} // namespace progress
