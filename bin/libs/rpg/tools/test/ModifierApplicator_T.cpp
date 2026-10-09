/**
 * @file ModifierApplicator_T.cpp
 * @author Antonius Torode
 * @date 10/09/2026
 * @brief Unit tests for applyModifiers, using the attribute and vital registries and storages.
 */

#include <string>

#include <gtest/gtest.h>

// Include the associated files for testing.
#include "ModifierApplicator.hpp"
#include "AttributeRegistry.hpp"
#include "VitalRegistry.hpp"
#include "Attributes.hpp"
#include "Vitals.hpp"
#include "Attribute.hpp"

// Used for exception assertions.
#include "MIAException.hpp"
// Used for locating the test data directory.
#include "Paths.hpp"

namespace
{
    /**
     * Loads a JSON file from the test data directory beside this source file.
     *
     * @param filename The file name within the test data directory.
     * @return The file contents.
     */
    std::string loadTestData(const std::string& filename)
    {
        std::string dataDir = paths::getCppFileDirAtCompileTime(__FILE__) + "/test_data";
        std::ifstream file(dataDir + "/" + filename);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }


    /**
     * Applies attribute modifiers to the vitals using the loaded registries.
     *
     * @param attributes The attribute storage acting as the modifier source.
     * @param vitals The vital storage receiving the modifiers.
     */
    void applyAttributeModifiers(stats::Attributes& attributes, stats::Vitals& vitals)
    {
        rpg::helper_methods::applyModifiers
            <stats::AttributeRegistry, stats::VitalRegistry,
             stats::Attributes, stats::Vitals>
            (stats::AttributeRegistry::getInstance(), stats::VitalRegistry::getInstance(),
             attributes, vitals);
    }
}


/**
 * Test fixture for the applicator tests. Loads the shared test data registries and
 * builds an attribute and vital storage for each test to populate.
 */
class ModifierApplicator_T : public ::testing::Test
{
protected:
    void SetUp() override
    {
        stats::AttributeRegistry::getInstance().loadFromString(
            loadTestData("ModifierApplicator_attributes.json"));
        stats::VitalRegistry::getInstance().loadFromString(
            loadTestData("ModifierApplicator_vitals.json"));
    }

    stats::Attributes attributes; ///< The attribute storage under test.
    stats::Vitals vitals;         ///< The vital storage under test.
};


/**
 * @brief Verifies an ADD_MAX modifies declaration adds valuePer * source value to the
 * target vital's maximum.
 */
TEST_F(ModifierApplicator_T, AppliesAddMaxModifier)
{
    attributes.add("Constitution", 15);
    vitals.add("Health", 100, 0, 100);

    applyAttributeModifiers(attributes, vitals);

    const stats::VitalData& health = vitals.get("Health");
    EXPECT_EQ(health.getBaseMax(), 100) << "The base maximum should stay unchanged.";
    EXPECT_EQ(health.getCurrentMax(), 175) << "5.0 per point at 15 points adds 75 to 100.";
    EXPECT_EQ(health.getCurrent(), 100) << "The current value should be untouched.";
}


/**
 * @brief Verifies the attached modifier records its source and computed value.
 */
TEST_F(ModifierApplicator_T, AttachedModifierRecordsSource)
{
    attributes.add("Constitution", 15);
    vitals.add("Health", 100, 0, 100);

    applyAttributeModifiers(attributes, vitals);

    const auto& modifiers = vitals.get("Health").getModifiers(stats::VitalDataTarget::CURRENT_MAX);
    ASSERT_EQ(modifiers.size(), 1);
    EXPECT_EQ(modifiers[0].sourceID, 2u);
    EXPECT_EQ(modifiers[0].source, rpg::ModifierSourceType::ATTRIBUTE);
    EXPECT_EQ(modifiers[0].modifyType, rpg::ModifyType::ADD_MAX);
    EXPECT_EQ(modifiers[0].getValueAsInt(), 75);
}


/**
 * @brief Verifies a MULTIPLY modifies declaration scales the target vital's maximum.
 */
TEST_F(ModifierApplicator_T, AppliesMultiplyModifier)
{
    attributes.add("Focus", 2);
    vitals.add("Mana", 200, 0, 200);

    applyAttributeModifiers(attributes, vitals);

    EXPECT_EQ(vitals.get("Mana").getCurrentMax(), 240)
        << "0.1 per point at 2 points scales 200 by 1.2.";
}


/**
 * @brief Verifies a SET modifies declaration replaces the target vital's maximum.
 */
TEST_F(ModifierApplicator_T, AppliesSetModifier)
{
    attributes.add("Fate", 1);
    vitals.add("Mana", 200, 0, 200);

    applyAttributeModifiers(attributes, vitals);

    EXPECT_EQ(vitals.get("Mana").getCurrentMax(), 50) << "The SET modifier should override.";
    EXPECT_EQ(vitals.get("Mana").getCurrent(), 50) << "The current value clamps to the new max.";
}


/**
 * @brief Verifies the phase ordering end to end: multipliers scale the base before
 * additions are summed on. 200 scaled by 1.2 is 240, plus 30 is 270; adding first
 * would give (200 + 30) * 1.2 = 276.
 */
TEST_F(ModifierApplicator_T, MultiplyAppliesBeforeAdd)
{
    attributes.add("Focus", 2);
    attributes.add("Wisdom", 3);
    vitals.add("Mana", 200, 0, 200);

    applyAttributeModifiers(attributes, vitals);

    EXPECT_EQ(vitals.get("Mana").getCurrentMax(), 270);
}


/**
 * @brief Verifies re-applying after the source's value changes replaces the old modifier
 * instead of stacking alongside it.
 */
TEST_F(ModifierApplicator_T, ReapplyReplacesInsteadOfStacks)
{
    attributes.add("Constitution", 15);
    vitals.add("Health", 100, 0, 100);

    applyAttributeModifiers(attributes, vitals);
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 175);

    attributes.update("Constitution", 20);
    applyAttributeModifiers(attributes, vitals);

    const auto& modifiers = vitals.get("Health").getModifiers(stats::VitalDataTarget::CURRENT_MAX);
    EXPECT_EQ(modifiers.size(), 1) << "The replacement should not stack with the old modifier.";
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 200) << "5.0 per point at 20 points adds 100.";
}


/**
 * @brief Verifies a source without modifies declarations leaves the targets unchanged.
 */
TEST_F(ModifierApplicator_T, SkipsSourcesWithoutModifies)
{
    attributes.add("Strength", 10);
    vitals.add("Health", 100, 0, 100);

    applyAttributeModifiers(attributes, vitals);

    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 100);
}


/**
 * @brief Verifies KEEP_STRONGEST modifiers from different sources compete, leaving only
 * the strongest attached. Guardian contributes 10.0 per point at 2 points (20) and
 * Blessing 5.0 per point at 3 points (15), so only Guardian's 20 should survive,
 * whatever order the attributes are visited in.
 */
TEST_F(ModifierApplicator_T, AppliesKeepStrongestModifier)
{
    attributes.add("Guardian", 2);
    attributes.add("Blessing", 3);
    vitals.add("Health", 100, 0, 100);

    applyAttributeModifiers(attributes, vitals);

    const auto& modifiers = vitals.get("Health").getModifiers(stats::VitalDataTarget::CURRENT_MAX);
    ASSERT_EQ(modifiers.size(), 1) << "Only the strongest KEEP_STRONGEST modifier should remain.";
    EXPECT_EQ(modifiers[0].sourceID, 7u);
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 120);
}


/**
 * @brief Verifies a modifies declaration whose target is not a registered vital throws.
 */
TEST_F(ModifierApplicator_T, MissingTargetThrows)
{
    attributes.add("Hex", 3);
    vitals.add("Health", 100, 0, 100);

    EXPECT_THROW(applyAttributeModifiers(attributes, vitals), error::MIAException);
}


/**
 * @brief Verifies an attribute in storage which is missing from the registry throws.
 */
TEST_F(ModifierApplicator_T, MissingSourceThrows)
{
    stats::Attribute ghost(999, "Ghost", "Not in the registry.");
    attributes.add(ghost, 5);
    vitals.add("Health", 100, 0, 100);

    EXPECT_THROW(applyAttributeModifiers(attributes, vitals), error::MIAException);
}
