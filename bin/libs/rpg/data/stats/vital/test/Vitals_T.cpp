/**
 * @file Vitals_T.cpp
 * @author Antonius Torode
 * @date 07/09/2025
 * Description: Unit tests for the rpg::Vitals class.
 */

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "Vitals.hpp"
#include "VitalRegistry.hpp"
#include "MIAException.hpp"

using namespace stats;

/**
 * @brief Test fixture for Vitals tests.
 */
class Vitals_T : public ::testing::Test 
{
protected:
    void SetUp() override
    {
        // Create a JSON object with the "vitals" key containing the array of vitals.
        nlohmann::json jsonObject;
        jsonObject["VITAL"] = { health.toJson(), mana.toJson(), rage.toJson() };
        std::string jsonData = jsonObject.dump();
        
        // Load currencies into registry.
        VitalRegistry::getInstance().loadFromString(jsonData);
    }
    
    Vitals vitals;

    Vital health {1, "Health", "The health of a player.", VitalType::DEPLETIVE};
    Vital mana  {2, "Mana", "The mana of a player.", VitalType::DEPLETIVE};
    Vital rage  {3, "Rage", "Therage built up by a player.", VitalType::ACCUMULATIVE};
};


/*
 * @brief Tests the get method.
 */
TEST_F(Vitals_T, get)
{
    vitals.add("Health", 80, 0, 100);
    VitalData data = vitals.get("Health");
    EXPECT_EQ(data.getCurrent(), 80);
    EXPECT_EQ(data.getCurrentMin(), 0);
    EXPECT_EQ(data.getCurrentMax(), 100);
    EXPECT_TRUE(data.getModifiers(VitalDataTarget::CURRENT_MAX).empty());
    EXPECT_TRUE(data.getModifiers(VitalDataTarget::CURRENT_MIN).empty());

    data = vitals.get(1);
    EXPECT_EQ(data.getCurrent(), 80);
    EXPECT_EQ(data.getCurrentMax(), 100);

    data = vitals.get(health);
    EXPECT_EQ(data.getCurrent(), 80);
    EXPECT_EQ(data.getCurrentMax(), 100);

    EXPECT_THROW(vitals.get("invalid"), error::MIAException);
}


/*
 * @brief Test adding a new vital.
 */
TEST_F(Vitals_T, add)
{
    EXPECT_NO_THROW(vitals.add("Health", 80, 0, 100));
    const VitalData& data = vitals.get("Health");
    EXPECT_EQ(data.getCurrent(), 80);
    EXPECT_EQ(data.getCurrentMin(), 0);
    EXPECT_EQ(data.getCurrentMax(), 100);
    
    EXPECT_THROW(vitals.add("Health", 50, 0, 100), error::MIAException); // Already exists
    EXPECT_THROW(vitals.add("invalid", 50, 0, 100), error::MIAException); // Invalid vital
    EXPECT_THROW(vitals.add(1, 150, 0, 100), error::MIAException); // Current > max
    EXPECT_THROW(vitals.add(1, 50, 120, 100), error::MIAException); // Min > max
}


/*
 * @brief Test updating the current vital values.
 */
TEST_F(Vitals_T, updateCurrent)
{
    vitals.add("Health", 80, 0, 100);

    // Valid update within range.
    EXPECT_NO_THROW(vitals.update("Health", 90));
    EXPECT_EQ(vitals.get("Health").getCurrent(), 90);

    // Update above max should clamp to max.
    EXPECT_NO_THROW(vitals.update(1, 120));
    EXPECT_EQ(vitals.get(1).getCurrent(), 100);

    // Update below min should clamp to min.
    EXPECT_NO_THROW(vitals.update(health, -10));
    EXPECT_EQ(vitals.get(health).getCurrent(), 0);

    // Invalid vital name should throw.
    EXPECT_THROW(vitals.update("invalid", 50), error::MIAException);
}


/*
 * @brief Test that the bounds of the base values behave accordingly.
 */
TEST_F(Vitals_T, baseBounds)
{
    vitals.add("Health", 80, 0, 100);

    // Raising the base min clamps the current value and leaves the base max alone.
    vitals.get("Health").setBaseMin(90);
    EXPECT_EQ(vitals.get("Health").getBaseMin(), 90);
    EXPECT_EQ(vitals.get("Health").getCurrentMin(), 90);
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 100);
    EXPECT_EQ(vitals.get("Health").getCurrent(), 90);

    // Lowering the base max clamps the current value and pulls the min down with it.
    vitals.get(1).setBaseMax(50);
    EXPECT_EQ(vitals.get(1).getBaseMax(), 50);
    EXPECT_EQ(vitals.get(1).getCurrentMax(), 50);
    EXPECT_EQ(vitals.get(1).getCurrentMin(), 50);
    EXPECT_EQ(vitals.get(1).getCurrent(), 50);

    // A base min above the base max yields to the max.
    vitals.get(health).setBaseMin(150);
    EXPECT_EQ(vitals.get(health).getBaseMin(), 150);
    EXPECT_EQ(vitals.get(health).getCurrentMin(), 50);
    EXPECT_EQ(vitals.get(health).getCurrentMax(), 50);
}


/*
 * @brief Test the various modifier types and that they work correctly.
 */
TEST_F(Vitals_T, modifierTypes)
{
    vitals.add("Health", 80, 0, 100);

    // An additive modifier adds its amount to the base max.
    vitals.addModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20,
                       VitalDataTarget::CURRENT_MAX);
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 120);

    // A multiplier bonus scales the base before additions are applied.
    rpg::Modifier multiply(2, rpg::ModifierSourceType::BUFF, 0.1);
    vitals.addModifier("Health", multiply, VitalDataTarget::CURRENT_MAX);
    // (100 * 1.1) + 20 = 130
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 130);

    // A set modifier replaces the computed value.
    rpg::Modifier set(3, rpg::ModifierSourceType::ITEM, 90, rpg::ModifyType::SET);
    vitals.addModifier(1, set, VitalDataTarget::CURRENT_MAX);
    EXPECT_EQ(vitals.get(1).getCurrentMax(), 90);

    // Removing the set modifier restores the computed value from the other modifiers.
    vitals.removeModifier(1, 3, rpg::ModifierSourceType::ITEM, VitalDataTarget::CURRENT_MAX);
    EXPECT_EQ(vitals.get(1).getCurrentMax(), 130);

    // Removing the multiplier drops the max back to the additive-only result.
    vitals.removeModifier(1, 2, rpg::ModifierSourceType::BUFF, VitalDataTarget::CURRENT_MAX);
    EXPECT_EQ(vitals.get(1).getCurrentMax(), 120);
}


/*
 * @brief Test that adding and removing modifiers correctly updates values.
 */
TEST_F(Vitals_T, AddRemoveModifier)
{
    vitals.add("Health", 80, 0, 100);

    // Add +20 modifier to max -> max becomes 120.
    EXPECT_NO_THROW(vitals.addModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20,
                                       VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 120);

    // Add -10 modifier to max -> max becomes 110.
    EXPECT_NO_THROW(vitals.addModifier(1, 2, rpg::ModifierSourceType::ITEM, -10,
                                            VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.get(1).getCurrentMax(), 110);

    // Add +5 modifier to min -> min becomes 5.
    EXPECT_NO_THROW(vitals.addModifier(health, 3, rpg::ModifierSourceType::BUFF, 5,
                                            VitalDataTarget::CURRENT_MIN));
    EXPECT_EQ(vitals.get(health).getModifiers(VitalDataTarget::CURRENT_MIN).size(), 1);
    EXPECT_EQ(vitals.get(health).getCurrentMin(), 5);

    // Remove modifier with sourceId 1 from max -> max becomes 90.
    EXPECT_NO_THROW(vitals.removeModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE,
                                          VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 90);

    // Remove non-existent modifier -> no exception; data unchanged.
    EXPECT_NO_THROW(vitals.removeModifier("Health", 999, rpg::ModifierSourceType::ATTRIBUTE,
                                          VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 90); // Confirm unchanged.

    // Invalid vital name -> throws
    EXPECT_THROW(vitals.addModifier("invalid", 1, rpg::ModifierSourceType::ATTRIBUTE, 20,
                                    VitalDataTarget::CURRENT_MAX), error::MIAException);

    // Overflow risk (optional test) - depending on logic, this may or may not throw.
    EXPECT_NO_THROW(vitals.addModifier(1, 1, rpg::ModifierSourceType::ATTRIBUTE,
                                       std::numeric_limits<int>::max(),
                                       VitalDataTarget::CURRENT_MAX));
    // The resulting max may wrap, clamp, or just increase depending on implementation.
}


/*
 * @brief Test removing a vital.
 */
TEST_F(Vitals_T, remove)
{
    vitals.add("Health", 80, 0, 100);

    // Remove Health -> data removed from internal map.
    EXPECT_NO_THROW(vitals.remove("Health"));

    // Querying after removal should return default
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 100); // From fallback/default.

    // Attempting second removal does not throw, but does nothing.
    EXPECT_NO_THROW(vitals.remove("Health"));

    // Remove invalid vital -> throws.
    EXPECT_THROW(vitals.remove("invalid"), error::MIAException);
}


/*
 * @brief Test that a vital is correctly found when it exists.
 */
TEST_F(Vitals_T, has)
{
    vitals.add("Health", 80, 0, 100);

    // Test whether or not having enough works.
    EXPECT_TRUE(vitals.has("Health", 50));

    // Test whether or not it correctly finds not enough..
    EXPECT_FALSE(vitals.has("Health", 90));

    // Invalid vital -> throws
    EXPECT_THROW(vitals.has("invalid", 5), error::MIAException);
}


/*
 * @brief Test getting the min and max values.
 */
TEST_F(Vitals_T, GetVitalMaxMin)
{
    vitals.add("Health", 80, 0, 100);
    vitals.addModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20, 
                       VitalDataTarget::CURRENT_MAX);
    vitals.addModifier("Health", 2, rpg::ModifierSourceType::ITEM, -10,  
                       VitalDataTarget::CURRENT_MAX);
    vitals.addModifier("Health", 3, rpg::ModifierSourceType::BUFF, 5,  
                       VitalDataTarget::CURRENT_MIN);

    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 110);
    EXPECT_EQ(vitals.get("Health").getCurrentMin(), 5);

    EXPECT_THROW(vitals.get(999).getCurrentMax(), error::MIAException);
    EXPECT_THROW(vitals.get("invalid").getCurrentMin(), error::MIAException);
}


/*
 * @brief Test that serialize and deserialize work as expected.
 */
TEST_F(Vitals_T, SerializeDeserialize)
{
    vitals.add("Health", 80, 0, 100);
    vitals.add("Mana", 50, 0, 200);
    vitals.addModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20,  
                       VitalDataTarget::CURRENT_MAX);
    vitals.addModifier("Health", 2, rpg::ModifierSourceType::BUFF, 5,  
                       VitalDataTarget::CURRENT_MIN);

    std::string serialized = vitals.serialize();
    Vitals newVitals = Vitals::deserialize(serialized);

    EXPECT_EQ(newVitals.get("Health").getCurrent(), 80);
    EXPECT_EQ(newVitals.get("Health").getCurrentMax(), 120);
    EXPECT_EQ(newVitals.get("Health").getCurrentMin(), 5);
    EXPECT_EQ(newVitals.get("Mana").getCurrentMax(), 200);

    EXPECT_THROW(Vitals::deserialize("invalid data"), error::MIAException);
}
