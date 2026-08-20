/**
 * @file TaxConstants_T.cpp
 * @author Antonius Torode
 * @date 08/20/2026
 * @brief Tests for the TaxConstants storage class and associated methods.
 */
 
#include <string>
#include <filesystem>
#include <limits>
 
#include <gtest/gtest.h>

// Associated header to test.
#include "TaxConstants.hpp"
// Used for the configuration storage.
#include "MIAConfig.hpp"
// Used for the TaxBrackets type returned by the conversion helper.
#include "FinanceUtils.hpp"
// Needed for config types.
#include "Constants.hpp"

namespace maple
{
    /**
     * This is a class which creates a custom TaxRateConstants object used for testing.
     *
     *
     */
    class TaxConstants_T : public ::testing::Test
    {
    protected:
        /**
         * Constructor which sets custom values for the internal TaxRateConstants.
         */
        TaxConstants_T()
        {
            constants.taxBracketSingle = {0.0, 1000.0, 11000.0};
            constants.taxBracketMarried = {0.0, 2000.0, 22000.0};
            constants.taxRateBracket = {0.0, 0.10, 0.50};
            
            std::string thisFilesPath = __FILE__; // Full path of this file at compile time.
            std::size_t pos = thisFilesPath.find_last_of('/');
            testDataFolder = (pos == std::string::npos) ? "" : thisFilesPath.substr(0, pos);
        }
        
        /**
         * A custom constants object with simplified values for testing.
         * These values are set in the constructor.
         */
        TaxRateConstants constants;
        
        /// The folder location for the test data used by this file.
        std::string testDataFolder;
    }; // class TaxConstants_T


    /**
     * @brief This will test if the toTaxBrackets method correctly constructs a 
     * TaxBrackets object for a single filing status.
     */
    TEST_F(TaxConstants_T, TaxBracketsCorrectlyProducedForSingle)
    {
        math::finance::TaxBrackets brackets = toTaxBrackets(constants, FilingStatus::Single);
        ASSERT_EQ(brackets.size(), 3)
            << "There should be three elements in the constructed tax brackets.";
        ASSERT_EQ(brackets[0].rate, 0.0);
        ASSERT_EQ(brackets[0].lowerBound, 0.0);
        ASSERT_EQ(brackets[1].rate, 0.10);
        ASSERT_EQ(brackets[1].lowerBound, 1000.0);
        ASSERT_EQ(brackets[2].rate, 0.50);
        ASSERT_EQ(brackets[2].lowerBound, 11000.0);
    }
    
    
    /**
     * @brief This will test if the toTaxBrackets method correctly constructs a 
     * TaxBrackets object for a married filing status.
     */
    TEST_F(TaxConstants_T, TaxBracketsCorrectlyProducedForMarried)
    {
        math::finance::TaxBrackets brackets = toTaxBrackets(constants, FilingStatus::Married);
        ASSERT_EQ(brackets.size(), 3)
            << "There should be three elements in the constructed tax brackets.";
        ASSERT_EQ(brackets[0].rate, 0.0);
        ASSERT_EQ(brackets[0].lowerBound, 0.0);
        ASSERT_EQ(brackets[1].rate, 0.10);
        ASSERT_EQ(brackets[1].lowerBound, 2000.0);
        ASSERT_EQ(brackets[2].rate, 0.50);
        ASSERT_EQ(brackets[2].lowerBound, 22000.0);
    }
    
    
    /**
     * @brief This will construct a TaxRateConstants from a config object (which is
     * constructed from a test file), then ensure that all of the values are set as expected.
     */
    TEST_F(TaxConstants_T, ConstructingTaxConstantsFromConfigFile)
    {
        // Construct and load the config from a test file.
        std::string configFile = testDataFolder + "/test_files/taxConstants.MIA";
        ASSERT_TRUE(std::filesystem::exists(configFile));
        config::MIAConfig config(configFile, constants::ConfigType::KEY_VALUE);
        config.initialize();
        
        // Construct the expected bracket vectors.
        double inf = std::numeric_limits<double>::infinity();
        const std::vector<double> expectedSingle{0.0, 1000.0, inf};
        const std::vector<double> expectedMarried{0.0, 2000.0, inf};
        const std::vector<double> expectedRates{0.00, 0.10, 0.50};
        
        // Create the constants and compare to what matches the file values.
        TaxRateConstants constants = createTaxRateConstantsFromConfig(config);
        ASSERT_EQ(constants.medicareTaxRate, 0.075);
        ASSERT_EQ(constants.oasdiTaxRate, 0.095);
        ASSERT_EQ(constants.salesTax, 0.085);
        ASSERT_EQ(constants.taxBracketSingle, expectedSingle);
        ASSERT_EQ(constants.taxBracketMarried, expectedMarried);
        ASSERT_EQ(constants.taxRateBracket, expectedRates);
        ASSERT_EQ(constants.standardDeductibleSingle, 10000.0);
        ASSERT_EQ(constants.standardDeductibleHeadOfHousehold, 15000.0);
        ASSERT_EQ(constants.standardDeductibleMarried, 20000.0);
    }
} // namespace maple
