/**
 * @file VitalData_T.cpp
 * @author Antonius Torode
 * @date 10/08/2026
 * @brief Google Test file for testing the VitalData class functionality.
 */

#include <gtest/gtest.h>
#include <vector>

#include "VitalData.hpp"
#include "Modifier.hpp"
#include "MIAException.hpp"

namespace stats
{
    class VitalData_T : public ::testing::Test
    {
    protected:
        VitalData_T()
            : sampleModifier(1, rpg::ModifierSourceType::ATTRIBUTE, 5),
              anotherModifier(2, rpg::ModifierSourceType::ITEM, 10)
        {}

        rpg::Modifier sampleModifier;
        rpg::Modifier anotherModifier;
    };

    /**
     * @brief Verifies the explicit-values constructor stores the current value and the base bounds.
     */
    TEST_F(VitalData_T, ExplicitValuesConstructor)
    {
        VitalData vital(80, 0, 100);
        EXPECT_EQ(vital.getCurrent(), 80)
            << "The constructor should set the current value to 80.";
        EXPECT_EQ(vital.getBaseMin(), 0)
            << "The constructor should set the base minimum to 0.";
        EXPECT_EQ(vital.getBaseMax(), 100)
            << "The constructor should set the base maximum to 100.";
        EXPECT_EQ(vital.getCurrentMin(), 0)
            << "The effective minimum should equal the base minimum with no modifiers.";
        EXPECT_EQ(vital.getCurrentMax(), 100)
            << "The effective maximum should equal the base maximum with no modifiers.";
    }

    /**
     * @brief Verifies the VitalType constructor initializes the current value per behavior type.
     */
    TEST_F(VitalData_T, VitalTypeConstructor)
    {
        // An accumulative vital starts empty at its minimum.
        VitalData rage(VitalType::ACCUMULATIVE, 0, 100);
        EXPECT_EQ(rage.getCurrent(), 0)
            << "An accumulative vital should start at its minimum.";

        // A depletive vital starts full at its maximum.
        VitalData health(VitalType::DEPLETIVE, 0, 100);
        EXPECT_EQ(health.getCurrent(), 100)
            << "A depletive vital should start at its maximum.";

        // An unknown vital type defaults to half of the range.
        VitalData unknown(VitalType::UNKNOWN, 0, 100);
        EXPECT_EQ(unknown.getCurrent(), 50)
            << "An unknown vital type should start at half of the range.";
    }

    /**
     * @brief Verifies setCurrent clamps the value into the effective range.
     */
    TEST_F(VitalData_T, SetCurrentClamps)
    {
        VitalData vital(80, 0, 100);
        vital.setCurrent(90);
        EXPECT_EQ(vital.getCurrent(), 90)
            << "An in-range value should be stored as-is.";

        vital.setCurrent(120);
        EXPECT_EQ(vital.getCurrent(), 100)
            << "A value above the maximum should clamp to the maximum.";

        vital.setCurrent(-10);
        EXPECT_EQ(vital.getCurrent(), 0)
            << "A value below the minimum should clamp to the minimum.";
    }

    /**
     * @brief Verifies getCurrent clamps a current value stored outside the effective range.
     */
    TEST_F(VitalData_T, GetCurrentClamps)
    {
        // The constructor does not clamp the stored current; reading it does.
        VitalData vital(150, 0, 100);
        EXPECT_EQ(vital.getCurrent(), 100)
            << "A stored current above the maximum should read as the maximum.";

        // Lowering the base maximum clamps the read value.
        vital.setBaseMax(50);
        EXPECT_EQ(vital.getCurrent(), 50)
            << "The current value should clamp to the lowered effective maximum.";
    }

    /**
     * @brief Verifies modifier replacement: the same source, source type, and modify type replaces.
     */
    TEST_F(VitalData_T, AddModifierReplaces)
    {
        VitalData vital(80, 0, 100);
        vital.addModifier(sampleModifier, VitalDataTarget::CURRENT_MAX);
        EXPECT_EQ(vital.getCurrentMax(), 105)
            << "The additive modifier should raise the effective maximum.";

        rpg::Modifier replacement(1, rpg::ModifierSourceType::ATTRIBUTE, 20);
        vital.addModifier(replacement, VitalDataTarget::CURRENT_MAX);
        ASSERT_EQ(vital.getModifiers(VitalDataTarget::CURRENT_MAX).size(), 1)
            << "Replacing modifier should not grow the vector.";
        EXPECT_EQ(vital.getCurrentMax(), 120)
            << "The replaced modifier should carry the new value.";
    }

    /**
     * @brief Verifies removeModifier removes every modifier from the source, regardless of modify type.
     */
    TEST_F(VitalData_T, RemoveModifierRemovesWholeSource)
    {
        VitalData vital(80, 0, 100);
        vital.addModifier(rpg::Modifier(1, rpg::ModifierSourceType::ATTRIBUTE, 20),
                          VitalDataTarget::CURRENT_MAX);
        vital.addModifier(rpg::Modifier(1, rpg::ModifierSourceType::ATTRIBUTE, 0.1,
                                        rpg::ModifyType::MULTIPLY),
                          VitalDataTarget::CURRENT_MAX);
        vital.addModifier(anotherModifier, VitalDataTarget::CURRENT_MAX);
        EXPECT_EQ(vital.getCurrentMax(), 140)
            << "Both modifiers should apply: (100 * 1.1) + 20 + 10.";

        // A placeholder modifier whose source matches is enough to remove both.
        vital.removeModifier(rpg::Modifier(1, rpg::ModifierSourceType::ATTRIBUTE, 0),
                             VitalDataTarget::CURRENT_MAX);
        ASSERT_EQ(vital.getModifiers(VitalDataTarget::CURRENT_MAX).size(), 1)
            << "Removing a source should drop every modify type from that source.";
        EXPECT_EQ(vital.getModifiers(VitalDataTarget::CURRENT_MAX)[0].sourceID, 2)
            << "Only the other source's modifier should remain.";
        EXPECT_EQ(vital.getCurrentMax(), 110)
            << "The effective maximum should derive from the remaining modifier.";
    }

    /**
     * @brief Verifies ADD_MAX, MULTIPLY, and SET modifiers on the maximum follow the phase order.
     */
    TEST_F(VitalData_T, ModifierTypes)
    {
        VitalData vital(80, 0, 100);

        vital.addModifier(rpg::Modifier(1, rpg::ModifierSourceType::ATTRIBUTE, 20),
                          VitalDataTarget::CURRENT_MAX);
        EXPECT_EQ(vital.getCurrentMax(), 120)
            << "An additive modifier should add to the base maximum.";

        vital.addModifier(rpg::Modifier(2, rpg::ModifierSourceType::BUFF, 0.1,
                                        rpg::ModifyType::MULTIPLY),
                          VitalDataTarget::CURRENT_MAX);
        EXPECT_EQ(vital.getCurrentMax(), 130)
            << "The multiplier should scale the base before additions: (100 * 1.1) + 20.";

        vital.addModifier(rpg::Modifier(3, rpg::ModifierSourceType::ITEM, 90,
                                        rpg::ModifyType::SET),
                          VitalDataTarget::CURRENT_MAX);
        EXPECT_EQ(vital.getCurrentMax(), 90)
            << "A set modifier should replace the computed maximum.";

        // Removing the set modifier restores the computed value from the other modifiers.
        vital.removeModifier(rpg::Modifier(3, rpg::ModifierSourceType::ITEM, 0),
                             VitalDataTarget::CURRENT_MAX);
        EXPECT_EQ(vital.getCurrentMax(), 130)
            << "Removing the set modifier should restore the computed value.";

        // Modifiers can also target the minimum.
        vital.addModifier(rpg::Modifier(4, rpg::ModifierSourceType::BUFF, 5),
                          VitalDataTarget::CURRENT_MIN);
        EXPECT_EQ(vital.getCurrentMin(), 5)
            << "An additive modifier should raise the effective minimum.";
    }

    /**
     * @brief Verifies the effective minimum never rises above the effective maximum.
     */
    TEST_F(VitalData_T, EffectiveMinYieldsToMax)
    {
        VitalData vital(80, 0, 100);

        // A base minimum above the base maximum yields to the maximum.
        vital.setBaseMin(150);
        EXPECT_EQ(vital.getCurrentMin(), 100)
            << "The effective minimum should yield to the effective maximum.";
        EXPECT_EQ(vital.getCurrentMax(), 100)
            << "The effective maximum should be unaffected.";
        EXPECT_EQ(vital.getCurrent(), 100)
            << "The current value should clamp to the effective bounds.";

        // A raising modifier on the minimum is also capped by the maximum.
        VitalData capped(50, 0, 100);
        capped.addModifier(rpg::Modifier(1, rpg::ModifierSourceType::ATTRIBUTE, 150),
                           VitalDataTarget::CURRENT_MIN);
        EXPECT_EQ(capped.getCurrentMin(), 100)
            << "A modifier pushing the minimum past the maximum should be capped.";
    }

    /**
     * @brief Verifies getModifiers throws for targets which have no modifier vector.
     */
    TEST_F(VitalData_T, GetModifiersInvalidTarget)
    {
        VitalData vital(80, 0, 100);
        EXPECT_THROW(vital.getModifiers(VitalDataTarget::CURRENT), error::MIAException)
            << "The CURRENT target has no modifier vector.";
        EXPECT_THROW(vital.getModifiers(VitalDataTarget::UNKNOWN), error::MIAException)
            << "The UNKNOWN target has no modifier vector.";
    }
} // namespace stats
