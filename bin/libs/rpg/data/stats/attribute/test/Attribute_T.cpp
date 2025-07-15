/**
 * @file Attribute_T.cpp
 * @author Antonius Torode
 * @date 07/13/2025
 * Description: Google Test file for testing the Attribute class functionality.
 */
#include <gtest/gtest.h>
#include <sstream>
#include "Attribute.hpp"

namespace stats
{
    // Test fixture for Attribute tests, setting up common data.
    class Attribute_T : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            // Sample JSON data for testing serialization/deserialization
            jsonData = R"({
                "id": 1,
                "name": "Strength",
                "description": "Physical power.",
                "baseValue": 10,
                "modifies": [
                    {
                        "targetType": "VITAL",
                        "targetName": "Health",
                        "ModifyType": "ADD",
                        "ModifyValuePer": 5.0
                    }
                ]
            })";

            // Sample Modifies object for testing
            sampleModify = rpg::Modifies(rpg::DataType::VITAL, "Health", rpg::ModifyType::ADD, 5.0);
        }

        std::string jsonData; ///< JSON string used for testing serialization/deserialization.
        rpg::Modifies sampleModify; ///< Sample Modifies object for testing addModify.
    };

    // Tests the default constructor to ensure it initializes an Attribute with default values.
    TEST_F(Attribute_T, DefaultConstructor)
    {
        Attribute attr;
        EXPECT_EQ(attr.getID(), 0) << "Default constructor should set ID to 0";
        EXPECT_EQ(attr.getName(), "") << "Default constructor should set empty name";
        EXPECT_EQ(attr.getDescription(), "") << "Default constructor should set empty description";
        EXPECT_EQ(attr.getBaseValue(), 0) << "Default constructor should set baseValue to 0";
        EXPECT_TRUE(attr.getModifies().empty()) << "Default constructor should set empty modifies vector";
    }

    // Tests the parameterized constructor with and without modifies.
    TEST_F(Attribute_T, ParameterizedConstructor)
    {
        // Test without modifies
        Attribute attr(1, "Strength", "Physical power.", 10);
        EXPECT_EQ(attr.getID(), 1) << "ID should be set to 1";
        EXPECT_EQ(attr.getName(), "Strength") << "Name should be set to Strength";
        EXPECT_EQ(attr.getDescription(), "Physical power.") << "Description should be set correctly";
        EXPECT_EQ(attr.getBaseValue(), 10) << "BaseValue should be set to 10";
        EXPECT_TRUE(attr.getModifies().empty()) << "Modifies should be empty by default";

        // Test with modifies
        std::vector<rpg::Modifies> modifies = {sampleModify};
        Attribute attrWithMod(2, "Constitution", "Endurance.", 15, modifies);
        EXPECT_EQ(attrWithMod.getID(), 2) << "ID should be set to 2";
        EXPECT_EQ(attrWithMod.getName(), "Constitution") << "Name should be set to Constitution";
        EXPECT_EQ(attrWithMod.getBaseValue(), 15) << "BaseValue should be set to 15";
        ASSERT_EQ(attrWithMod.getModifies().size(), 1) << "Modifies vector should contain one element";
        EXPECT_EQ(attrWithMod.getModifies()[0], sampleModify) << "Modifies should match the provided sample";
    }

    // Tests the getter methods to ensure they return the correct values.
    TEST_F(Attribute_T, Getters)
    {
        Attribute attr(1, "Strength", "Physical power.", 10);
        EXPECT_EQ(attr.getID(), 1) << "getID should return the correct ID";
        EXPECT_EQ(attr.getName(), "Strength") << "getName should return the correct name";
        EXPECT_EQ(attr.getDescription(), "Physical power.") << "getDescription should return the correct description";
        EXPECT_EQ(attr.getBaseValue(), 10) << "getBaseValue should return the correct base value";
        EXPECT_TRUE(attr.getModifies().empty()) << "getModifies should return an empty vector";
    }

    // Tests the addModify method to ensure it correctly adds a Modifies object to the vector.
    TEST_F(Attribute_T, AddModify)
    {
        Attribute attr(1, "Strength", "Physical power.", 10);
        attr.addModify(sampleModify);
        ASSERT_EQ(attr.getModifies().size(), 1) << "Modifies vector should contain one element after adding";
        EXPECT_EQ(attr.getModifies()[0], sampleModify) << "Added modify should match the sample";
        
        // Add another modify to test vector growth
        rpg::Modifies anotherMod(rpg::DataType::VITAL, "Mana", rpg::ModifyType::MULTIPLY, 2.0);
        attr.addModify(anotherMod);
        ASSERT_EQ(attr.getModifies().size(), 2) << "Modifies vector should contain two elements";
        EXPECT_EQ(attr.getModifies()[1], anotherMod) << "Second added modify should match";
    }

    // Tests the toJson method to ensure it correctly serializes the Attribute object.
    TEST_F(Attribute_T, ToJson)
    {
        std::vector<rpg::Modifies> modifies = {sampleModify};
        Attribute attr(1, "Strength", "Physical power.", 10, modifies);
        nlohmann::json j = attr.toJson();

        EXPECT_EQ(j["id"], 1) << "JSON should contain correct ID";
        EXPECT_EQ(j["name"], "Strength") << "JSON should contain correct name";
        EXPECT_EQ(j["description"], "Physical power.") << "JSON should contain correct description";
        EXPECT_EQ(j["baseValue"], 10) << "JSON should contain correct baseValue";
        ASSERT_TRUE(j.contains("modifies")) << "JSON should contain modifies array";
        ASSERT_EQ(j["modifies"].size(), 1) << "Modifies array should contain one element";
        EXPECT_EQ(j["modifies"][0]["targetType"], "VITAL") << "Modify targetType should be VITAL";
        EXPECT_EQ(j["modifies"][0]["targetName"], "Health") << "Modify targetName should be Health";
        EXPECT_EQ(j["modifies"][0]["ModifyType"], "ADD") << "Modify type should be ADD";
        EXPECT_DOUBLE_EQ(j["modifies"][0]["ModifyValuePer"], 5.0) << "Modify valuePer should be 5.0";

        // Test without modifies
        Attribute attrNoMod(2, "Dexterity", "Agility.", 5);
        j = attrNoMod.toJson();
        EXPECT_FALSE(j.contains("modifies")) << "JSON should not contain modifies key when empty";
    }

    // Tests the fromJson method to ensure it correctly deserializes a JSON object into an Attribute.
    TEST_F(Attribute_T, FromJson)
    {
        nlohmann::json j = nlohmann::json::parse(jsonData);
        Attribute attr = Attribute::fromJson(j);

        EXPECT_EQ(attr.getID(), 1) << "Deserialized ID should be 1";
        EXPECT_EQ(attr.getName(), "Strength") << "Deserialized name should be Strength";
        EXPECT_EQ(attr.getDescription(), "Physical power.") << "Deserialized description should match";
        EXPECT_EQ(attr.getBaseValue(), 10) << "Deserialized baseValue should be 10";
        ASSERT_EQ(attr.getModifies().size(), 1) << "Deserialized modifies vector should contain one element";
        EXPECT_EQ(attr.getModifies()[0], sampleModify) << "Deserialized modify should match sample";

        // Test with JSON missing optional modifies
        std::string noModJson = R"({"id": 2, "name": "Dexterity", "description": "Agility.", "baseValue": 5})";
        j = nlohmann::json::parse(noModJson);
        attr = Attribute::fromJson(j);
        EXPECT_TRUE(attr.getModifies().empty()) << "Deserialized modifies should be empty when not provided";
    }

    // Tests that fromJson throws an exception for invalid JSON missing required fields.
    TEST_F(Attribute_T, FromJsonInvalid)
    {
        std::string invalidJson = R"({"name": "Strength", "description": "Physical power."})"; // Missing id, baseValue
        nlohmann::json j = nlohmann::json::parse(invalidJson);
        EXPECT_THROW(Attribute::fromJson(j), nlohmann::json::out_of_range) << "Missing required fields should throw out_of_range";
    }
} // namespace stats
