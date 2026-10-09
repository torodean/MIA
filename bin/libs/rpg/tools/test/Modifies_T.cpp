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

namespace rpg
{
    namespace
    {
        /**
         * Builds a sample Modifies declaration.
         * @return A Modifies which adds 5.0 per unit to the Health vital.
         */
        Modifies sampleModifies()
        {
            return Modifies(DataType::VITAL, "Health", ModifyType::ADD_MAX, 5.0);
        }
    } // namespace


    /**
     * @brief Verifies every ModifyType converts to its expected string.
     */
    TEST(ModifiesTest, ModifyTypeToString)
    {
        EXPECT_EQ(modifyTypeToString(ModifyType::ADD_MAX), "ADD_MAX");
        EXPECT_EQ(modifyTypeToString(ModifyType::MULTIPLY), "MULTIPLY");
        EXPECT_EQ(modifyTypeToString(ModifyType::SET), "SET");
        EXPECT_EQ(modifyTypeToString(ModifyType::UNKNOWN), "UNKNOWN");
    }


    /**
     * @brief Verifies string parsing is case-insensitive and unknown strings map to UNKNOWN.
     */
    TEST(ModifiesTest, StringToModifyType)
    {
        EXPECT_EQ(stringToModifyType("ADD_MAX"), ModifyType::ADD_MAX);
        EXPECT_EQ(stringToModifyType("add_max"), ModifyType::ADD_MAX);
        EXPECT_EQ(stringToModifyType("Multiply"), ModifyType::MULTIPLY);
        EXPECT_EQ(stringToModifyType("set"), ModifyType::SET);
        EXPECT_EQ(stringToModifyType("nonsense"), ModifyType::UNKNOWN);
    }


    /**
     * @brief Verifies every ModifierStackPolicy converts to its expected string.
     */
    TEST(ModifiesTest, ModifierStackPolicyToString)
    {
        EXPECT_EQ(modifierStackPolicyToString(ModifierStackPolicy::REPLACE), "REPLACE");
        EXPECT_EQ(modifierStackPolicyToString(ModifierStackPolicy::STACK), "STACK");
        EXPECT_EQ(modifierStackPolicyToString(ModifierStackPolicy::KEEP_STRONGEST),
                  "KEEP_STRONGEST");
        EXPECT_EQ(modifierStackPolicyToString(ModifierStackPolicy::UNKNOWN), "UNKNOWN");
    }


    /**
     * @brief Verifies stack policy parsing is case-insensitive and unknown strings map to UNKNOWN.
     */
    TEST(ModifiesTest, StringToModifierStackPolicy)
    {
        EXPECT_EQ(stringToModifierStackPolicy("REPLACE"), ModifierStackPolicy::REPLACE);
        EXPECT_EQ(stringToModifierStackPolicy("stack"), ModifierStackPolicy::STACK);
        EXPECT_EQ(stringToModifierStackPolicy("Keep_Strongest"),
                  ModifierStackPolicy::KEEP_STRONGEST);
        EXPECT_EQ(stringToModifierStackPolicy("nonsense"), ModifierStackPolicy::UNKNOWN);
    }


    /**
     * @brief Verifies the constructor stores every field.
     */
    TEST(ModifiesTest, ConstructorInitializesFields)
    {
        Modifies modifies = sampleModifies();

        EXPECT_EQ(modifies.targetType, DataType::VITAL);
        EXPECT_EQ(modifies.targetName, "Health");
        EXPECT_EQ(modifies.modifyType, ModifyType::ADD_MAX);
        EXPECT_DOUBLE_EQ(modifies.modifyValuePer, 5.0);
    }


    /**
     * @brief Verifies equality compares all four fields.
     */
    TEST(ModifiesTest, EqualityComparesAllFields)
    {
        Modifies base = sampleModifies();

        EXPECT_TRUE(base == sampleModifies());

        EXPECT_FALSE(base == Modifies(DataType::ATTRIBUTE, "Health",
                                           ModifyType::ADD_MAX, 5.0));
        EXPECT_FALSE(base == Modifies(DataType::VITAL, "Mana",
                                           ModifyType::ADD_MAX, 5.0));
        EXPECT_FALSE(base == Modifies(DataType::VITAL, "Health",
                                           ModifyType::MULTIPLY, 5.0));
        EXPECT_FALSE(base == Modifies(DataType::VITAL, "Health",
                                           ModifyType::ADD_MAX, 10.0));
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
        std::vector<Modifies> originals{
            sampleModifies(),
            Modifies(DataType::VITAL, "Mana", ModifyType::MULTIPLY, 0.1,
                          ModifierStackPolicy::STACK),
            Modifies(DataType::VITAL, "Mana", ModifyType::SET, 50.0,
                          ModifierStackPolicy::KEEP_STRONGEST)
        };

        for (const auto& original : originals)
        {
            Modifies restored = Modifies::deserialize(original.serialize());
            EXPECT_TRUE(restored == original)
                << "Round trip through serialize/deserialize should preserve " << original.serialize();
        }
    }


    /**
     * @brief Verifies the JSON round trip preserves every field.
     */
    TEST(ModifiesTest, JsonRoundTrip)
    {
        Modifies original(DataType::VITAL, "Health", ModifyType::ADD_MAX, 5.0,
                               ModifierStackPolicy::KEEP_STRONGEST);

        Modifies restored = Modifies::fromJson(original.toJson());
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

        Modifies modifies = Modifies::fromJson(json);
        EXPECT_EQ(modifies.stackPolicy, ModifierStackPolicy::REPLACE)
            << "A missing StackPolicy should default to REPLACE.";
    }


    /**
     * @brief Verifies deserializing a string with the wrong token count throws.
     */
    TEST(ModifiesTest, Deserialize_WrongTokenCountThrows)
    {
        EXPECT_THROW(Modifies::deserialize("VITAL:Health:ADD_MAX"), error::MIAException);
        EXPECT_THROW(Modifies::deserialize("VITAL:Health:ADD_MAX:5.0"), error::MIAException);
        EXPECT_THROW(Modifies::deserialize("VITAL:Health:ADD_MAX:5.0:REPLACE:extra"),
                     error::MIAException);
        EXPECT_THROW(Modifies::deserialize(""), error::MIAException);
    }


    /**
     * @brief Verifies unrecognized type tokens parse to UNKNOWN instead of throwing.
     */
    TEST(ModifiesTest, Deserialize_UnknownTypeTokens)
    {
        Modifies modifies = Modifies::deserialize("FOO:Health:BAR:1.5:STACK");

        EXPECT_EQ(modifies.targetType, DataType::UNKNOWN);
        EXPECT_EQ(modifies.targetName, "Health");
        EXPECT_EQ(modifies.modifyType, ModifyType::UNKNOWN);
        EXPECT_DOUBLE_EQ(modifies.modifyValuePer, 1.5);
        EXPECT_EQ(modifies.stackPolicy, ModifierStackPolicy::STACK);
    }


    /**
     * @brief Verifies a non-numeric value token is rejected with a MIAException.
     */
    TEST(ModifiesTest, Deserialize_NonNumericValueThrows)
    {
        EXPECT_THROW(Modifies::deserialize("VITAL:Health:ADD_MAX:not_a_number"),
                     error::MIAException);
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
} // namespace rpg
