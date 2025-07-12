/**
 * File: Vitals_T.cpp
 * Author: Antonius Torode
 * Created on: 07/09/2025
 * Description: Unit tests for the rpg::Vitals class.
 */

#include <gtest/gtest.h>
#include "Vitals.hpp"
#include "VitalRegistry.hpp"
#include <nlohmann/json.hpp>

using namespace stats;

/**
 * Test fixture for Vitals tests.
 */
class Vitals_T : public ::testing::Test 
{
protected:
    void SetUp() override
    {
        // Create a JSON object with the "vitals" key containing the array of vitals
        nlohmann::json jsonObject;
        jsonObject["vital"] = { health.toJson(), mana.toJson(), rage.toJson() };
        std::string jsonData = jsonObject.dump();
        
        // Load currencies into registry
        VitalRegistry::getInstance().loadFromString(jsonData);
    }
    
    Vitals vitals;

    Vital health {1, "Health", "The health of a player.", VitalType::DEPLETIVE};
    Vital mana  {2, "Mana", "The mana of a player.", VitalType::DEPLETIVE};
    Vital rage  {3, "Rage", "Therage built up by a player.", VitalType::ACCUMULATIVE};
};


TEST_F(Vitals_T, GetVitalData)
{
    vitals.addVitalData("Health", 80, 0, 100);
    VitalData data = vitals.getVitalData("Health");
    EXPECT_EQ(data.current, 80);
    EXPECT_EQ(data.currentMin, 0);
    EXPECT_EQ(data.currentMax, 100);
    EXPECT_TRUE(data.maxModifiers.empty());
    EXPECT_TRUE(data.minModifiers.empty());

    data = vitals.getVitalData(1);
    EXPECT_EQ(data.current, 80);
    EXPECT_EQ(data.currentMax, 100);

    data = vitals.getVitalData(health);
    EXPECT_EQ(data.current, 80);
    EXPECT_EQ(data.currentMax, 100);

    data = vitals.getVitalData("invalid");
    EXPECT_EQ(data.current, 0);
    EXPECT_EQ(data.currentMax, 100);
}

TEST_F(Vitals_T, AddVitalData)
{
    EXPECT_TRUE(vitals.addVitalData("Health", 80, 0, 100));
    EXPECT_FALSE(vitals.addVitalData("Health", 50, 0, 100)); // Already exists
    EXPECT_FALSE(vitals.addVitalData("invalid", 50, 0, 100)); // Invalid vital
    EXPECT_FALSE(vitals.addVitalData(1, 150, 0, 100)); // Current > max
    EXPECT_FALSE(vitals.addVitalData(1, 50, 120, 100)); // Min > max

    VitalData data = vitals.getVitalData(1);
    EXPECT_EQ(data.current, 80);
    EXPECT_EQ(data.currentMin, 0);
    EXPECT_EQ(data.currentMax, 100);
}

TEST_F(Vitals_T, UpdateVitalCurrent)
{
    vitals.addVitalData("Health", 80, 0, 100);
    EXPECT_TRUE(vitals.updateVitalCurrent("Health", 90));
    EXPECT_EQ(vitals.getVitalData("Health").current, 90);

    EXPECT_TRUE(vitals.updateVitalCurrent(1, 120)); // Clamped to max
    EXPECT_EQ(vitals.getVitalData(1).current, 100);

    EXPECT_TRUE(vitals.updateVitalCurrent(health, -10)); // Clamped to min
    EXPECT_EQ(vitals.getVitalData(health).current, 0);

    EXPECT_FALSE(vitals.updateVitalCurrent("invalid", 50));
}

TEST_F(Vitals_T, UpdateVitalMin)
{
    vitals.addVitalData("Health", 80, 0, 100);
    EXPECT_TRUE(vitals.updateVitalMin("Health", 10));
    EXPECT_EQ(vitals.getVitalData("Health").currentMin, 10);
    EXPECT_EQ(vitals.getVitalData("Health").current, 80); // Current unchanged

    EXPECT_TRUE(vitals.updateVitalMin(1, 90));
    EXPECT_EQ(vitals.getVitalData(1).currentMin, 90);
    EXPECT_EQ(vitals.getVitalData(1).current, 90); // Current clamped to new min

    EXPECT_FALSE(vitals.updateVitalMin(1, 150)); // Min > max
    EXPECT_FALSE(vitals.updateVitalMin("invalid", 10));
}

TEST_F(Vitals_T, UpdateVitalMax)
{
    vitals.addVitalData("Health", 80, 0, 100);
    EXPECT_TRUE(vitals.updateVitalMax("Health", 150));
    EXPECT_EQ(vitals.getVitalData("Health").currentMax, 150);
    EXPECT_EQ(vitals.getVitalData("Health").current, 80); // Current unchanged

    EXPECT_TRUE(vitals.updateVitalMax(1, 50));
    EXPECT_EQ(vitals.getVitalData(1).currentMax, 50);
    EXPECT_EQ(vitals.getVitalData(1).current, 50); // Current clamped to new max

    EXPECT_FALSE(vitals.updateVitalMax(1, -10)); // Max < min
    EXPECT_FALSE(vitals.updateVitalMax("invalid", 50));
}

TEST_F(Vitals_T, AddRemoveVitalModifier)
{
    // Create Health with min 0 and max 100.
    vitals.addVitalData("Health", 80, 0, 100);
    
    // Add +20 modifier to Health max: min 0 and max 120.
    EXPECT_TRUE(vitals.addVitalModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20, 
                                        VitalDataModifierTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.getVitalEffectiveMax("Health"), 120);
    
    // Add -10 modifier to Health max: min 0 and max 110.
    EXPECT_TRUE(vitals.addVitalModifier(1, 2, rpg::ModifierSourceType::ITEM, -10, 
                                        VitalDataModifierTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.getVitalEffectiveMax(1), 110);

    // Add +5 modifier to Health min: min 5 and max 110.
    EXPECT_TRUE(vitals.addVitalModifier(health, 3, rpg::ModifierSourceType::BUFF, 5, 
                                        VitalDataModifierTarget::CURRENT_MIN));
    EXPECT_EQ(vitals.getVitalData(health).minModifiers.size(), 1);
    EXPECT_EQ(vitals.getVitalEffectiveMin(health), 5);

    // Remove modifier to Health max: min 5 and max 90.
    EXPECT_TRUE(vitals.removeVitalModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 
                                           VitalDataModifierTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.getVitalEffectiveMax("Health"), 90);
    
    // Remove non-existant modifier to Health max: min 5 and max 90.
    EXPECT_FALSE(vitals.removeVitalModifier("Health", 999, rpg::ModifierSourceType::ATTRIBUTE, 
                                            VitalDataModifierTarget::CURRENT_MAX));

    // Add +20 modifier to Health max: min 5 and max 110.
    EXPECT_FALSE(vitals.addVitalModifier("invalid", 1, rpg::ModifierSourceType::ATTRIBUTE, 20, 
                                         VitalDataModifierTarget::CURRENT_MAX));
                                         
    // Add +inf modifier to Health max: min 5 and max inf.
    EXPECT_FALSE(vitals.addVitalModifier(1, 1, rpg::ModifierSourceType::ATTRIBUTE, 
                                         std::numeric_limits<int>::max(), 
                                         VitalDataModifierTarget::CURRENT_MAX)); // Overflow
}

TEST_F(Vitals_T, RemoveVital)
{
    vitals.addVitalData("Health", 80, 0, 100);
    EXPECT_TRUE(vitals.removeVital("Health"));
    EXPECT_EQ(vitals.getVitalData("Health").currentMax, 100); // Default VitalData
    EXPECT_FALSE(vitals.removeVital("Health")); // Already removed
    EXPECT_FALSE(vitals.removeVital("invalid"));
}

TEST_F(Vitals_T, GetVitalEffectiveMaxMin)
{
    vitals.addVitalData("Health", 80, 0, 100);
    vitals.addVitalModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20, 
                            VitalDataModifierTarget::CURRENT_MAX);
    vitals.addVitalModifier("Health", 2, rpg::ModifierSourceType::ITEM, -10,  
                            VitalDataModifierTarget::CURRENT_MAX);
    vitals.addVitalModifier("Health", 3, rpg::ModifierSourceType::BUFF, 5,  
                            VitalDataModifierTarget::CURRENT_MIN);

    EXPECT_EQ(vitals.getVitalEffectiveMax("Health"), 110);
    EXPECT_EQ(vitals.getVitalEffectiveMin("Health"), 5);

    EXPECT_EQ(vitals.getVitalEffectiveMax(999), 0);
    EXPECT_EQ(vitals.getVitalEffectiveMin("invalid"), 0);
}


TEST_F(Vitals_T, SerializeDeserialize)
{
    vitals.addVitalData("Health", 80, 0, 100);
    vitals.addVitalData("Mana", 50, 0, 200);
    vitals.addVitalModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20,  
                            VitalDataModifierTarget::CURRENT_MAX);
    vitals.addVitalModifier("Health", 2, rpg::ModifierSourceType::BUFF, 5,  
                            VitalDataModifierTarget::CURRENT_MIN);

    std::string serialized = vitals.serialize();
    Vitals newVitals = Vitals::deserialize(serialized);

    EXPECT_EQ(newVitals.getVitalData("Health").current, 80);
    EXPECT_EQ(newVitals.getVitalEffectiveMax("Health"), 120);
    EXPECT_EQ(newVitals.getVitalEffectiveMin("Health"), 5);
    EXPECT_EQ(newVitals.getVitalData("Mana").currentMax, 200);

    EXPECT_THROW({
        Vitals::deserialize("invalid data");
    }, std::invalid_argument);
}
