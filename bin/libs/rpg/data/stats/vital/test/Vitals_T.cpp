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
 * Test fixture for Vitals tests.
 */
class Vitals_T : public ::testing::Test 
{
protected:
    void SetUp() override
    {
        // Create a JSON object with the "vitals" key containing the array of vitals
        nlohmann::json jsonObject;
        jsonObject["VITAL"] = { health.toJson(), mana.toJson(), rage.toJson() };
        std::string jsonData = jsonObject.dump();
        
        // Load currencies into registry
        VitalRegistry::getInstance().loadFromString(jsonData);
    }
    
    Vitals vitals;

    Vital health {1, "Health", "The health of a player.", VitalType::DEPLETIVE};
    Vital mana  {2, "Mana", "The mana of a player.", VitalType::DEPLETIVE};
    Vital rage  {3, "Rage", "Therage built up by a player.", VitalType::ACCUMULATIVE};
};


TEST_F(Vitals_T, getData)
{
    vitals.addData("Health", 80, 0, 100);
    VitalData data = vitals.getData("Health");
    EXPECT_EQ(data.getCurrent(), 80);
    EXPECT_EQ(data.getCurrentMin(), 0);
    EXPECT_EQ(data.getCurrentMax(), 100);
    EXPECT_TRUE(data.getModifiers(VitalDataTarget::CURRENT_MAX).empty());
    EXPECT_TRUE(data.getModifiers(VitalDataTarget::CURRENT_MIN).empty());

    data = vitals.getData(1);
    EXPECT_EQ(data.getCurrent(), 80);
    EXPECT_EQ(data.getCurrentMax(), 100);

    data = vitals.getData(health);
    EXPECT_EQ(data.getCurrent(), 80);
    EXPECT_EQ(data.getCurrentMax(), 100);

    EXPECT_THROW(vitals.getData("invalid"), error::MIAException);
}

TEST_F(Vitals_T, addData)
{
    EXPECT_NO_THROW(vitals.addData("Health", 80, 0, 100));
    const VitalData& data = vitals.getData("Health");
    EXPECT_EQ(data.getCurrent(), 80);
    EXPECT_EQ(data.getCurrentMin(), 0);
    EXPECT_EQ(data.getCurrentMax(), 100);
    
    EXPECT_THROW(vitals.addData("Health", 50, 0, 100), error::MIAException); // Already exists
    EXPECT_THROW(vitals.addData("invalid", 50, 0, 100), error::MIAException); // Invalid vital
    EXPECT_THROW(vitals.addData(1, 150, 0, 100), error::MIAException); // Current > max
    EXPECT_THROW(vitals.addData(1, 50, 120, 100), error::MIAException); // Min > max
}

TEST_F(Vitals_T, updateVitalCurrent)
{
    vitals.addData("Health", 80, 0, 100);

    // Valid update within range
    EXPECT_NO_THROW(vitals.updateVital("Health", VitalDataTarget::CURRENT, 90));
    EXPECT_EQ(vitals.getData("Health").getCurrent(), 90);

    // Update above max should clamp to max
    EXPECT_NO_THROW(vitals.updateVital(1, VitalDataTarget::CURRENT, 120));
    EXPECT_EQ(vitals.getData(1).getCurrent(), 100);

    // Update below min should clamp to min
    EXPECT_NO_THROW(vitals.updateVital(health, VitalDataTarget::CURRENT, -10));
    EXPECT_EQ(vitals.getData(health).getCurrent(), 0);

    // Invalid vital name should throw
    EXPECT_THROW(vitals.updateVital("invalid", VitalDataTarget::CURRENT, 50), error::MIAException);
}

TEST_F(Vitals_T, UpdateVitalMin)
{
    vitals.addData("Health", 80, 0, 100);

    // Valid update: raise min to 10
    EXPECT_NO_THROW(vitals.updateVital("Health", VitalDataTarget::CURRENT_MIN, 10));
    EXPECT_EQ(vitals.getData("Health").getCurrentMin(), 10);
    EXPECT_EQ(vitals.getData("Health").getCurrent(), 80); // Current unchanged

    // Valid update: raise min to 90, clamps current
    EXPECT_NO_THROW(vitals.updateVital(1, VitalDataTarget::CURRENT_MIN, 90));
    EXPECT_EQ(vitals.getData(1).getCurrentMin(), 90);
    EXPECT_EQ(vitals.getData(1).getCurrent(), 90); // Clamped

    // Invalid: min > max
    EXPECT_THROW(vitals.updateVital(1, VitalDataTarget::CURRENT_MIN, 150), error::MIAException);

    // Invalid vital
    EXPECT_THROW(vitals.updateVital("invalid", VitalDataTarget::CURRENT_MIN, 10), error::MIAException);
}

TEST_F(Vitals_T, UpdateVitalMax)
{
    vitals.addData("Health", 80, 0, 100);

    // Valid update: raise max to 150
    EXPECT_NO_THROW(vitals.updateVital("Health", VitalDataTarget::CURRENT_MAX, 150));
    EXPECT_EQ(vitals.getData("Health").getCurrentMax(), 150);
    EXPECT_EQ(vitals.getData("Health").getCurrent(), 80); // Current unchanged

    // Valid update: lower max to 50, clamps current
    EXPECT_NO_THROW(vitals.updateVital(1, VitalDataTarget::CURRENT_MAX, 50));
    EXPECT_EQ(vitals.getData(1).getCurrentMax(), 50);
    EXPECT_EQ(vitals.getData(1).getCurrent(), 50); // Clamped

    // Invalid: max < min
    EXPECT_THROW(vitals.updateVital(1, VitalDataTarget::CURRENT_MAX, -10), error::MIAException);

    // Invalid vital
    EXPECT_THROW(vitals.updateVital("invalid", VitalDataTarget::CURRENT_MAX, 50), error::MIAException);
}


TEST_F(Vitals_T, AddRemoveVitalModifier)
{
    vitals.addData("Health", 80, 0, 100);

    // Add +20 modifier to max → max becomes 120
    EXPECT_NO_THROW(vitals.addVitalModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20,
                                            VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.getData("Health").getCurrentMax(), 120);

    // Add -10 modifier to max → max becomes 110
    EXPECT_NO_THROW(vitals.addVitalModifier(1, 2, rpg::ModifierSourceType::ITEM, -10,
                                            VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.getData(1).getCurrentMax(), 110);

    // Add +5 modifier to min → min becomes 5
    EXPECT_NO_THROW(vitals.addVitalModifier(health, 3, rpg::ModifierSourceType::BUFF, 5,
                                            VitalDataTarget::CURRENT_MIN));
    EXPECT_EQ(vitals.getData(health).getModifiers(VitalDataTarget::CURRENT_MIN).size(), 1);
    EXPECT_EQ(vitals.getData(health).getCurrentMin(), 5);

    // Remove modifier with sourceId 1 from max → max becomes 90
    EXPECT_NO_THROW(vitals.removeVitalModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE,
                                               VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.getData("Health").getCurrentMax(), 90);

    // Remove non-existent modifier → no exception; data unchanged
    EXPECT_NO_THROW(vitals.removeVitalModifier("Health", 999, rpg::ModifierSourceType::ATTRIBUTE,
                                               VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.getData("Health").getCurrentMax(), 90); // Confirm unchanged

    // Invalid vital name → throws
    EXPECT_THROW(vitals.addVitalModifier("invalid", 1, rpg::ModifierSourceType::ATTRIBUTE, 20,
                                         VitalDataTarget::CURRENT_MAX), error::MIAException);

    // Overflow risk (optional test) — depending on logic, this may or may not throw
    EXPECT_NO_THROW(vitals.addVitalModifier(1, 1, rpg::ModifierSourceType::ATTRIBUTE,
                                            std::numeric_limits<int>::max(),
                                            VitalDataTarget::CURRENT_MAX));
    // The resulting max may wrap, clamp, or just increase depending on implementation
}

TEST_F(Vitals_T, RemoveVital)
{
    vitals.addData("Health", 80, 0, 100);

    // Remove Health → data removed from internal map
    EXPECT_NO_THROW(vitals.removeVital("Health"));

    // Querying after removal should return default
    EXPECT_EQ(vitals.getData("Health").getCurrentMax(), 100); // From fallback/default

    // Attempting second removal does not throw, but does nothing
    EXPECT_NO_THROW(vitals.removeVital("Health"));

    // Remove invalid vital → throws
    EXPECT_THROW(vitals.removeVital("invalid"), error::MIAException);
}

TEST_F(Vitals_T, GetVitalMaxMin)
{
    vitals.addData("Health", 80, 0, 100);
    vitals.addVitalModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20, 
                            VitalDataTarget::CURRENT_MAX);
    vitals.addVitalModifier("Health", 2, rpg::ModifierSourceType::ITEM, -10,  
                            VitalDataTarget::CURRENT_MAX);
    vitals.addVitalModifier("Health", 3, rpg::ModifierSourceType::BUFF, 5,  
                            VitalDataTarget::CURRENT_MIN);

    EXPECT_EQ(vitals.getData("Health").getCurrentMax(), 110);
    EXPECT_EQ(vitals.getData("Health").getCurrentMin(), 5);

    EXPECT_THROW(vitals.getData(999).getCurrentMax(), error::MIAException);
    EXPECT_THROW(vitals.getData("invalid").getCurrentMin(), error::MIAException);
}


TEST_F(Vitals_T, SerializeDeserialize)
{
    vitals.addData("Health", 80, 0, 100);
    vitals.addData("Mana", 50, 0, 200);
    vitals.addVitalModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20,  
                            VitalDataTarget::CURRENT_MAX);
    vitals.addVitalModifier("Health", 2, rpg::ModifierSourceType::BUFF, 5,  
                            VitalDataTarget::CURRENT_MIN);

    std::string serialized = vitals.serialize();
    Vitals newVitals = Vitals::deserialize(serialized);

    EXPECT_EQ(newVitals.getData("Health").getCurrent(), 80);
    EXPECT_EQ(newVitals.getData("Health").getCurrentMax(), 120);
    EXPECT_EQ(newVitals.getData("Health").getCurrentMin(), 5);
    EXPECT_EQ(newVitals.getData("Mana").getCurrentMax(), 200);

    EXPECT_THROW({
        Vitals::deserialize("invalid data");
    }, std::invalid_argument);
}
