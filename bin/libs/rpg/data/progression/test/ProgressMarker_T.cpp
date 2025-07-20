/**
 * @file ProgressMarker_T.cpp
 * @author Antonius Torode
 * @date 07/20/2025
 * @brief Unit tests for the ProgressMarker class using Google Test.
 */

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "ProgressMarker.hpp"

namespace progress
{

    // Test fixture for ProgressMarker tests.
    class ProgressMarkerTest : public ::testing::Test
    {
    protected:
        // Common setup for tests.
        void SetUp() override
        {
            // Initialize a sample ProgressMarker for tests.
            marker = ProgressMarker(1, "TestProgress", "This is a test progress marker.");
        }

        ProgressMarker marker;
    };

    // Test the constructor and inherited getters.
    TEST_F(ProgressMarkerTest, ConstructorAndGetters)
    {
        EXPECT_EQ(marker.getID(), 1);
        EXPECT_EQ(marker.getName(), "TestProgress");
        EXPECT_EQ(marker.getDescription(), "This is a test progress marker.");
    }

    // Test the default constructor.
    TEST_F(ProgressMarkerTest, DefaultConstructor)
    {
        ProgressMarker defaultMarker;
        EXPECT_EQ(defaultMarker.getID(), 0);
        EXPECT_EQ(defaultMarker.getName(), "");
        EXPECT_EQ(defaultMarker.getDescription(), "");
    }

    // Test the toJson method.
    TEST_F(ProgressMarkerTest, ToJson)
    {
        nlohmann::json json = marker.toJson();
        EXPECT_EQ(json["id"], 1);
        EXPECT_EQ(json["name"], "TestProgress");
        EXPECT_EQ(json["description"], "This is a test progress marker.");
    }

    // Test the fromJson method.
    TEST_F(ProgressMarkerTest, FromJson)
    {
        nlohmann::json json = {
            {"id", 2},
            {"name", "AnotherProgress"},
            {"description", "Another test progress marker."}
        };
        ProgressMarker newMarker = ProgressMarker::fromJson(json);
        EXPECT_EQ(newMarker.getID(), 2);
        EXPECT_EQ(newMarker.getName(), "AnotherProgress");
        EXPECT_EQ(newMarker.getDescription(), "Another test progress marker.");
    }

    // Test fromJson with missing description (should use default empty string).
    TEST_F(ProgressMarkerTest, FromJsonMissingDescription)
    {
        nlohmann::json json = {
            {"id", 3},
            {"name", "NoDescriptionProgress"}
        };
        ProgressMarker newMarker = ProgressMarker::fromJson(json);
        EXPECT_EQ(newMarker.getID(), 3);
        EXPECT_EQ(newMarker.getName(), "NoDescriptionProgress");
        EXPECT_EQ(newMarker.getDescription(), "");
    }

    // Test fromJson with invalid JSON (missing required fields).
    TEST_F(ProgressMarkerTest, FromJsonInvalid)
    {
        nlohmann::json json = {
            {"description", "Invalid progress marker."}
        };
        EXPECT_THROW(ProgressMarker::fromJson(json), nlohmann::json::out_of_range);
    }

    // Test empty name and description in constructor.
    TEST_F(ProgressMarkerTest, ConstructorEmptyStrings)
    {
        ProgressMarker emptyMarker(4, "", "");
        EXPECT_EQ(emptyMarker.getID(), 4);
        EXPECT_EQ(emptyMarker.getName(), "");
        EXPECT_EQ(emptyMarker.getDescription(), "");
    }

} // namespace progress
