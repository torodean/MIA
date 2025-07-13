/**
 * @file Database_T.cpp
 * @author Antonius Torode
 * @date 07/05/2025
 * Description: Unit tests for the Database class.
 */

#include <gtest/gtest.h>

// Include the associated headers we are testing.
#include "Database.hpp"

class Database_T : public ::testing::Test {
protected:
    // Runs once before any test in this test suite
    static void SetUpTestSuite() 
    {
        // Credentials matching the database from the setup.sh script.
        credentials = DatabaseCredentials("unit_test_user", "localhost", 3306, "unit_test_password");
        db.setCredentials(credentials);
        std::string databaseName = "unit_test_db";
        db.setDatabase(databaseName);
        db.connect();
    }

    // Runs once after all tests in this test suite
    static void TearDownTestSuite() 
    {
        // Optional: clean up shared resources
    }

    // Runs before each test
    void SetUp() override 
    {
        // Optional: per-test setup
    }

    // Runs after each test
    void TearDown() override 
    {
        // Optional: per-test teardown
    }

    static Database db;
    static DatabaseCredentials credentials;
};

// Defines linkage to the static class members.
DatabaseCredentials Database_T::credentials;
Database Database_T::db;

TEST_F(Database_T, testAddAndViewTable) 
{
    std::vector<std::string> columns = {"id INT PRIMARY KEY", "name VARCHAR(50)"};
    db.addTable("test_table", columns);

    std::vector<std::string> elementNames = {"id", "name"};
    std::vector<std::string> values1 = {"1", "'Alice'"};
    std::vector<std::string> values2 = {"2", "'Bob'"};
    db.addElementToTable("test_table", elementNames, values1);
    db.addElementToTable("test_table", elementNames, values2);

    std::string tableOutput = db.viewTable("test_table");

    ASSERT_FALSE(tableOutput.empty());
    ASSERT_NE(tableOutput.find("1, Alice"), std::string::npos);
    ASSERT_NE(tableOutput.find("2, Bob"), std::string::npos);
}

TEST_F(Database_T, testDeleteTable) 
{
    db.deleteTable("test_table");

    std::string tableOutput = db.viewTable("test_table");

    ASSERT_TRUE(tableOutput.empty());
}

// Test accessing data from the setup.sh script.
TEST_F(Database_T, ViewUsersTable) 
{
    std::string tableContents = db.viewTable("users");
    // Basic check that the output contains known inserted user names
    EXPECT_NE(tableContents.find("alice"), std::string::npos);
    EXPECT_NE(tableContents.find("bob"), std::string::npos);
    EXPECT_NE(tableContents.find("charlie"), std::string::npos);
}

// Test accessing data from the setup.sh script.
TEST_F(Database_T, AddAndViewTable) 
{
    std::vector<std::string> columns = {"id INT PRIMARY KEY", "name VARCHAR(50)"};
    db.addTable("test_table", columns);

    std::vector<std::string> elements = {"id", "name"};
    std::vector<std::string> values = {"1", "'testname'"};
    db.addElementToTable("test_table", elements, values);

    std::string output = db.viewTable("test_table");
    EXPECT_NE(output.find("testname"), std::string::npos);
}

// Test accessing data from the setup.sh script.
TEST_F(Database_T, DeleteTable) 
{
    db.deleteTable("test_table");
    std::string output = db.viewTable("test_table");
    // Expect output to be empty or indicate error (adjust based on your implementation)
    EXPECT_TRUE(output.empty() || output.find("ERROR") != std::string::npos);
}

