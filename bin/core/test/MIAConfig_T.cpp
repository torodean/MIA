/**
 * @file MIAConfig_T.cpp
 * @author Antonius Torode
 * @date 6/15/25
 * @brief Unit tests for MIAConfig class using Google Test.
 */


#include <fstream>
#include <cstdio>
#include <filesystem>

#include <gtest/gtest.h>

#include "MIAConfig.hpp"
#include "Constants.hpp"
#include "MIAException.hpp"

using namespace constants;

namespace config
{
    namespace
    {
        // Utility to create a temporary key-value config file for testing.
        std::string createTempConfigFile(const std::string& content) 
        {
            std::string tempFileName = "test_config.cfg";
            std::ofstream out(tempFileName);
            out << content;
            out.close();

            // Return absolute path using std::filesystem
            return std::filesystem::absolute(tempFileName).string();
        }


        // Clean up temporary config file.
        void removeTempConfigFile(const std::string& path) 
        {
            std::remove(path.c_str());
        }
    } // namespace


    class MIAConfig_T : public ::testing::Test 
    {
    protected:
        std::string configContent = 
            "# port2=9999\n"
            "\n"
            "ab\n"
            "port=8080\n"
            "host=localhost\n"
            "pi=3.14\n"
            "debug=true\n"
            "falseFlag=false\n"
            "zeroFlag=0\n"
            "noFlag=no\n"
            "offFlag=off\n"
            "upper=TRUE\n"
            "junkBool=guacamole\n"
            "ids=1,2,3\n"
            "names=alpha,beta,gamma\n"
            "integers=1,2,3,4,42\n"
            "brokenInts=1,2,3,4,42,guacamole\n";

        std::string filePath;

        void SetUp() override 
        {
            filePath = createTempConfigFile(configContent);
        }

        void TearDown() override 
        {
            removeTempConfigFile(filePath);
        }
    };


    /**
     * @brief Verifies that MIAConfig loads a key-value configuration file and correctly
     *        retrieves values using the typed getter methods.
     *
     * After initialization, the test checks retrieval of integer, string, double,
     * boolean, and vector values from the configuration map and confirms that the
     * stored configuration filename matches the provided path.
     */
    TEST_F(MIAConfig_T, LoadAndRetrieveValues) 
    {
        MIAConfig config(filePath, ConfigType::KEY_VALUE);
        config.initialize();

        EXPECT_EQ(config.getConfigFileName(), filePath);
        EXPECT_EQ(config.getInt("port"), 8080);
        EXPECT_EQ(config.getString("host"), "localhost");
        EXPECT_DOUBLE_EQ(config.getDouble("pi"), 3.14);
        EXPECT_TRUE(config.getBool("debug"));

        std::vector<int> expectedIds = {1, 2, 3};
        EXPECT_EQ(config.getIntVector("ids", ','), expectedIds);

        std::vector<std::string> expectedNames = {"alpha", "beta", "gamma"};
        EXPECT_EQ(config.getVector("names", ','), expectedNames);
    }


    /**
     * @brief Verifies that the configuration type is preserved and that reload()
     *        re-parses the configuration file successfully.
     *
     * The test confirms that getConfigType() returns the expected configuration
     * format and that calling reload() maintains correct access to configuration
     * values.
     */
    TEST_F(MIAConfig_T, ReloadAndGetType) 
    {
        MIAConfig config(filePath, ConfigType::KEY_VALUE);
        config.initialize();

        EXPECT_EQ(config.getConfigType(), ConfigType::KEY_VALUE);

        config.reload(); // Should work identically to initialize()
        EXPECT_EQ(config.getString("host"), "localhost");
    }


    /**
     * @brief Verifies that configuration entries can be retrieved as a collection
     *        and serialized using dumpConfigMap().
     *
     * The test ensures that getAllConfigPairs() returns a non-empty container and
     * that dumpConfigMap() writes key-value entries to the provided output stream.
     */
    TEST_F(MIAConfig_T, GetAllPairsAndDump) 
    {
        MIAConfig config(filePath, ConfigType::KEY_VALUE);
        config.initialize();

        auto pairs = config.getAllConfigPairs();
        EXPECT_FALSE(pairs.empty());

        std::ostringstream oss;
        config.dumpConfigMap(oss);
        std::string output = oss.str();
        EXPECT_NE(output.find("port=8080"), std::string::npos);
    }


    /**
     * @brief Verifies that setConfigFileName() updates the configuration source and
     *        initializes the configuration data.
     *
     * The test constructs a default MIAConfig instance, sets the configuration file
     * name and type, and confirms that configuration values can be retrieved
     * immediately afterward.
     */
    TEST_F(MIAConfig_T, SetConfigFileNameReinitializes) 
    {
        MIAConfig config;
        config.setConfigFileName(filePath, ConfigType::KEY_VALUE);
        EXPECT_EQ(config.getInt("port"), 8080);
    }


    /**
     * @brief Verifies that accessing undefined or broken configuration keys throws
     *        MIAException.
     *
     * The test attempts to retrieve integer, vector, and boolean values for keys
     * that do not exist in the configuration and confirms that the expected
     * exception type is thrown. It also tries to parse invalid vectors and ensures
     * the failure occurs.
     */
    TEST_F(MIAConfig_T, ExceptionThrownForMissingKey) 
    {
        MIAConfig config(filePath, ConfigType::KEY_VALUE);
        config.initialize();
        std::vector<std::string> emptyVector;
        EXPECT_THROW(config.getInt("nonexistent"), error::MIAException);
        EXPECT_EQ(config.getVector("missing", ','), emptyVector);
        EXPECT_THROW(config.getBool("undefined"), error::MIAException);

        std::vector<int> integers = {1,2,3,4,42};
        EXPECT_EQ(config.getIntVector("integers", ','), integers);
        EXPECT_THROW(config.getIntVector("brokenInts", ','), error::MIAException);
    }


    /**
     * @brief Tests whether the optional values correctly pass and return default values.
     */
     TEST_F(MIAConfig_T, OptionalValues)
     {
        MIAConfig config(filePath, ConfigType::KEY_VALUE);
        config.initialize();

        EXPECT_EQ(config.getInt("nonexistent", true, 14), 14);
        EXPECT_EQ(config.getInt("port", true, 14), 8080);

        EXPECT_EQ(config.getBool("nonexistent", true, false), false);
        EXPECT_EQ(config.getBool("debug", true, false), true);

        EXPECT_EQ(config.getString("nonexistent", true, "potato"), "potato");
        EXPECT_EQ(config.getString("host", true, "potato"), "localhost");

        EXPECT_EQ(config.getDouble("nonexistent", true, 14.823), 14.823);
        EXPECT_EQ(config.getDouble("pi", true, 14.823), 3.14);
    }


    /**
     * @brief Verifies that boolean values are parsed from all supported true and false
     *        tokens and that an unrecognized boolean value throws.
     */
    TEST_F(MIAConfig_T, BoolValueParsing) 
    {
        MIAConfig config(filePath, ConfigType::KEY_VALUE);
        config.initialize();

        EXPECT_TRUE(config.getBool("debug"));
        EXPECT_TRUE(config.getBool("upper"));
        EXPECT_FALSE(config.getBool("falseFlag"));
        EXPECT_FALSE(config.getBool("zeroFlag"));
        EXPECT_FALSE(config.getBool("noFlag"));
        EXPECT_FALSE(config.getBool("offFlag"));
        EXPECT_THROW(config.getBool("junkBool"), error::MIAException);
    }


    /**
     * @brief Verifies that comment, empty, and too-short configuration lines are skipped
     *        during loading and never stored as configuration entries.
     */
    TEST_F(MIAConfig_T, CommentAndShortLinesSkipped) 
    {
        MIAConfig config(filePath, ConfigType::KEY_VALUE);
        config.initialize();

        EXPECT_THROW(config.getInt("port2"), error::MIAException);
        EXPECT_THROW(config.getString("ab"), error::MIAException);
        EXPECT_EQ(config.getInt("port"), 8080);
    }
} // namespace config
