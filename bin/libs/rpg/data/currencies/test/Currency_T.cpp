/**
 * File: Currency_T.cpp
 * Author: Antonius Torode
 * Created on: 07/09/2025
 * Description: Google Test suite for the currency::Currency class and related functions.
 */

#include "Currency.hpp"
#include <gtest/gtest.h>

using namespace currency;

/**
 * Tests for currency type conversion functions.
 */
TEST(CurrencyTypeConversionTest, StringToCurrencyTypeValid)
{
    EXPECT_EQ(stringToCurrencyType("COIN"), CurrencyType::COIN);
    EXPECT_EQ(stringToCurrencyType("fiat"), CurrencyType::FIAT);
    EXPECT_EQ(stringToCurrencyType("Token"), CurrencyType::TOKEN);
    EXPECT_EQ(stringToCurrencyType("EVENT"), CurrencyType::EVENT);
    EXPECT_EQ(stringToCurrencyType("gem"), CurrencyType::GEM);
}

TEST(CurrencyTypeConversionTest, StringToCurrencyTypeInvalid)
{
    EXPECT_EQ(stringToCurrencyType("INVALID"), CurrencyType::UNKNOWN);
    EXPECT_EQ(stringToCurrencyType(""), CurrencyType::UNKNOWN);
    EXPECT_EQ(stringToCurrencyType("123"), CurrencyType::UNKNOWN);
}

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
 * Tests for the Currency class.
 */
class Currency_T : public ::testing::Test
{
protected:
    Currency testCurrency{
        42,
        "Gold",
        "Standard gold currency",
        CurrencyType::COIN,
        true,
        "gold_icon.png"
    };
};

TEST_F(Currency_T, ConstructorAndGetters)
{
    EXPECT_EQ(testCurrency.getID(), 42u);
    EXPECT_EQ(testCurrency.getName(), "Gold");
    EXPECT_EQ(testCurrency.getDescription(), "Standard gold currency");
    EXPECT_EQ(testCurrency.getCurrencyType(), CurrencyType::COIN);
    EXPECT_TRUE(testCurrency.isTradeable());
    EXPECT_EQ(testCurrency.getIconArt(), "gold_icon.png");
}

TEST_F(Currency_T, ToJsonProducesCorrectJson)
{
    nlohmann::json expected = {
        {"id", 42},
        {"name", "Gold"},
        {"description", "Standard gold currency"},
        {"type", "COIN"},
        {"tradeable", true}
    };
    EXPECT_EQ(testCurrency.toJson(), expected);
}

