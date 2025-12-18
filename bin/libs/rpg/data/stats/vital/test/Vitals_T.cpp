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

TEST_F(Vitals_T, updateCurrent)
{
    vitals.add("Health", 80, 0, 100);

    // Valid update within range
    EXPECT_NO_THROW(vitals.update("Health", VitalDataTarget::CURRENT, 90));
    EXPECT_EQ(vitals.get("Health").getCurrent(), 90);

    // Update above max should clamp to max
    EXPECT_NO_THROW(vitals.update(1, VitalDataTarget::CURRENT, 120));
    EXPECT_EQ(vitals.get(1).getCurrent(), 100);

    // Update below min should clamp to min
    EXPECT_NO_THROW(vitals.update(health, VitalDataTarget::CURRENT, -10));
    EXPECT_EQ(vitals.get(health).getCurrent(), 0);

    // Invalid vital name should throw
    EXPECT_THROW(vitals.update("invalid", VitalDataTarget::CURRENT, 50), error::MIAException);
}

TEST_F(Vitals_T, updateMin)
{
    vitals.add("Health", 80, 0, 100);

    // Valid update: raise min to 10
    EXPECT_NO_THROW(vitals.update("Health", VitalDataTarget::CURRENT_MIN, 10));
    EXPECT_EQ(vitals.get("Health").getCurrentMin(), 10);
    EXPECT_EQ(vitals.get("Health").getCurrent(), 80); // Current unchanged

    // Valid update: raise min to 90, clamps current
    EXPECT_NO_THROW(vitals.update(1, VitalDataTarget::CURRENT_MIN, 90));
    EXPECT_EQ(vitals.get(1).getCurrentMin(), 90);
    EXPECT_EQ(vitals.get(1).getCurrent(), 90); // Clamped

    // Invalid: min > max
    EXPECT_THROW(vitals.update(1, VitalDataTarget::CURRENT_MIN, 150), error::MIAException);

    // Invalid vital
    EXPECT_THROW(vitals.update("invalid", VitalDataTarget::CURRENT_MIN, 10), error::MIAException);
}

TEST_F(Vitals_T, updateMax)
{
    vitals.add("Health", 80, 0, 100);

    // Valid update: raise max to 150
    EXPECT_NO_THROW(vitals.update("Health", VitalDataTarget::CURRENT_MAX, 150));
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 150);
    EXPECT_EQ(vitals.get("Health").getCurrent(), 80); // Current unchanged

    // Valid update: lower max to 50, clamps current
    EXPECT_NO_THROW(vitals.update(1, VitalDataTarget::CURRENT_MAX, 50));
    EXPECT_EQ(vitals.get(1).getCurrentMax(), 50);
    EXPECT_EQ(vitals.get(1).getCurrent(), 50); // Clamped

    // Invalid: max < min
    EXPECT_THROW(vitals.update(1, VitalDataTarget::CURRENT_MAX, -10), error::MIAException);

    // Invalid vital
    EXPECT_THROW(vitals.update("invalid", VitalDataTarget::CURRENT_MAX, 50), error::MIAException);
}


TEST_F(Vitals_T, AddremoveModifier)
{
    vitals.add("Health", 80, 0, 100);

    // Add +20 modifier to max → max becomes 120
    EXPECT_NO_THROW(vitals.addModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE, 20,
                                       VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 120);

    // Add -10 modifier to max → max becomes 110
    EXPECT_NO_THROW(vitals.addModifier(1, 2, rpg::ModifierSourceType::ITEM, -10,
                                            VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.get(1).getCurrentMax(), 110);

    // Add +5 modifier to min → min becomes 5
    EXPECT_NO_THROW(vitals.addModifier(health, 3, rpg::ModifierSourceType::BUFF, 5,
                                            VitalDataTarget::CURRENT_MIN));
    EXPECT_EQ(vitals.get(health).getModifiers(VitalDataTarget::CURRENT_MIN).size(), 1);
    EXPECT_EQ(vitals.get(health).getCurrentMin(), 5);

    // Remove modifier with sourceId 1 from max → max becomes 90
    EXPECT_NO_THROW(vitals.removeModifier("Health", 1, rpg::ModifierSourceType::ATTRIBUTE,
                                          VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 90);

    // Remove non-existent modifier → no exception; data unchanged
    EXPECT_NO_THROW(vitals.removeModifier("Health", 999, rpg::ModifierSourceType::ATTRIBUTE,
                                          VitalDataTarget::CURRENT_MAX));
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 90); // Confirm unchanged

    // Invalid vital name → throws
    EXPECT_THROW(vitals.addModifier("invalid", 1, rpg::ModifierSourceType::ATTRIBUTE, 20,
                                    VitalDataTarget::CURRENT_MAX), error::MIAException);

    // Overflow risk (optional test) — depending on logic, this may or may not throw
    EXPECT_NO_THROW(vitals.addModifier(1, 1, rpg::ModifierSourceType::ATTRIBUTE,
                                       std::numeric_limits<int>::max(),
                                       VitalDataTarget::CURRENT_MAX));
    // The resulting max may wrap, clamp, or just increase depending on implementation
}

TEST_F(Vitals_T, remove)
{
    vitals.add("Health", 80, 0, 100);

    // Remove Health → data removed from internal map
    EXPECT_NO_THROW(vitals.remove("Health"));

    // Querying after removal should return default
    EXPECT_EQ(vitals.get("Health").getCurrentMax(), 100); // From fallback/default

    // Attempting second removal does not throw, but does nothing
    EXPECT_NO_THROW(vitals.remove("Health"));

    // Remove invalid vital → throws
    EXPECT_THROW(vitals.remove("invalid"), error::MIAException);
}

TEST_F(Vitals_T, has)
{
    vitals.add("Health", 80, 0, 100);

    // Test whether or not having enough works.
    EXPECT_TRUE(vitals.has("Health", 50));

    // Test whether or not it correctly finds not enough..
    EXPECT_FALSE(vitals.has("Health", 90));

    // Invalid vital → throws
    EXPECT_THROW(vitals.has("invalid", 5), error::MIAException);
}

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

    EXPECT_THROW({
        Vitals::deserialize("invalid data");
    }, std::invalid_argument);
}
