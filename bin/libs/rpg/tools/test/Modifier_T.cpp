/**
 * @file Modifier_T.cpp
 * @author Antonius Torode
 * @date 10/09/2026
 * @brief Unit tests for the Modifier struct and the computeModifiedValue utility.
 */

#include <limits>
#include <sstream>
#include <vector>

#include <gtest/gtest.h>

// Include the associated file for testing.
#include "Modifier.hpp"
// Used for exception assertions.
#include "MIAException.hpp"

namespace rpg
{
    namespace
    {
        /**
         * Builds a single modifier with an int value.
         *
         * @param id The source ID.
         * @param type The modification type.
         * @param value The modifier value.
         * @param policy The stack policy; defaults to REPLACE.
         * @return A Modifier with the given fields and an ATTRIBUTE source.
         */
        Modifier intModifier(uint32_t id, ModifyType type, int value,
                                  ModifierStackPolicy policy = ModifierStackPolicy::REPLACE)
        {
            return Modifier(id, ModifierSourceType::ATTRIBUTE, value, type, policy);
        }


        /**
         * Builds a single modifier with a double value.
         *
         * @param id The source ID.
         * @param type The modification type.
         * @param value The modifier value.
         * @param policy The stack policy; defaults to REPLACE.
         * @return A Modifier with the given fields and an ATTRIBUTE source.
         */
        Modifier doubleModifier(uint32_t id, ModifyType type, double value,
                                     ModifierStackPolicy policy = ModifierStackPolicy::REPLACE)
        {
            return Modifier(id, ModifierSourceType::ATTRIBUTE, value, type, policy);
        }
    } // namespace


    /**
     * @brief Verifies every ModifierSourceType converts to its expected string.
     */
    TEST(ModifierTest, ModifierSourceTypeToString)
    {
        EXPECT_EQ(modifierSourceTypeToString(ModifierSourceType::ATTRIBUTE), "ATTRIBUTE");
        EXPECT_EQ(modifierSourceTypeToString(ModifierSourceType::ITEM), "ITEM");
        EXPECT_EQ(modifierSourceTypeToString(ModifierSourceType::BUFF), "BUFF");
        EXPECT_EQ(modifierSourceTypeToString(ModifierSourceType::DEBUFF), "DEBUFF");
    }


    /**
     * @brief Verifies string parsing is case-insensitive and unknown strings map to UNKNOWN.
     */
    TEST(ModifierTest, StringToModifierSourceType)
    {
        EXPECT_EQ(stringToModifierSourceType("ATTRIBUTE"), ModifierSourceType::ATTRIBUTE);
        EXPECT_EQ(stringToModifierSourceType("item"), ModifierSourceType::ITEM);
        EXPECT_EQ(stringToModifierSourceType("Buff"), ModifierSourceType::BUFF);
        EXPECT_EQ(stringToModifierSourceType("debuff"), ModifierSourceType::DEBUFF);
        EXPECT_EQ(stringToModifierSourceType("nonsense"), ModifierSourceType::UNKNOWN);
    }


    /**
     * @brief Verifies the int constructor stores an int alternative and defaults to ADD_MAX.
     */
    TEST(ModifierTest, IntConstructor)
    {
        Modifier mod = intModifier(7, ModifyType::ADD_MAX, 5);

        EXPECT_EQ(mod.sourceID, 7u);
        EXPECT_EQ(mod.source, ModifierSourceType::ATTRIBUTE);
        EXPECT_EQ(mod.modifyType, ModifyType::ADD_MAX);
        EXPECT_TRUE(std::holds_alternative<int>(mod.value));
        EXPECT_EQ(std::get<int>(mod.value), 5);
        EXPECT_EQ(mod.getValueAsInt(), 5);
    }


    /**
     * @brief Verifies the double constructor stores a double alternative and defaults to MULTIPLY.
     */
    TEST(ModifierTest, DoubleConstructor)
    {
        Modifier mod = doubleModifier(7, ModifyType::MULTIPLY, 0.25);

        EXPECT_EQ(mod.sourceID, 7u);
        EXPECT_EQ(mod.source, ModifierSourceType::ATTRIBUTE);
        EXPECT_EQ(mod.modifyType, ModifyType::MULTIPLY);
        EXPECT_TRUE(std::holds_alternative<double>(mod.value));
        EXPECT_DOUBLE_EQ(std::get<double>(mod.value), 0.25);
        EXPECT_DOUBLE_EQ(mod.getValueAsDouble(), 0.25);
    }


    /**
     * @brief Verifies the variant constructor accepts a pre-chosen alternative.
     */
    TEST(ModifierTest, VariantConstructor)
    {
        Modifier::Value value = 12;
        Modifier mod(3, ModifierSourceType::ITEM, value, ModifyType::SET);

        EXPECT_EQ(mod.modifyType, ModifyType::SET);
        EXPECT_EQ(mod.getValueAsInt(), 12);
    }


    /**
     * @brief Verifies the typed getters throw when the value holds the other alternative.
     */
    TEST(ModifierTest, TypedGetters_WrongAlternativeThrows)
    {
        Modifier intMod = intModifier(1, ModifyType::SET, 10);
        Modifier doubleMod = doubleModifier(1, ModifyType::MULTIPLY, 0.5);

        EXPECT_THROW(intMod.getValueAsDouble(), error::MIAException);
        EXPECT_THROW(doubleMod.getValueAsInt(), error::MIAException);
    }


    /**
     * @brief Verifies equality compares sourceID, source, and modifyType but not value, so
     * re-adding a modifier with a new value counts as the same effect.
     */
    TEST(ModifierTest, EqualityIgnoresValue)
    {
        Modifier first = intModifier(2, ModifyType::ADD_MAX, 5);
        Modifier second = intModifier(2, ModifyType::ADD_MAX, 50);

        EXPECT_TRUE(first == second)
            << "Modifiers from the same source and type should be equal.";
    }


    /**
     * @brief Verifies modifiers differing in sourceID, source, modifyType, or stackPolicy
     * are not equal. The held alternative is part of the value field, so an ADD_MAX holding
     * an int and one holding a double from the same source still compare equal.
     */
    TEST(ModifierTest, InequalityOnIdentityFields)
    {
        Modifier base = intModifier(2, ModifyType::ADD_MAX, 5);

        EXPECT_FALSE(base == intModifier(3, ModifyType::ADD_MAX, 5));
        EXPECT_FALSE(base == intModifier(2, ModifyType::SET, 5));
        EXPECT_TRUE(base == doubleModifier(2, ModifyType::ADD_MAX, 0.0))
            << "The held alternative is value state, not identity.";
    }


    /**
     * @brief Verifies modifiers with the same identity but different stack policies are
     * not equal.
     */
    TEST(ModifierTest, InequalityOnStackPolicy)
    {
        Modifier replace = intModifier(2, ModifyType::ADD_MAX, 5,
                                            ModifierStackPolicy::REPLACE);
        Modifier keepStrongest = intModifier(2, ModifyType::ADD_MAX, 5,
                                                  ModifierStackPolicy::KEEP_STRONGEST);

        EXPECT_FALSE(replace == keepStrongest)
            << "The stack policy is part of the modifier identity.";
    }


    /**
     * @brief Verifies isStrongerThan compares the value within one modify type.
     */
    TEST(ModifierTest, IsStrongerThan)
    {
        Modifier weakAdd = intModifier(1, ModifyType::ADD_MAX, 5);
        Modifier strongAdd = intModifier(2, ModifyType::ADD_MAX, 10);
        Modifier weakMultiply = doubleModifier(1, ModifyType::MULTIPLY, 0.1);
        Modifier strongMultiply = doubleModifier(2, ModifyType::MULTIPLY, 0.2);

        EXPECT_TRUE(strongAdd.isStrongerThan(weakAdd));
        EXPECT_FALSE(weakAdd.isStrongerThan(strongAdd));
        EXPECT_FALSE(weakAdd.isStrongerThan(weakAdd)) << "Equal values are not strictly stronger.";
        EXPECT_TRUE(strongMultiply.isStrongerThan(weakMultiply));
        EXPECT_FALSE(weakMultiply.isStrongerThan(strongMultiply));
    }


    /**
     * @brief Verifies modifiers of different modify types are never stronger than one another.
     */
    TEST(ModifierTest, IsStrongerThan_DifferentModifyTypes)
    {
        Modifier add = intModifier(1, ModifyType::ADD_MAX, 100);
        Modifier multiply = doubleModifier(2, ModifyType::MULTIPLY, 0.1);

        EXPECT_FALSE(add.isStrongerThan(multiply));
        EXPECT_FALSE(multiply.isStrongerThan(add));
    }


    /**
     * @brief Verifies a REPLACE modifier replaces the attached one from the same source.
     */
    TEST(ModifierTest, AttachModifier_ReplacePolicy)
    {
        std::vector<Modifier> modifiers{intModifier(1, ModifyType::ADD_MAX, 5)};

        attachModifier(modifiers, intModifier(1, ModifyType::ADD_MAX, 50));

        ASSERT_EQ(modifiers.size(), 1);
        EXPECT_EQ(modifiers[0].getValueAsInt(), 50);
    }


    /**
     * @brief Verifies a REPLACE modifier from a different source appends alongside the
     * attached one.
     */
    TEST(ModifierTest, ReplacePolicy_DifferentSourceStacks)
    {
        std::vector<Modifier> modifiers{intModifier(1, ModifyType::ADD_MAX, 5)};

        attachModifier(modifiers, intModifier(2, ModifyType::ADD_MAX, 50));

        EXPECT_EQ(modifiers.size(), 2);
    }


    /**
     * @brief Verifies a STACK modifier appends even when it matches the attached one.
     */
    TEST(ModifierTest, AttachModifier_StackPolicy)
    {
        std::vector<Modifier> modifiers{intModifier(1, ModifyType::ADD_MAX, 5)};

        attachModifier(modifiers, intModifier(1, ModifyType::ADD_MAX, 5,
                                                   ModifierStackPolicy::STACK));

        ASSERT_EQ(modifiers.size(), 2);
        EXPECT_EQ(computeModifiedValue(10, modifiers), 20);
    }


    /**
     * @brief Verifies a KEEP_STRONGEST modifier only displaces an attached modifier of the
     * same kind when it is strictly stronger.
     */
    TEST(ModifierTest, AttachModifier_KeepStrongestPolicy)
    {
        std::vector<Modifier> modifiers{intModifier(1, ModifyType::ADD_MAX, 20)};

        // A weaker modifier from another source does not displace the attached one.
        attachModifier(modifiers, intModifier(2, ModifyType::ADD_MAX, 5,
                                                   ModifierStackPolicy::KEEP_STRONGEST));
        ASSERT_EQ(modifiers.size(), 1);
        EXPECT_EQ(modifiers[0].sourceID, 1u);

        // A stronger modifier from another source takes over.
        attachModifier(modifiers, intModifier(2, ModifyType::ADD_MAX, 50,
                                                   ModifierStackPolicy::KEEP_STRONGEST));
        ASSERT_EQ(modifiers.size(), 1);
        EXPECT_EQ(modifiers[0].sourceID, 2u) << "The winner's source should be kept.";
        EXPECT_EQ(modifiers[0].getValueAsInt(), 50);

        // A modifier from the same source which is not stronger is a no-op.
        attachModifier(modifiers, intModifier(2, ModifyType::ADD_MAX, 30,
                                                   ModifierStackPolicy::KEEP_STRONGEST));
        EXPECT_EQ(modifiers[0].getValueAsInt(), 50);
    }


    /**
     * @brief Verifies a KEEP_STRONGEST modifier attaches normally when no competitor exists.
     */
    TEST(ModifierTest, KeepStrongestPolicy_NoCompetitionAttaches)
    {
        std::vector<Modifier> modifiers{intModifier(1, ModifyType::ADD_MAX, 5)};

        attachModifier(modifiers, doubleModifier(2, ModifyType::MULTIPLY, 0.1,
                                                      ModifierStackPolicy::KEEP_STRONGEST));

        ASSERT_EQ(modifiers.size(), 2) << "A different modify type is not a competitor.";
        EXPECT_EQ(computeModifiedValue(100, modifiers), 115);
    }


    /**
     * @brief Verifies the modifier serialize/deserialize round trip preserves every field.
     */
    TEST(ModifierTest, SerializeRoundTrip)
    {
        std::vector<Modifier> originals{
            intModifier(3, ModifyType::ADD_MAX, 5),
            Modifier(4, ModifierSourceType::ITEM, 0.1, ModifyType::MULTIPLY,
                          ModifierStackPolicy::KEEP_STRONGEST),
            Modifier(5, ModifierSourceType::BUFF, 50, ModifyType::SET,
                          ModifierStackPolicy::STACK)
        };

        for (const auto& original : originals)
        {
            Modifier restored = Modifier::deserialize(original.serialize());
            EXPECT_TRUE(restored == original)
                << "Round trip through serialize/deserialize should preserve " << original.serialize();

            if (original.modifyType == ModifyType::MULTIPLY)
                EXPECT_DOUBLE_EQ(restored.getValueAsDouble(), original.getValueAsDouble());
            else
                EXPECT_EQ(restored.getValueAsInt(), original.getValueAsInt());
        }
    }


    /**
     * @brief Verifies a MULTIPLY value round trips without precision loss.
     * A default-precision stream formats 0.123456789012345 as 0.123457, which would
     * deserialize to a different value.
     */
    TEST(ModifierTest, SerializeRoundTrip_FullDoublePrecision)
    {
        Modifier original = doubleModifier(1, ModifyType::MULTIPLY, 0.123456789012345);

        Modifier restored = Modifier::deserialize(original.serialize());

        EXPECT_EQ(restored.getValueAsDouble(), original.getValueAsDouble())
            << "A MULTIPLY value should round trip bit-exactly.";
    }


    /**
     * @brief Verifies a non-numeric id or value token is rejected with a MIAException.
     */
    TEST(ModifierTest, Deserialize_NonNumericTokenThrows)
    {
        EXPECT_THROW(Modifier::deserialize("not_an_id:BUFF:5:ADD_MAX:STACK"),
                     error::MIAException);
        EXPECT_THROW(Modifier::deserialize("1:BUFF:not_a_number:ADD_MAX:STACK"),
                     error::MIAException);
    }


    /**
     * @brief Verifies the stream operator output format.
     */
    TEST(ModifierTest, StreamOperator)
    {
        Modifier intMod = intModifier(3, ModifyType::ADD_MAX, 5);
        std::ostringstream oss;
        oss << intMod;
        EXPECT_EQ(oss.str(),
                  "Modifier{sourceID=3, source=ATTRIBUTE, value=5, type=ADD_MAX, policy=REPLACE}");
    }


    /**
     * @brief Verifies a base value with no modifiers passes through unchanged.
     */
    TEST(ModifierTest, Compute_NoModifiers)
    {
        EXPECT_EQ(computeModifiedValue(42, {}), 42);
    }


    /**
     * @brief Verifies a single MULTIPLY scales the base by (1 + bonus).
     */
    TEST(ModifierTest, Compute_SingleMultiply)
    {
        std::vector<Modifier> mods{doubleModifier(1, ModifyType::MULTIPLY, 0.1)};
        EXPECT_EQ(computeModifiedValue(100, mods), 110);
    }

    /**
     * @brief Verifies multiple multipliers compound multiplicatively.
     */
    TEST(ModifierTest, Compute_MultipliersCompound)
    {
        std::vector<Modifier> mods{doubleModifier(1, ModifyType::MULTIPLY, 0.1),
                                        doubleModifier(2, ModifyType::MULTIPLY, 0.1)};
        EXPECT_EQ(computeModifiedValue(100, mods), 121);
    }


    /**
     * @brief Verifies additive modifiers sum onto the base.
     */
    TEST(ModifierTest, Compute_AdditivesSum)
    {
        std::vector<Modifier> mods{intModifier(1, ModifyType::ADD_MAX, 5),
                                        intModifier(2, ModifyType::ADD_MAX, 7)};
        EXPECT_EQ(computeModifiedValue(10, mods), 22);
    }


    /**
     * @brief Verifies the phase ordering: additions are not amplified by multipliers.
     * 100 scaled by +10% is 110, plus 10 is 120; adding first would give (100 + 10) * 1.1 = 121.
     */
    TEST(ModifierTest, Compute_MultiplyAppliesBeforeAdd)
    {
        std::vector<Modifier> mods{doubleModifier(1, ModifyType::MULTIPLY, 0.1),
                                        intModifier(2, ModifyType::ADD_MAX, 10)};
        EXPECT_EQ(computeModifiedValue(100, mods), 120);
    }


    /**
     * @brief Verifies a SET modifier overrides the multiplied and added value.
     */
    TEST(ModifierTest, Compute_SetOverrides)
    {
        std::vector<Modifier> mods{doubleModifier(1, ModifyType::MULTIPLY, 0.5),
                                        intModifier(2, ModifyType::ADD_MAX, 25),
                                        intModifier(3, ModifyType::SET, 9)};
        EXPECT_EQ(computeModifiedValue(100, mods), 9);
    }


    /**
     * @brief Verifies that when several SET modifiers are attached, the last one wins.
     */
    TEST(ModifierTest, Compute_LastSetWins)
    {
        std::vector<Modifier> mods{intModifier(1, ModifyType::SET, 50),
                                        intModifier(2, ModifyType::SET, 70)};
        EXPECT_EQ(computeModifiedValue(100, mods), 70);
    }


    /**
     * @brief Verifies negative additive modifiers subtract from the base.
     */
    TEST(ModifierTest, Compute_NegativeAdditive)
    {
        std::vector<Modifier> mods{intModifier(1, ModifyType::ADD_MAX, -20)};
        EXPECT_EQ(computeModifiedValue(10, mods), -10);
    }


    /**
     * @brief Verifies fractional results are rounded to the nearest int.
     */
    TEST(ModifierTest, Compute_RoundsFractionalResult)
    {
        std::vector<Modifier> mods{doubleModifier(1, ModifyType::MULTIPLY, 0.1)};
        EXPECT_EQ(computeModifiedValue(25, mods), 28);
    }


    /**
     * @brief Verifies results clamp into the range of int instead of overflowing.
     */
    TEST(ModifierTest, Compute_ClampsToIntRange)
    {
        const int maxInt = std::numeric_limits<int>::max();
        const int minInt = std::numeric_limits<int>::min();

        std::vector<Modifier> add{intModifier(1, ModifyType::ADD_MAX, 100)};
        EXPECT_EQ(computeModifiedValue(maxInt, add), maxInt);

        std::vector<Modifier> subtract{intModifier(1, ModifyType::ADD_MAX, -100)};
        EXPECT_EQ(computeModifiedValue(minInt, subtract), minInt);

        std::vector<Modifier> scale{doubleModifier(1, ModifyType::MULTIPLY, 2.0)};
        EXPECT_EQ(computeModifiedValue(2000000000, scale), maxInt);
    }
} // namespace rpg
