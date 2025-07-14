/**
  @file AttributeData_T.cpp
  @author Antonius Torode
  @date 07/13/2025
  Description: Google Test file for testing the AttributeData class functionality.
*/
#include <gtest/gtest.h>
#include <vector>
#include "AttributeData.hpp"
#include "Modifier.hpp"

namespace stats
{
    class AttributeData_T : public ::testing::Test
    {
    protected:
        AttributeData_T()
            : sampleModifier(1, rpg::ModifierSourceType::ATTRIBUTE, 5),
              anotherModifier(2, rpg::ModifierSourceType::ITEM, 10)
        {}

        void SetUp() override
        {
            // Optionally modify sampleModifier here if needed.
        }

        rpg::Modifier<int> sampleModifier;
        rpg::Modifier<int> anotherModifier;
    };

    // Tests the single-value constructor to ensure it initializes current and empty modifiers.
    TEST_F(AttributeData_T, SingleValueConstructor)
    {
        AttributeData attr(10);
        EXPECT_EQ(attr.getCurrent(), 10) << "Single-value constructor should set current to 10";
        EXPECT_TRUE(attr.getModifiers().empty()) << "Single-value constructor should set empty modifiers vector";
    }

    // Tests the constructor with modifiers to ensure it initializes current and modifiers correctly.
    TEST_F(AttributeData_T, ConstructorWithModifiers)
    {
        std::vector<rpg::Modifier<int>> mods = {sampleModifier};
        AttributeData attr(15, mods);
        EXPECT_EQ(attr.getCurrent(), 15) << "Constructor with modifiers should set current to 15";
        ASSERT_EQ(attr.getModifiers().size(), 1) << "Constructor with modifiers should set one modifier";
        EXPECT_EQ(attr.getModifiers()[0].value, sampleModifier.value) << "Modifier value should match";
        EXPECT_EQ(attr.getModifiers()[0].sourceId, sampleModifier.sourceId) << "Modifier sourceId should match";
        EXPECT_EQ(attr.getModifiers()[0].source, sampleModifier.source) << "Modifier source should match";
    }

    // Tests the getCurrent method to ensure it returns the correct current value.
    TEST_F(AttributeData_T, GetCurrent)
    {
        AttributeData attr(20);
        EXPECT_EQ(attr.getCurrent(), 20) << "getCurrent should return initial current value";
        attr.addModifier(sampleModifier);
        EXPECT_EQ(attr.getCurrent(), 25) << "getCurrent should return updated current value after adding modifier";
    }

    // Tests the addModifier method to ensure it adds a modifier and updates current.
    TEST_F(AttributeData_T, AddModifier)
    {
        AttributeData attr(10);
        attr.addModifier(sampleModifier);
        ASSERT_EQ(attr.getModifiers().size(), 1) << "Modifiers vector should contain one element after adding";
        EXPECT_EQ(attr.getModifiers()[0].value, sampleModifier.value) << "Added modifier value should match";
        EXPECT_EQ(attr.getCurrent(), 15) << "Current value should increase by modifier value";

        // Add another modifier to test vector growth.
        attr.addModifier(anotherModifier);
        ASSERT_EQ(attr.getModifiers().size(), 2) << "Modifiers vector should contain two elements";
        EXPECT_EQ(attr.getModifiers()[1].value, anotherModifier.value) << "Second modifier value should match";
        EXPECT_EQ(attr.getCurrent(), 25) << "Current value should increase by both modifiers";
    }

    // Tests the removeModifier method to ensure it removes a modifier and updates current.
    TEST_F(AttributeData_T, RemoveModifier)
    {
        std::vector<rpg::Modifier<int>> mods = {sampleModifier, anotherModifier};
        AttributeData attr(10, mods);
        attr.addModifier(sampleModifier); // Add duplicate to test specific removal
        EXPECT_EQ(attr.getCurrent(), 10) << "Current value should reflect the set current";

        attr.removeModifier(sampleModifier);
        ASSERT_EQ(attr.getModifiers().size(), 1) << "Modifiers vector should contain one element after removal";
        EXPECT_EQ(attr.getModifiers()[0].value, anotherModifier.value) << "Remaining modifier should be anotherModifier";
        EXPECT_EQ(attr.getCurrent(), 5) << "Current value should decrease by removed modifier value (10)";

        // Test removing a non-existent modifier.
        rpg::Modifier<int> nonExistentMod{15, rpg::ModifierSourceType::ITEM, 4};
        attr.removeModifier(nonExistentMod);
        EXPECT_EQ(attr.getModifiers().size(), 1) << "Modifiers vector should remain unchanged for non-existent modifier";
        EXPECT_EQ(attr.getCurrent(), 5) << "Current value should remain unchanged";
    }
} // namespace stats
