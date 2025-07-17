/**
 * @file AttributeRegistry_T.cpp
 * @author Antonius Torode
 * @date 07/13/2025
 * Description: Google Test file for testing the AttributeRegistry class functionality.
 */
#include <gtest/gtest.h>
#include <sstream>
#include "AttributeRegistry.hpp"

namespace stats
{
    // Test fixture for AttributeRegistry tests, setting up common JSON data.
    class AttributeRegistry_T : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            // Sample JSON data for testing, containing two attribute entries with one having a modify.
            jsonData = R"({
                "ATTRIBUTE": [
                    {
                        "id": 1,
                        "name": "Strength",
                        "description": "Physical power.",
                        "baseValue": 10
                    },
                    {
                        "id": 2,
                        "name": "Constitution",
                        "description": "Endurance and resilience.",
                        "baseValue": 15,
                        "modifies": [
                            {
                                "targetType": "VITAL",
                                "targetName": "Health",
                                "ModifyType": "ADD_MAX",
                                "ModifyValuePer": 5.0
                            }
                        ]
                    }
                ]
            })";
        }

        std::string jsonData; ///< JSON string used for testing AttributeRegistry loading.
    };

    // Tests that AttributeRegistry is a singleton by ensuring multiple calls to getInstance return the same object.
    TEST_F(AttributeRegistry_T, Singleton)
    {
        AttributeRegistry& instance1 = AttributeRegistry::getInstance();
        AttributeRegistry& instance2 = AttributeRegistry::getInstance();
        EXPECT_EQ(&instance1, &instance2) << "AttributeRegistry should return the same singleton instance";
    }

    // Tests loading valid JSON data into AttributeRegistry and verifies attributes and their modifies can be retrieved.
    TEST_F(AttributeRegistry_T, LoadFromString)
    {
        AttributeRegistry& registry = AttributeRegistry::getInstance();
        ASSERT_NO_THROW(registry.loadFromString(jsonData)) << "Loading valid JSON should not throw";

        const Attribute* strength = registry.getByName("Strength");
        ASSERT_NE(strength, nullptr) << "Strength attribute should be found";
        EXPECT_EQ(strength->getID(), 1) << "Strength ID should be 1";
        EXPECT_EQ(strength->getName(), "Strength") << "Strength name should match";
        EXPECT_EQ(strength->getDescription(), "Physical power.") << "Strength description should match";
        EXPECT_EQ(strength->getBaseValue(), 10) << "Strength baseValue should be 10";
        EXPECT_TRUE(strength->getModifies().empty()) << "Strength should have no modifies";

        const Attribute* constitution = registry.getByName("Constitution");
        ASSERT_NE(constitution, nullptr) << "Constitution attribute should be found";
        EXPECT_EQ(constitution->getID(), 2) << "Constitution ID should be 2";
        EXPECT_EQ(constitution->getName(), "Constitution") << "Constitution name should match";
        EXPECT_EQ(constitution->getBaseValue(), 15) << "Constitution baseValue should be 15";
        ASSERT_EQ(constitution->getModifies().size(), 1) << "Constitution should have one modify";
        const auto& mod = constitution->getModifies()[0];
        EXPECT_EQ(mod.targetType, rpg::DataType::VITAL) << "Modify targetType should be VITAL";
        EXPECT_EQ(mod.targetName, "Health") << "Modify targetName should be Health";
        EXPECT_EQ(mod.modifyType, rpg::ModifyType::ADD_MAX) << "Modify type should be ADD_MAX";
        EXPECT_DOUBLE_EQ(mod.modifyValuePer, 5.0) << "Modify valuePer should be 5.0";
    }

    // Tests retrieving attributes by ID, including handling of non-existent IDs.
    TEST_F(AttributeRegistry_T, getByID)
    {
        AttributeRegistry& registry = AttributeRegistry::getInstance();
        registry.loadFromString(jsonData);

        const Attribute* attribute = registry.getByID(1);
        ASSERT_NE(attribute, nullptr) << "Attribute with ID 1 should be found";
        EXPECT_EQ(attribute->getName(), "Strength") << "Attribute with ID 1 should be Strength";

        attribute = registry.getByID(999);
        EXPECT_EQ(attribute, nullptr) << "Non-existent ID should return nullptr";
    }

    // Tests the dump functionality to ensure all loaded attributes and their modifies are correctly output.
    TEST_F(AttributeRegistry_T, Dump)
    {
        AttributeRegistry& registry = AttributeRegistry::getInstance();
        registry.loadFromString(jsonData);

        std::ostringstream oss;
        registry.dump(oss);
        std::string output = oss.str();

        EXPECT_TRUE(output.find("Name: Strength, Description: Physical power., BaseValue: 10") != std::string::npos)
            << "Dump should contain Strength attribute details";
        EXPECT_TRUE(output.find("Name: Constitution, Description: Endurance and resilience., BaseValue: 15, Modifies: [Modifies{targetType=VITAL, targetName=Health, modifyType=ADD_MAX, modifyValuePer=5}") != std::string::npos)
            << "Dump should contain Constitution attribute details with modifies";
    }

    // Tests handling of invalid JSON inputs, ensuring appropriate exceptions are thrown.
    TEST_F(AttributeRegistry_T, InvalidJson)
    {
        AttributeRegistry& registry = AttributeRegistry::getInstance();
        std::string invalidJson = R"({"ATTRIBUTE": "not_an_array"})";
        EXPECT_THROW(registry.loadFromString(invalidJson), std::runtime_error) << "Invalid JSON should throw runtime_error";

        std::string missingKeyJson = R"({"WRONG_KEY": []})";
        EXPECT_THROW(registry.loadFromString(missingKeyJson), std::runtime_error) << "Missing ATTRIBUTE key should throw runtime_error";
    }
} // namespace stats
