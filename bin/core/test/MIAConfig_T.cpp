/**
 * @file MIAConfig_T.cpp
 * @author Antonius Torode
 * @date 6/15/25
 * Description: Unit tests for MIAConfig class using Google Test.
 */

#include <gtest/gtest.h>
#include "MIAConfig.hpp"
#include "Constants.hpp"
#include "MIAException.hpp"

#include <fstream>
#include <cstdio>
#include <filesystem>

using namespace config;
using namespace constants;


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


class MIAConfigTest : public ::testing::Test 
{
protected:
    std::string configContent = 
        "port=8080\n"
        "host=localhost\n"
        "pi=3.14\n"
        "debug=true\n"
        "ids=1,2,3\n"
        "names=alpha,beta,gamma\n";

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
 * @test MIAConfigTest.LoadAndRetrieveValues
 * @brief Verifies that MIAConfig loads a key-value configuration file and correctly
 *        retrieves values using the typed getter methods.
 *
 * After initialization, the test checks retrieval of integer, string, double,
 * boolean, and vector values from the configuration map and confirms that the
 * stored configuration filename matches the provided path.
 */
TEST_F(MIAConfigTest, LoadAndRetrieveValues) 
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
 * @test MIAConfigTest.ReloadAndGetType
 * @brief Verifies that the configuration type is preserved and that reload()
 *        re-parses the configuration file successfully.
 *
 * The test confirms that getConfigType() returns the expected configuration
 * format and that calling reload() maintains correct access to configuration
 * values.
 */
TEST_F(MIAConfigTest, ReloadAndGetType) 
{
    MIAConfig config(filePath, ConfigType::KEY_VALUE);
    config.initialize();

    EXPECT_EQ(config.getConfigType(), ConfigType::KEY_VALUE);

    config.reload(); // Should work identically to initialize()
    EXPECT_EQ(config.getString("host"), "localhost");
}

/**
 * @test MIAConfigTest.GetAllPairsAndDump
 * @brief Verifies that configuration entries can be retrieved as a collection
 *        and serialized using dumpConfigMap().
 *
 * The test ensures that getAllConfigPairs() returns a non-empty container and
 * that dumpConfigMap() writes key-value entries to the provided output stream.
 */
TEST_F(MIAConfigTest, GetAllPairsAndDump) 
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
 * @test MIAConfigTest.SetConfigFileNameReinitializes
 * @brief Verifies that setConfigFileName() updates the configuration source and
 *        initializes the configuration data.
 *
 * The test constructs a default MIAConfig instance, sets the configuration file
 * name and type, and confirms that configuration values can be retrieved
 * immediately afterward.
 */
TEST_F(MIAConfigTest, SetConfigFileNameReinitializes) 
{
    MIAConfig config;
    config.setConfigFileName(filePath, ConfigType::KEY_VALUE);
    EXPECT_EQ(config.getInt("port"), 8080);
}

/**
 * @test MIAConfigTest.ExceptionThrownForMissingKey
 * @brief Verifies that accessing undefined configuration keys throws MIAException.
 *
 * The test attempts to retrieve integer, vector, and boolean values for keys
 * that do not exist in the configuration and confirms that the expected
 * exception type is thrown.
 */
TEST_F(MIAConfigTest, ExceptionThrownForMissingKey) 
{
    MIAConfig config(filePath, ConfigType::KEY_VALUE);
    config.initialize();
    EXPECT_THROW(config.getInt("nonexistent"), error::MIAException);
    EXPECT_THROW(config.getVector("missing", ','), error::MIAException);
    EXPECT_THROW(config.getBool("undefined"), error::MIAException);
}

