/**
 * @file: MIAConfig_T.cpp
 * @author: Antonius Torode
 * Created on: 6/15/25
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

TEST_F(MIAConfigTest, ReloadAndGetType) 
{
    MIAConfig config(filePath, ConfigType::KEY_VALUE);
    config.initialize();

    EXPECT_EQ(config.getConfigType(), ConfigType::KEY_VALUE);

    config.reload(); // Should work identically to initialize()
    EXPECT_EQ(config.getString("host"), "localhost");
}

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

TEST_F(MIAConfigTest, SetConfigFileNameReinitializes) 
{
    MIAConfig config;
    config.setConfigFileName(filePath, ConfigType::KEY_VALUE);
    EXPECT_EQ(config.getInt("port"), 8080);
}

TEST_F(MIAConfigTest, ExceptionThrownForMissingKey) 
{
    MIAConfig config(filePath, ConfigType::KEY_VALUE);
    config.initialize();
    EXPECT_THROW(config.getInt("nonexistent"), error::MIAException);
    EXPECT_THROW(config.getVector("missing", ','), error::MIAException);
    EXPECT_THROW(config.getBool("undefined"), error::MIAException);
}

