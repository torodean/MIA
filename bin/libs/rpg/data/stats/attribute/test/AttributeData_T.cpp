/**
 * @file AttributeData_T.cpp
 * @author Antonius Torode
 * @date 07/13/2025
 * @brief Google Test file for testing the AttributeData class functionality.
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

        rpg::Modifier sampleModifier;
        rpg::Modifier anotherModifier;
    };

    /**
     * @brief Verifies the single-value constructor initializes the base value and empty modifiers.
     */
    TEST_F(AttributeData_T, SingleValueConstructor)
    {
        AttributeData attr(10);
        EXPECT_EQ(attr.getCurrent(), 10)
            << "Single-value constructor should set current to 10";
        EXPECT_TRUE(attr.getModifiers().empty())
            << "Single-value constructor should set empty modifiers vector";
    }

    /**
     * @brief Verifies the constructor with modifiers derives the current value from the base and modifiers.
     */
    TEST_F(AttributeData_T, ConstructorWithModifiers)
    {
        std::vector<rpg::Modifier> mods = {sampleModifier};
        AttributeData attr(15, mods);
        EXPECT_EQ(attr.getCurrent(), 20)
            << "Constructor with modifiers should set current to base + modifiers";
        EXPECT_EQ(attr.getBaseValue(), 15)
            << "Constructor with modifiers should set base to 15";
        ASSERT_EQ(attr.getModifiers().size(), 1)
            << "Constructor with modifiers should set one modifier";
        EXPECT_EQ(attr.getModifiers()[0].getValueAsInt(), sampleModifier.getValueAsInt())
            << "Modifier value should match";
        EXPECT_EQ(attr.getModifiers()[0].sourceID, sampleModifier.sourceID)
            << "Modifier sourceID should match";
        EXPECT_EQ(attr.getModifiers()[0].source, sampleModifier.source)
            << "Modifier source should match";
    }

    /**
     * @brief Verifies getCurrent returns the correct current value.
     */
    TEST_F(AttributeData_T, GetCurrent)
    {
        AttributeData attr(20);
        EXPECT_EQ(attr.getCurrent(), 20)
            << "getCurrent should return initial current value";
        attr.addModifier(sampleModifier);
        EXPECT_EQ(attr.getCurrent(), 25)
            << "getCurrent should return updated current value after adding modifier";
    }

    /**
     * @brief Verifies addModifier adds a modifier and updates the current value.
     */
    TEST_F(AttributeData_T, AddModifier)
    {
        AttributeData attr(10);
        attr.addModifier(sampleModifier);
        ASSERT_EQ(attr.getModifiers().size(), 1)
            << "Modifiers vector should contain one element after adding";
        EXPECT_EQ(attr.getModifiers()[0].getValueAsInt(), sampleModifier.getValueAsInt())
            << "Added modifier value should match";
        EXPECT_EQ(attr.getCurrent(), 15)
            << "Current value should increase by modifier value";

        // Add another modifier to test vector growth.
        attr.addModifier(anotherModifier);
        ASSERT_EQ(attr.getModifiers().size(), 2)
            << "Modifiers vector should contain two elements";
        EXPECT_EQ(attr.getModifiers()[1].getValueAsInt(), anotherModifier.getValueAsInt())
            << "Second modifier value should match";
        EXPECT_EQ(attr.getCurrent(), 25)
            << "Current value should increase by both modifiers";
    }

    /**
     * @brief Verifies re-adding a modifier from the same source and type replaces it instead of stacking.
     */
    TEST_F(AttributeData_T, AddModifierReplaces)
    {
        AttributeData attr(10);
        attr.addModifier(sampleModifier);
        EXPECT_EQ(attr.getCurrent(), 15);

        rpg::Modifier replacement(1, rpg::ModifierSourceType::ATTRIBUTE, 7);
        attr.addModifier(replacement);
        ASSERT_EQ(attr.getModifiers().size(), 1)
            << "Replacing modifier should not grow the vector";
        EXPECT_EQ(attr.getModifiers()[0].getValueAsInt(), 7)
            << "Replaced modifier should carry the new value";
        EXPECT_EQ(attr.getCurrent(), 17)
            << "Current value should reflect the replaced modifier value";
    }

    /**
     * @brief Verifies removeModifier removes a modifier and updates the current value.
     */
    TEST_F(AttributeData_T, RemoveModifier)
    {
        std::vector<rpg::Modifier> mods = {sampleModifier, anotherModifier};
        AttributeData attr(10, mods);
        EXPECT_EQ(attr.getCurrent(), 25)
            << "Current value should reflect the base with both modifiers";

        attr.addModifier(sampleModifier); // Duplicate add is a no-op.
        ASSERT_EQ(attr.getModifiers().size(), 2)
            << "Duplicate modifier should not grow the vector";

        attr.removeModifier(sampleModifier);
        ASSERT_EQ(attr.getModifiers().size(), 1)
            << "Modifiers vector should contain one element after removal";
        EXPECT_EQ(attr.getModifiers()[0].getValueAsInt(), anotherModifier.getValueAsInt())
            << "Remaining modifier should be anotherModifier";
        EXPECT_EQ(attr.getCurrent(), 20)
            << "Current value should derive from the base and remaining modifier";

        // Test removing a non-existent modifier.
        rpg::Modifier nonExistentMod{15, rpg::ModifierSourceType::ITEM, 4};
        attr.removeModifier(nonExistentMod);
        EXPECT_EQ(attr.getModifiers().size(), 1)
            << "Modifiers vector should remain unchanged for non-existent modifier";
        EXPECT_EQ(attr.getCurrent(), 20)
            << "Current value should remain unchanged";
    }

    /**
     * @brief Verifies MULTIPLY modifiers scale the base before ADD modifiers are applied.
     */
    TEST_F(AttributeData_T, MultiplyAndSetModifiers)
    {
        AttributeData attr(100);
        rpg::Modifier multiply(3, rpg::ModifierSourceType::BUFF, 0.1);
        rpg::Modifier add(4, rpg::ModifierSourceType::BUFF, 50);
        rpg::Modifier set(5, rpg::ModifierSourceType::ITEM, 75, rpg::ModifyType::SET);

        attr.addModifier(multiply);
        EXPECT_EQ(attr.getCurrent(), 110)
            << "A multiplier bonus should scale the base";

        attr.addModifier(add);
        EXPECT_EQ(attr.getCurrent(), 160)
            << "Additions should apply after multipliers: (100 * 1.1) + 50";

        attr.addModifier(set);
        EXPECT_EQ(attr.getCurrent(), 75)
            << "A set modifier should replace the computed value";

        attr.removeModifier(set);
        EXPECT_EQ(attr.getCurrent(), 160)
            << "Removing the set modifier should restore the computed value";

        attr.removeModifier(multiply);
        EXPECT_EQ(attr.getCurrent(), 150)
            << "Removing the multiplier should leave the additive result";
    }

    /**
     * @brief Verifies setBaseValue changes the base value, which the current value derives from.
     */
    TEST_F(AttributeData_T, SetBaseValueSetsBase)
    {
        AttributeData attr(10);
        attr.addModifier(sampleModifier);
        attr.setBaseValue(20);
        EXPECT_EQ(attr.getBaseValue(), 20)
            << "setBaseValue should set the base value";
        EXPECT_EQ(attr.getCurrent(), 25)
            << "Current value should derive from the new base and modifiers";
    }
} // namespace stats
