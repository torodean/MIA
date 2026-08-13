/**
 * @file Currency_T.cpp
 * @author Antonius Torode
 * @date 07/09/2025
 * Description: Google Test suite for the currency::Currency class and related functions.
 */

#include <gtest/gtest.h>

#include "Currency.hpp"

namespace currency
{
    /**
     * @brief Verifies stringToCurrencyType maps each valid currency name to its enum value, including mixed-case input.
     */
    TEST(CurrencyTypeConversionTest, StringToCurrencyTypeValid)
    {
        EXPECT_EQ(stringToCurrencyType("COIN"), CurrencyType::COIN);
        EXPECT_EQ(stringToCurrencyType("fiat"), CurrencyType::FIAT);
        EXPECT_EQ(stringToCurrencyType("Token"), CurrencyType::TOKEN);
        EXPECT_EQ(stringToCurrencyType("EvEnT"), CurrencyType::EVENT);
        EXPECT_EQ(stringToCurrencyType("gem"), CurrencyType::GEM);
    }
    
    /**
     * @brief Verifies stringToCurrencyType resolves unrecognized, empty, and numeric strings to CurrencyType::UNKNOWN.
     */
    TEST(CurrencyTypeConversionTest, StringToCurrencyTypeInvalid)
    {
        EXPECT_EQ(stringToCurrencyType("INVALID"), CurrencyType::UNKNOWN);
        EXPECT_EQ(stringToCurrencyType(""), CurrencyType::UNKNOWN);
        EXPECT_EQ(stringToCurrencyType("123"), CurrencyType::UNKNOWN);
        EXPECT_EQ(stringToCurrencyType("UnknOWn"), CurrencyType::UNKNOWN);
    }
    
    /**
     * @brief Verifies currencyTypeToString serializes each CurrencyType enum value to its uppercase string name.
     */
    TEST(CurrencyTypeConversionTest, CurrencyTypeToString)
    {
        EXPECT_EQ(currencyTypeToString(CurrencyType::COIN), "COIN");
        EXPECT_EQ(currencyTypeToString(CurrencyType::FIAT), "FIAT");
        EXPECT_EQ(currencyTypeToString(CurrencyType::TOKEN), "TOKEN");
        EXPECT_EQ(currencyTypeToString(CurrencyType::EVENT), "EVENT");
        EXPECT_EQ(currencyTypeToString(CurrencyType::GEM), "GEM");
        EXPECT_EQ(currencyTypeToString(CurrencyType::UNKNOWN), "UNKNOWN");
    }
    
    /**
     * Test fixture for the Currency class. 
     */
    class Currency_T : public ::testing::Test
    {
    protected:
        /// Sample Currency used across the test fixture.
        Currency testCurrency{
            42,
            "Gold",
            "Standard gold currency",
            CurrencyType::COIN,
            true,
            "gold_icon.png"
        };
    };
    
    /**
     * @brief Verifies the parameterized constructor stores all fields and the getters return them unchanged.
     */
    TEST_F(Currency_T, ConstructorAndGetters)
    {
        EXPECT_EQ(testCurrency.getID(), 42u);
        EXPECT_EQ(testCurrency.getName(), "Gold");
        EXPECT_EQ(testCurrency.getDescription(), "Standard gold currency");
        EXPECT_EQ(testCurrency.getCurrencyType(), CurrencyType::COIN);
        EXPECT_TRUE(testCurrency.isTradeable());
        EXPECT_EQ(testCurrency.getIconArt(), "gold_icon.png");
    }
    
    /**
     * @brief Verifies toJson serializes the Currency into the expected field set and values.
     */
    TEST_F(Currency_T, ToJsonProducesCorrectJson)
    {
        nlohmann::json expected = {
            {"id", 42},
            {"name", "Gold"},
            {"description", "Standard gold currency"},
            {"type", "COIN"},
            {"tradeable", true},
            {"icon", "gold_icon.png"}
        };
        EXPECT_EQ(testCurrency.toJson(), expected);
    }
} // namespace currency
