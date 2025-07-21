/**
 * @file ProgressMarkers_T.cpp
 * @author Antonius Torode
 * @date 07/20/2025
 * @brief Unit tests for the ProgressMarkers class using Google Test.
 */

#include <gtest/gtest.h>
#include <sstream>
#include "ProgressMarkers.hpp"
#include "ProgressRegistry.hpp"

#include "MIAException.hpp"

namespace progress
{
    // Test fixture for ProgressMarkers tests, setting up common data.
    class ProgressMarkers_T : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            // Initialize a ProgressMarker for testing.
            marker = ProgressMarker(1, "Quest1", "First quest progress.");
            // Assume ProgressRegistry is populated for ID lookups.
            ProgressRegistry& registry = ProgressRegistry::getInstance();
            std::string jsonData = R"({
                "PROGRESS": [
                    {
                        "id": 1,
                        "name": "Quest1",
                        "description": "First quest progress."
                    },
                    {
                        "id": 2,
                        "name": "Quest2",
                        "description": "Second quest progress."
                    }
                ]
            })";
            registry.loadFromString(jsonData);
        }

        ProgressMarker marker; ///< Sample ProgressMarker for testing.
    };

    // Tests the default constructor.
    TEST_F(ProgressMarkers_T, DefaultConstructor)
    {
        ProgressMarkers markers;
        std::ostringstream oss;
        markers.dump(oss);
        EXPECT_EQ(oss.str(), "") << "Default constructor should create an empty container";
    }

    // Tests the get methods (by name, ID, and ProgressMarker).
    TEST_F(ProgressMarkers_T, GetMethods)
    {
        ProgressMarkers markers;
        markers.add("Quest1", 100);
        
        ProgressValue& valueByName = markers.get("Quest1");
        EXPECT_EQ(valueByName.get(), 100) << "get by name should return correct value";

        ProgressValue& valueById = markers.get(1);
        EXPECT_EQ(valueById.get(), 100) << "get by ID should return correct value";

        ProgressValue& valueByMarker = markers.get(marker);
        EXPECT_EQ(valueByMarker.get(), 100) << "get by ProgressMarker should return correct value";

        EXPECT_THROW(markers.get("NonExistent"), error::MIAException);
    }

    // Tests the add methods (by name, ID, and ProgressMarker).
    TEST_F(ProgressMarkers_T, AddMethods)
    {
        ProgressMarkers markers;
        
        markers.add("Quest1", 50);
        EXPECT_EQ(markers.get("Quest1").get(), 50) << "add by name should set initial value";

        EXPECT_THROW(markers.add("Quest1", 50), error::MIAException);

        markers.add(2, 75);
        EXPECT_EQ(markers.get(2).get(), 75) << "add by ID should set initial value";
    }

    // Tests the remove methods (by name, ID, and ProgressMarker).
    TEST_F(ProgressMarkers_T, RemoveMethods)
    {
        ProgressMarkers markers;
        markers.add("Quest1", 100);
        markers.add("Quest2", 200);

        markers.remove("Quest1");
        EXPECT_EQ(markers.get("Quest1").get(), 0) << "remove by name should clear value";

        markers.remove(2);
        EXPECT_EQ(markers.get(2).get(), 0) << "remove by ID should clear value";

        markers.remove(marker);
        EXPECT_EQ(markers.get(marker).get(), 0) << "remove by ProgressMarker should clear value";
    }

    // Tests the update methods (by name, ID, and ProgressMarker).
    TEST_F(ProgressMarkers_T, UpdateMethods)
    {
        ProgressMarkers markers;
        markers.add("Quest1", 100);

        markers.update("Quest1", 150);
        EXPECT_EQ(markers.get("Quest1").get(), 150) << "update by name should set new value";

        markers.update(1, 200);
        EXPECT_EQ(markers.get(1).get(), 200) << "update by ID should set new value";

        markers.update(marker, 300);
        EXPECT_EQ(markers.get(marker).get(), 300) << "update by ProgressMarker should set new value";
    }

    // Tests the has methods (by name, ID, and ProgressMarker).
    TEST_F(ProgressMarkers_T, HasMethods)
    {
        ProgressMarkers markers;
        markers.add("Quest1", 100);

        EXPECT_TRUE(markers.has("Quest1", 50)) << "has by name should return true for sufficient value";
        EXPECT_FALSE(markers.has("Quest1", 150)) << "has by name should return false for insufficient value";

        EXPECT_TRUE(markers.has(1, 50)) << "has by ID should return true for sufficient value";
        EXPECT_FALSE(markers.has(1, 150)) << "has by ID should return false for insufficient value";

        EXPECT_TRUE(markers.has(marker, 50)) << "has by ProgressMarker should return true for sufficient value";
        EXPECT_FALSE(markers.has(marker, 150)) << "has by ProgressMarker should return false for insufficient value";

        EXPECT_THROW(markers.has("NonExistent", 50), error::MIAException);
    }

    // Tests the dump method.
    TEST_F(ProgressMarkers_T, Dump)
    {
        ProgressMarkers markers;
        markers.add("Quest1", 100);
        markers.add("Quest2", 200);

        std::ostringstream oss;
        markers.dump(oss);
        std::string output = oss.str();

        EXPECT_TRUE(output.find("Quest1") != std::string::npos) << "Dump should contain Quest1 name";
        EXPECT_TRUE(output.find("Quest2") != std::string::npos) << "Dump should contain Quest2 name";
    }
    
    // Tests the serialize and deserialize methods.
    TEST_F(ProgressMarkers_T, SerializeDeserialize)
    {
        ProgressMarkers markers;
        markers.add("Quest1", 100);
        markers.add("Quest2", 200);

        std::string serialized = markers.serialize();
        EXPECT_TRUE(serialized.find("[PROGRESS_BEGIN]") != std::string::npos);
        EXPECT_TRUE(serialized.find("[PROGRESS_END]") != std::string::npos);
        EXPECT_TRUE(serialized.find("1:100") != std::string::npos); 
        EXPECT_TRUE(serialized.find("2:200") != std::string::npos);

        ProgressMarkers deserialized = ProgressMarkers::deserialize(serialized);
        EXPECT_EQ(deserialized.get("Quest1").get(), 100) << "deserialize should restore Quest1 value";
        EXPECT_EQ(deserialized.get("Quest2").get(), 200) << "deserialize should restore Quest2 value";
    }
} // namespace progress
