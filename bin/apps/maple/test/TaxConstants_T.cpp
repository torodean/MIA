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
// Used for finding test data
#include "Paths.hpp"

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
            
            testDataFolder = paths::getCppFileDirAtCompileTime(__FILE__) + "/test_files";
        }
        
        void SetUp() override
        {
            // Construct and load the config from a test file.
            std::string configFile = testDataFolder + "/taxConstants.MIA";
            
            ASSERT_TRUE(std::filesystem::exists(configFile))
                << "Cannot find test config file at: " << configFile << std::endl;
            config.setConfigFileName(configFile, constants::ConfigType::KEY_VALUE);
            config.initialize();
        }
        
        /**
         * A custom constants object with simplified values for testing.
         * These values are set in the constructor.
         */
        TaxRateConstants constants;
        
        /// The folder location for the test data used by this file.
        std::string testDataFolder;
        
        /// A config object for parsing a test config file.
        config::MIAConfig config;
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
        // Construct the expected bracket vectors.
        double inf = std::numeric_limits<double>::infinity();
        const std::vector<double> expectedSingle{0.0, 1000.0, inf};
        const std::vector<double> expectedMarried{0.0, 2000.0, inf};
        const std::vector<double> expectedRates{0.00, 0.10, 0.50};
        
        // Create the constants and compare to what matches the file values.
        constants = createTaxRateConstantsFromConfig(config);
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
    
    
    /**
     * @brief This will construct a TaxRateConstants from a config object (which is
     * constructed from a test file), then ensure that all of the values are set as expected.
     * This uses a custom suffix on the tax values in the config file and ensures the method
     * still parses correctly when a suffix is supplied.
     */
    TEST_F(TaxConstants_T, ConstructingTaxConstantsFromConfigFileWithSuffix)
    {        
        // Construct the expected bracket vectors.
        double inf = std::numeric_limits<double>::infinity();
        const std::vector<double> expectedSingle{0.0, 2000.0, inf};
        const std::vector<double> expectedMarried{0.0, 4000.0, inf};
        const std::vector<double> expectedRates{0.00, 0.20, 0.70};
        
        // Create the constants and compare to what matches the file values.
        constants = createTaxRateConstantsFromConfig(config, "_custom");
        ASSERT_EQ(constants.medicareTaxRate, 0.035);
        ASSERT_EQ(constants.oasdiTaxRate, 0.025);
        ASSERT_EQ(constants.salesTax, 0.015);
        ASSERT_EQ(constants.taxBracketSingle, expectedSingle);
        ASSERT_EQ(constants.taxBracketMarried, expectedMarried);
        ASSERT_EQ(constants.taxRateBracket, expectedRates);
        ASSERT_EQ(constants.standardDeductibleSingle, 20000.0);
        ASSERT_EQ(constants.standardDeductibleHeadOfHousehold, 25000.0);
        ASSERT_EQ(constants.standardDeductibleMarried, 30000.0);
    }
} // namespace maple
