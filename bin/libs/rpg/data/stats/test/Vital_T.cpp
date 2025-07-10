/**
 * File: Vital_T.cpp
 * Author: Antonius Torode
 * Created on: 07/09/2025
 * Description: Unit tests for the rpg::Vital class and VitalType utilities.
 */

#include <gtest/gtest.h>
#include "Vital.hpp"

using namespace stats;

/**
 * Test suite for stringToVitalType and vitalTypeToString conversions.
 */
TEST(VitalTypeConversionTest, StringToEnum)
{
    EXPECT_EQ(stringToVitalType("DEPLETIVE"), VitalType::DEPLETIVE);
    EXPECT_EQ(stringToVitalType("accumulative"), VitalType::ACCUMULATIVE);
    EXPECT_EQ(stringToVitalType("unknown"), VitalType::UNKNOWN);
    EXPECT_EQ(stringToVitalType("invalid"), VitalType::UNKNOWN);
}

TEST(VitalTypeConversionTest, EnumToString)
{
    EXPECT_EQ(vitalTypeToString(VitalType::DEPLETIVE), "DEPLETIVE");
    EXPECT_EQ(vitalTypeToString(VitalType::ACCUMULATIVE), "ACCUMULATIVE");
    EXPECT_EQ(vitalTypeToString(VitalType::UNKNOWN), "UNKNOWN");
}

/**
 * Test suite for default and parameterized construction of Vital.
 */
TEST(Vital_T, DefaultConstruction)
{
    Vital v;
    EXPECT_EQ(v.getID(), 0u);
    EXPECT_EQ(v.getName(), "");
    EXPECT_EQ(v.getDescription(), "");
    EXPECT_EQ(v.getVitalType(), VitalType::UNKNOWN);
    EXPECT_EQ(v.getBaseMax(), 100);
    EXPECT_EQ(v.getBaseMin(), 0);
}

TEST(Vital_T, ParameterizedConstructionDepletive)
{
    Vital v(1, "Health", "Health bar", VitalType::DEPLETIVE);
    EXPECT_EQ(v.getID(), 1u);
    EXPECT_EQ(v.getName(), "Health");
    EXPECT_EQ(v.getDescription(), "Health bar");
    EXPECT_EQ(v.getVitalType(), VitalType::DEPLETIVE);
    EXPECT_EQ(v.getBaseMax(), 100);
    EXPECT_EQ(v.getBaseMin(), 0);
}

/**
 * Test suite for JSON serialization.
 */
TEST(Vital_T, SerializationToJson)
{
    Vital v(3, "Stamina", "Energy reserve", VitalType::DEPLETIVE, 25, 150);
    auto json = v.toJson();

    EXPECT_EQ(json["id"], 3u);
    EXPECT_EQ(json["name"], "Stamina");
    EXPECT_EQ(json["description"], "Energy reserve");
    EXPECT_EQ(json["type"], "DEPLETIVE");
    EXPECT_EQ(json["baseMax"], 150);
    EXPECT_EQ(json["baseMin"], 25);
}

