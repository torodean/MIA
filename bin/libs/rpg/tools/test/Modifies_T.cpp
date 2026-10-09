/**
 * @file Modifies_T.cpp
 * @author Antonius Torode
 * @date 10/09/2026
 * @brief Unit tests for the Modifies struct and the ModifyType conversion helpers.
 */

#include <sstream>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

// Include the associated file for testing.
#include "Modifies.hpp"
// Used for exception assertions.
#include "MIAException.hpp"

namespace
{
    /**
     * Builds a sample Modifies declaration.
     * @return A Modifies which adds 5.0 per unit to the Health vital.
     */
    rpg::Modifies sampleModifies()
    {
        return rpg::Modifies(rpg::DataType::VITAL, "Health", rpg::ModifyType::ADD_MAX, 5.0);
    }
} // namespace


/**
 * @brief Verifies every ModifyType converts to its expected string.
 */
TEST(ModifiesTest, ModifyTypeToString)
{
    EXPECT_EQ(rpg::modifyTypeToString(rpg::ModifyType::ADD_MAX), "ADD_MAX");
    EXPECT_EQ(rpg::modifyTypeToString(rpg::ModifyType::MULTIPLY), "MULTIPLY");
    EXPECT_EQ(rpg::modifyTypeToString(rpg::ModifyType::SET), "SET");
    EXPECT_EQ(rpg::modifyTypeToString(rpg::ModifyType::UNKNOWN), "UNKNOWN");
}


/**
 * @brief Verifies string parsing is case-insensitive and unknown strings map to UNKNOWN.
 */
TEST(ModifiesTest, StringToModifyType)
{
    EXPECT_EQ(rpg::stringToModifyType("ADD_MAX"), rpg::ModifyType::ADD_MAX);
    EXPECT_EQ(rpg::stringToModifyType("add_max"), rpg::ModifyType::ADD_MAX);
    EXPECT_EQ(rpg::stringToModifyType("Multiply"), rpg::ModifyType::MULTIPLY);
    EXPECT_EQ(rpg::stringToModifyType("set"), rpg::ModifyType::SET);
    EXPECT_EQ(rpg::stringToModifyType("nonsense"), rpg::ModifyType::UNKNOWN);
}


/**
 * @brief Verifies every ModifierStackPolicy converts to its expected string.
 */
TEST(ModifiesTest, ModifierStackPolicyToString)
{
    EXPECT_EQ(rpg::modifierStackPolicyToString(rpg::ModifierStackPolicy::REPLACE), "REPLACE");
    EXPECT_EQ(rpg::modifierStackPolicyToString(rpg::ModifierStackPolicy::STACK), "STACK");
    EXPECT_EQ(rpg::modifierStackPolicyToString(rpg::ModifierStackPolicy::KEEP_STRONGEST),
              "KEEP_STRONGEST");
    EXPECT_EQ(rpg::modifierStackPolicyToString(rpg::ModifierStackPolicy::UNKNOWN), "UNKNOWN");
}


/**
 * @brief Verifies stack policy parsing is case-insensitive and unknown strings map to UNKNOWN.
 */
TEST(ModifiesTest, StringToModifierStackPolicy)
{
    EXPECT_EQ(rpg::stringToModifierStackPolicy("REPLACE"), rpg::ModifierStackPolicy::REPLACE);
    EXPECT_EQ(rpg::stringToModifierStackPolicy("stack"), rpg::ModifierStackPolicy::STACK);
    EXPECT_EQ(rpg::stringToModifierStackPolicy("Keep_Strongest"),
              rpg::ModifierStackPolicy::KEEP_STRONGEST);
    EXPECT_EQ(rpg::stringToModifierStackPolicy("nonsense"), rpg::ModifierStackPolicy::UNKNOWN);
}


/**
 * @brief Verifies the constructor stores every field.
 */
TEST(ModifiesTest, ConstructorInitializesFields)
{
    rpg::Modifies modifies = sampleModifies();

    EXPECT_EQ(modifies.targetType, rpg::DataType::VITAL);
    EXPECT_EQ(modifies.targetName, "Health");
    EXPECT_EQ(modifies.modifyType, rpg::ModifyType::ADD_MAX);
    EXPECT_DOUBLE_EQ(modifies.modifyValuePer, 5.0);
}


/**
 * @brief Verifies equality compares all four fields.
 */
TEST(ModifiesTest, EqualityComparesAllFields)
{
    rpg::Modifies base = sampleModifies();

    EXPECT_TRUE(base == sampleModifies());

    EXPECT_FALSE(base == rpg::Modifies(rpg::DataType::ATTRIBUTE, "Health",
                                       rpg::ModifyType::ADD_MAX, 5.0));
    EXPECT_FALSE(base == rpg::Modifies(rpg::DataType::VITAL, "Mana",
                                       rpg::ModifyType::ADD_MAX, 5.0));
    EXPECT_FALSE(base == rpg::Modifies(rpg::DataType::VITAL, "Health",
                                       rpg::ModifyType::MULTIPLY, 5.0));
    EXPECT_FALSE(base == rpg::Modifies(rpg::DataType::VITAL, "Health",
                                       rpg::ModifyType::ADD_MAX, 10.0));
}


/**
 * @brief Verifies the serialized field order and format.
 */
TEST(ModifiesTest, SerializeFormat)
{
    EXPECT_EQ(sampleModifies().serialize(), "VITAL:Health:ADD_MAX:5.000000:REPLACE");
}


/**
 * @brief Verifies a serialize/deserialize round trip preserves every field.
 */
TEST(ModifiesTest, DeserializeRoundTrip)
{
    std::vector<rpg::Modifies> originals{
        sampleModifies(),
        rpg::Modifies(rpg::DataType::VITAL, "Mana", rpg::ModifyType::MULTIPLY, 0.1,
                      rpg::ModifierStackPolicy::STACK),
        rpg::Modifies(rpg::DataType::VITAL, "Mana", rpg::ModifyType::SET, 50.0,
                      rpg::ModifierStackPolicy::KEEP_STRONGEST)
    };

    for (const auto& original : originals)
    {
        rpg::Modifies restored = rpg::Modifies::deserialize(original.serialize());
        EXPECT_TRUE(restored == original)
            << "Round trip through serialize/deserialize should preserve " << original.serialize();
    }
}


/**
 * @brief Verifies the JSON round trip preserves every field.
 */
TEST(ModifiesTest, JsonRoundTrip)
{
    rpg::Modifies original(rpg::DataType::VITAL, "Health", rpg::ModifyType::ADD_MAX, 5.0,
                           rpg::ModifierStackPolicy::KEEP_STRONGEST);

    rpg::Modifies restored = rpg::Modifies::fromJson(original.toJson());
    EXPECT_TRUE(restored == original) << "A JSON round trip should preserve every field.";
}


/**
 * @brief Verifies the JSON stack policy is optional and defaults to REPLACE.
 */
TEST(ModifiesTest, Json_MissingStackPolicyDefaults)
{
    nlohmann::json json = {
        {"targetType", "VITAL"},
        {"targetName", "Health"},
        {"ModifyType", "ADD_MAX"},
        {"ModifyValuePer", 5.0}
    };

    rpg::Modifies modifies = rpg::Modifies::fromJson(json);
    EXPECT_EQ(modifies.stackPolicy, rpg::ModifierStackPolicy::REPLACE)
        << "A missing StackPolicy should default to REPLACE.";
}


/**
 * @brief Verifies deserializing a string with the wrong token count throws.
 */
TEST(ModifiesTest, Deserialize_WrongTokenCountThrows)
{
    EXPECT_THROW(rpg::Modifies::deserialize("VITAL:Health:ADD_MAX"), error::MIAException);
    EXPECT_THROW(rpg::Modifies::deserialize("VITAL:Health:ADD_MAX:5.0"), error::MIAException);
    EXPECT_THROW(rpg::Modifies::deserialize("VITAL:Health:ADD_MAX:5.0:REPLACE:extra"),
                 error::MIAException);
    EXPECT_THROW(rpg::Modifies::deserialize(""), error::MIAException);
}


/**
 * @brief Verifies unrecognized type tokens parse to UNKNOWN instead of throwing.
 */
TEST(ModifiesTest, Deserialize_UnknownTypeTokens)
{
    rpg::Modifies modifies = rpg::Modifies::deserialize("FOO:Health:BAR:1.5:STACK");

    EXPECT_EQ(modifies.targetType, rpg::DataType::UNKNOWN);
    EXPECT_EQ(modifies.targetName, "Health");
    EXPECT_EQ(modifies.modifyType, rpg::ModifyType::UNKNOWN);
    EXPECT_DOUBLE_EQ(modifies.modifyValuePer, 1.5);
    EXPECT_EQ(modifies.stackPolicy, rpg::ModifierStackPolicy::STACK);
}


/**
 * @brief Verifies a non-numeric value token is rejected.
 * The exception currently comes from std::stod rather than MIAException.
 */
TEST(ModifiesTest, Deserialize_NonNumericValueThrows)
{
    EXPECT_ANY_THROW(rpg::Modifies::deserialize("VITAL:Health:ADD_MAX:not_a_number"));
}


/**
 * @brief Verifies the stream operator output format.
 */
TEST(ModifiesTest, StreamOperator)
{
    std::ostringstream oss;
    oss << sampleModifies();
    EXPECT_EQ(oss.str(),
              "Modifies{targetName=Health, modifyType=ADD_MAX, modifyValuePer=5, stackPolicy=REPLACE}");
}
