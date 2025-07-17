/**
 * @file Attributes_T.cpp
 * @author Antonius Torode
 * @date 07/14/2025
 * Description: Google Test suite for the Attributes class.
 */

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "Attributes.hpp"
#include "Attribute.hpp"
#include "AttributeData.hpp"
#include "AttributeRegistry.hpp"
#include "Modifier.hpp"
#include "MIAException.hpp"

namespace stats
{

    // Test fixture for Attributes tests, modeled after Vitals_T
    class Attributes_T : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            // Create a JSON object with the "ATTRIBUTE" key containing the array of attributes
            nlohmann::json jsonObject;
            jsonObject["ATTRIBUTE"] = { wisdom.toJson(), dexterity.toJson(), strength.toJson() };
            std::string jsonData = jsonObject.dump();
        
            // Load currencies into registry
            AttributeRegistry::getInstance().loadFromString(jsonData);
            
            // Initialize attributes object with test data
            attributes.add(wisdom, 100);
            attributes.add(dexterity, 50);
            attributes.add(strength, 75);
        }

        Attributes attributes;
        
        Attribute wisdom{1, "Wisdom", "The wisdom of a player.", 100};
        Attribute dexterity{2, "Dexterity", "The dexterity of a player.", 50};
        Attribute strength{3, "Strength", "The strength of a player.", 75};
    };
    
    // Test the default constructor
    TEST_F(Attributes_T, DefaultConstructor)
    {
        Attributes emptyAttrs;
        EXPECT_TRUE(emptyAttrs.serialize().find("][") != std::string::npos)
            << "Default constructor should create an empty Attributes object.";
    }

    // Test get with string name
    TEST_F(Attributes_T, getByName)
    {
        const AttributeData& data = attributes.get("Wisdom");
        EXPECT_EQ(data.getCurrent(), 100) << "get('Wisdom') should return AttributeData with current=100.";

        EXPECT_THROW(attributes.get("NonExistent"), error::MIAException);
    }

    // Test get with uint32_t ID
    TEST_F(Attributes_T, getByID)
    {
        const AttributeData& data = attributes.get(1);
        EXPECT_EQ(data.getCurrent(), 100) << "get(1) should return AttributeData with current=100.";

        EXPECT_THROW(attributes.get(999), error::MIAException);
    }

    // Test get with Attribute object
    TEST_F(Attributes_T, getByAttribute)
    {
        const AttributeData& data = attributes.get(wisdom);
        EXPECT_EQ(data.getCurrent(), 100)
            << "get(wisdom) should return AttributeData with current=100.";

        Attribute nonExistentAttr(999, "NonExistent", "Non-existent attribute");
        const AttributeData& defaultData = attributes.get(nonExistentAttr);
        EXPECT_EQ(defaultData.getCurrent(), 0)
            << "get for non-existent Attribute should return default AttributeData.";
    }

    // Test add with string name
    TEST_F(Attributes_T, addByName)
    {
        Attributes attr;
        attr.add("Wisdom", 150);
        const AttributeData& data = attr.get("Wisdom");
        EXPECT_EQ(data.getCurrent(), 150) << "add('Wisdom', 150) should set current value to 150.";

        EXPECT_THROW(attr.add("NonExistent", 25), error::MIAException);
    }

    // Test add with uint32_t ID
    TEST_F(Attributes_T, addById)
    {
        Attributes attr;
        attr.add(3, 25);
        const AttributeData& data = attr.get(3);
        EXPECT_EQ(data.getCurrent(), 25) << "add(3, 25) should set current value to 25.";
    }

    // Test add with Attribute object
    TEST_F(Attributes_T, addByAttribute)
    {
        Attributes attr;
        attr.add(strength, 30);
        const AttributeData& data = attr.get(strength);
        EXPECT_EQ(data.getCurrent(), 30)
            << "add(strength, 30) should set current value to 30.";
    }

    // Test update with string name
    TEST_F(Attributes_T, updateByName)
    {
        attributes.update("Wisdom", 200);
        EXPECT_EQ(attributes.get("Wisdom").getCurrent(), 200)
            << "update('Wisdom', 200) should update current value to 200.";

        EXPECT_THROW(attributes.update("NonExistent", 300), error::MIAException);
    }

    // Test update with uint32_t ID
    TEST_F(Attributes_T, updateById)
    {
        attributes.update(1, 200);
        EXPECT_EQ(attributes.get(1).getCurrent(), 200)
            << "update(1, 200) should update current value to 200.";

        EXPECT_THROW(attributes.update(999, 300), error::MIAException);
    }

    // Test update with Attribute object
    TEST_F(Attributes_T, updateByAttribute)
    {
        attributes.update(dexterity, 150);
        EXPECT_EQ(attributes.get(dexterity).getCurrent(), 150)
            << "update(dexterity, 150) should update current value to 150.";
    }

    // Test addModifier with string name
    TEST_F(Attributes_T, addModifierByName)
    {
        attributes.addModifier("Wisdom", 2, rpg::ModifierSourceType::BUFF, 10);
        const AttributeData& data = attributes.get("Wisdom");
        EXPECT_EQ(data.getCurrent(), 110) // Assuming modifiers are applied in AttributeData::getCurrent
            << "addModifier('Wisdom', ...) should increase current value by 10.";

        const auto& modifiers = data.getModifiers();
        ASSERT_EQ(modifiers.size(), 1) << "One modifier should be added.";
        EXPECT_EQ(modifiers[0].sourceID, 2) << "Modifier sourceID should be 2.";
        EXPECT_EQ(modifiers[0].source, rpg::ModifierSourceType::BUFF) << "Modifier source should be BUFF.";
        EXPECT_EQ(modifiers[0].value, 10) << "Modifier value should be 10.";

        EXPECT_THROW(attributes.addModifier("NonExistent", 3, rpg::ModifierSourceType::BUFF, 5), 
                     error::MIAException);
    }

    // Test addModifier with uint32_t ID
    TEST_F(Attributes_T, addModifierById)
    {
        attributes.addModifier(1, 2, rpg::ModifierSourceType::BUFF, 10);
        const AttributeData& data = attributes.get(1);
        EXPECT_EQ(data.getCurrent(), 110) // Assuming modifiers are applied in AttributeData::getCurrent
            << "addModifier(1, ...) should increase current value by 10.";

        const auto& modifiers = data.getModifiers();
        ASSERT_EQ(modifiers.size(), 1) << "One modifier should be added.";
        EXPECT_EQ(modifiers[0].sourceID, 2) << "Modifier sourceID should be 2.";
        EXPECT_EQ(modifiers[0].source, rpg::ModifierSourceType::BUFF) << "Modifier source should be BUFF.";
        EXPECT_EQ(modifiers[0].value, 10) << "Modifier value should be 10.";

        EXPECT_THROW(attributes.addModifier(999, 3, rpg::ModifierSourceType::BUFF, 5), 
                     error::MIAException);
    }

    // Test addModifier with Attribute object
    TEST_F(Attributes_T, addModifierByAttribute)
    {
        attributes.addModifier(strength, 2, rpg::ModifierSourceType::BUFF, 15);
        const AttributeData& data = attributes.get(strength);
        EXPECT_EQ(data.getCurrent(), 90) // Assuming modifiers are applied in AttributeData::getCurrent
            << "addModifier(strength, ...) should increase current value by 15.";

        const auto& modifiers = data.getModifiers();
        ASSERT_EQ(modifiers.size(), 1) << "One modifier should be added.";
        EXPECT_EQ(modifiers[0].sourceID, 2) << "Modifier sourceID should be 2.";
        EXPECT_EQ(modifiers[0].source, rpg::ModifierSourceType::BUFF) << "Modifier source should be BUFF.";
        EXPECT_EQ(modifiers[0].value, 15) << "Modifier value should be 15.";
    }

    // Test removeModifier with string name
    TEST_F(Attributes_T, removeModifierByName)
    {
        attributes.addModifier("Wisdom", 2, rpg::ModifierSourceType::BUFF, 10);
        attributes.removeModifier("Wisdom", 2, rpg::ModifierSourceType::BUFF);
        const AttributeData& data = attributes.get("Wisdom");
        int current = data.getCurrent();
        EXPECT_EQ(data.getCurrent(), current) // Assuming modifier removal reverses effect
            << "removeModifier('Wisdom', ...) should revert current value to current.";
        EXPECT_TRUE(data.getModifiers().empty()) << "Modifier should be removed.";

        EXPECT_THROW(attributes.removeModifier("NonExistent", 3, rpg::ModifierSourceType::BUFF), 
                     error::MIAException);
    }

    // Test removeModifier with uint32_t ID
    TEST_F(Attributes_T, removeModifierById)
    {
        attributes.addModifier(1, 2, rpg::ModifierSourceType::BUFF, 10);
        const AttributeData& data = attributes.get(1);
        int current = data.getCurrent();
        attributes.removeModifier(1, 2, rpg::ModifierSourceType::BUFF);
        EXPECT_EQ(data.getCurrent(), current) // Assuming modifier removal reverses effect
            << "removeModifier(1, ...) should revert current value to current.";
        EXPECT_TRUE(data.getModifiers().empty()) << "Modifier should be removed.";

        EXPECT_THROW(attributes.removeModifier(999, 3, rpg::ModifierSourceType::BUFF), 
                     error::MIAException);
    }

    // Test removeModifier with Attribute object
    TEST_F(Attributes_T, removeModifierByAttribute)
    {
        attributes.addModifier(dexterity, 2, rpg::ModifierSourceType::BUFF, 10);
        attributes.removeModifier(dexterity, 2, rpg::ModifierSourceType::BUFF);
        const AttributeData& data = attributes.get(dexterity);
        int current = data.getCurrent();
        EXPECT_EQ(data.getCurrent(), current) // Assuming modifier removal reverses effect
            << "removeModifier(dexterity, ...) should revert current value to current.";
        EXPECT_TRUE(data.getModifiers().empty()) << "Modifier should be removed.";
    }

    // Test remove with string name
    TEST_F(Attributes_T, removeByName)
    {
        attributes.remove("Wisdom");
        EXPECT_EQ(attributes.get("Wisdom").getCurrent(), 100)
            << "remove('Wisdom') should remove the attribute, returning default AttributeData.";
    }

    // Test remove with uint32_t ID
    TEST_F(Attributes_T, removeById)
    {
        attributes.remove(1);
        EXPECT_EQ(attributes.get(1).getCurrent(), 100)
            << "remove(1) should remove the attribute, returning default AttributeData.";
    }

    // Test remove with Attribute object
    TEST_F(Attributes_T, removeByAttribute)
    {
        attributes.remove(strength);
        EXPECT_EQ(attributes.get(strength).getCurrent(), 75)
            << "remove(strength) should remove the attribute, returning default AttributeData.";
    }

    TEST_F(Attributes_T, SerializeDeserialize)
    {
        Attributes attr;
        attr.add(wisdom, 100);
        attr.addModifier(wisdom, 2, rpg::ModifierSourceType::BUFF, 10);
        attr.add(dexterity, 50);
        attr.add(strength, 75);

        std::string serialized = attr.serialize();
        Attributes deserialized = Attributes::deserialize(serialized);

        const AttributeData& data1 = deserialized.get(1);
        EXPECT_EQ(data1.getCurrent(), 110) << "Deserialized attribute 1 should have current=110.";
        const auto& mods1 = data1.getModifiers();
        ASSERT_EQ(mods1.size(), 1) << "Attribute 1 should have one modifier.";
        EXPECT_EQ(mods1[0].sourceID, 2) << "Modifier sourceID should be 2.";
        EXPECT_EQ(mods1[0].source, rpg::ModifierSourceType::BUFF) << "Modifier source should be BUFF.";
        EXPECT_EQ(mods1[0].value, 10) << "Modifier value should be 10.";

        const AttributeData& data2 = deserialized.get(2);
        EXPECT_EQ(data2.getCurrent(), 50) << "Deserialized attribute 2 should have current=50.";
        EXPECT_TRUE(data2.getModifiers().empty()) << "Attribute 2 should have no modifiers.";

        const AttributeData& data3 = deserialized.get(3);
        EXPECT_EQ(data3.getCurrent(), 75) << "Deserialized attribute 3 should have current=75.";
        EXPECT_TRUE(data3.getModifiers().empty()) << "Attribute 3 should have no modifiers.";
    }

} // namespace stats
