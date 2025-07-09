/**
 * File: Vitals_T.cpp
 * Author: Antonius Torode
 * Created on: 07/09/2025
 * Description: Unit tests for the rpg::Vitals class.
 */

#include <gtest/gtest.h>
#include "Vitals.hpp"

using namespace rpg;

/**
 * Helper to create a sample Vital object.
 */
Vital makeVital(uint32_t id, const std::string& name, VitalType type, int current = 0, int max = 100, int min = 0)
{
    return Vital(id, name, name + " desc", type, current, max, min);
}

TEST(Vitals_T, AddVitalByName)
{
    Vitals v;
    EXPECT_TRUE(v.addVital("Health"));
    EXPECT_FALSE(v.addVital("Health")); // already exists
}

TEST(Vitals_T, AddVitalById)
{
    Vitals v;
    EXPECT_TRUE(v.addVital(1));
    EXPECT_FALSE(v.addVital(1));
}

TEST(Vitals_T, AddVitalObject)
{
    Vitals v;
    auto vital = makeVital(10, "Mana", VitalType::DEPLETIVE);
    EXPECT_TRUE(v.addVital(vital));
    EXPECT_FALSE(v.addVital(vital)); // already exists
}

TEST(Vitals_T, GetVitalByIdOrName)
{
    Vitals v;
    auto vital = makeVital(2, "Stamina", VitalType::DEPLETIVE);
    v.addVital(vital);

    auto retrievedById = v.getVital(2);
    EXPECT_EQ(retrievedById.getName(), "Stamina");

    auto retrievedByName = v.getVital("Stamina");
    EXPECT_EQ(retrievedByName.getID(), 2u);

    auto defaultVital = v.getVital("Nonexistent");
    EXPECT_EQ(defaultVital.getID(), 0u);
}

TEST(Vitals_T, UpdateVitalValues)
{
    Vitals v;
    auto vital = makeVital(3, "Rage", VitalType::ACCUMULATIVE);
    v.addVital(vital);

    EXPECT_TRUE(v.updateVitalCurrent("Rage", 50));
    EXPECT_EQ(v.getVital(3).getCurrent(), 50);

    EXPECT_TRUE(v.updateVitalMin(3, 10));
    EXPECT_EQ(v.getVital(3).getMin(), 10);

    EXPECT_TRUE(v.updateVitalMax("Rage", 250));
    EXPECT_EQ(v.getVital(3).getMax(), 250);
}

TEST(Vitals_T, UpdateNonexistentFails)
{
    Vitals v;
    EXPECT_FALSE(v.updateVitalCurrent("X", 1));
    EXPECT_FALSE(v.updateVitalMin(99, 1));
    EXPECT_FALSE(v.updateVitalMax("Y", 1));
}

TEST(Vitals_T, RemoveVital)
{
    Vitals v;
    auto vital = makeVital(4, "Energy", VitalType::ACCUMULATIVE);
    v.addVital(vital);

    EXPECT_TRUE(v.removeVital("Energy"));
    EXPECT_FALSE(v.removeVital("Energy"));

    v.addVital(vital);
    EXPECT_TRUE(v.removeVital(4));
    EXPECT_FALSE(v.removeVital(4));

    v.addVital(vital);
    EXPECT_TRUE(v.removeVital(vital));
    EXPECT_FALSE(v.removeVital(vital));
}

TEST(Vitals_T, SerializeAndDeserialize)
{
    Vitals v;
    v.addVital(makeVital(1, "HP", VitalType::DEPLETIVE, 95, 100, 0));
    v.addVital(makeVital(2, "MP", VitalType::DEPLETIVE, 20, 50, 0));

    std::string serialized = v.serialize();
    EXPECT_TRUE(serialized.find("[CV_BEGIN]") != std::string::npos);
    EXPECT_TRUE(serialized.find("[CV_END]") != std::string::npos);

    Vitals newVitals;
    EXPECT_TRUE(newVitals.deserialize(serialized));

    EXPECT_EQ(newVitals.getVital(1).getCurrent(), 95);
    EXPECT_EQ(newVitals.getVital(2).getMax(), 50);
}

TEST(Vitals_T, DeserializeMalformedReturnsFalse)
{
    Vitals v;
    EXPECT_FALSE(v.deserialize("invalid data with no markers"));
    EXPECT_FALSE(v.deserialize("[CV_BEGIN]HP:100,abc,0;[CV_END]")); // malformed number
    EXPECT_FALSE(v.deserialize("[CV_BEGIN]NoColon100,100,0;[CV_END]"));
}

