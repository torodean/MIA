/**
 * @file Registry_T.cpp
 * @author Antonius Torode
 * @date 07/09/2025
 * @brief Unit tests for the Registry base class.
 */

#include <gtest/gtest.h>

// Include the associated file for testing.
#include "Registry.hpp"
// Used for exception and error handling.
#include "MIAException.hpp"

namespace rpg
{
    /**
     * A minimal object type used to exercise the Registry base class.
     */
    struct DummyObject
    {
        uint32_t id;
        std::string name;
    };


    /**
     * A minimal Registry instantiation used to exercise the base class behavior.
     */
    class DummyRegistry : public Registry<DummyRegistry, DummyObject>
    {
    public:
        static DummyRegistry& getInstance() 
        {
            static DummyRegistry instance;
            return instance;
        }

    protected:
        std::string getJsonKey() const override 
        { 
            return "dummy"; 
        }
        
        DummyObject parseJson(const nlohmann::json& j) override 
        {
            return DummyObject{ j.at("id").get<uint32_t>(), j.at("name").get<std::string>() };
        }

        std::string toString(const DummyObject& obj) const override 
        {
            return "DummyObject{id=" + std::to_string(obj.id) + ", name=" + obj.name + "}";
        }
    }; // class DummyRegistry


    /**
     * Test fixture for the Registry tests. Loads two DummyObject entries (ids 1 and 2)
     * into the DummyRegistry singleton before each test.
     */
    class RegistryTest : public ::testing::Test
    {
    protected:
        void SetUp() override 
        {
            std::ofstream file("test.json");
            file << R"({ "dummy": [
                {"id": 1, "name": "Foo"},
                {"id": 2, "name": "Bar"}
            ]})";
            file.close();

            rpg::DummyRegistry::getInstance().loadFromFile("test.json");
        }
    }; // class RegistryTest


    /**
     * @brief Verifies getByID returns the object registered under the requested id.
     */
    TEST_F(RegistryTest, getByID_ReturnsCorrectObject)
    {
        const auto* obj = rpg::DummyRegistry::getInstance().getByID(1);
        ASSERT_NE(obj, nullptr);
        EXPECT_EQ(obj->name, "Foo");
    }


    /**
     * @brief Verifies getByName returns the object registered under the requested name.
     */
    TEST_F(RegistryTest, GetByName_ReturnsCorrectObject)
    {
        const auto* obj = rpg::DummyRegistry::getInstance().getByName("Bar");
        ASSERT_NE(obj, nullptr);
        EXPECT_EQ(obj->id, 2);
    }


    /**
     * @brief Verifies getByID returns nullptr for an id that was never registered.
     */
    TEST_F(RegistryTest, getByID_InvalidIdReturnsNullptr)
    {
        const auto* obj = rpg::DummyRegistry::getInstance().getByID(999);
        EXPECT_EQ(obj, nullptr);
    }


    /**
     * @brief Verifies getByName returns nullptr for a name that was never registered.
     */
    TEST_F(RegistryTest, GetByName_InvalidNameReturnsNullptr)
    {
        const auto* obj = rpg::DummyRegistry::getInstance().getByName("Invalid");
        EXPECT_EQ(obj, nullptr);
    }


    /**
     * @brief Verifies dump writes every registered object's string representation.
     */
    TEST_F(RegistryTest, Dump_OutputIsNotEmpty)
    {
        std::stringstream ss;
        rpg::DummyRegistry::getInstance().dump(ss);
        std::string output = ss.str();
        EXPECT_NE(output.find("DummyObject{id=1, name=Foo}"), std::string::npos);
        EXPECT_NE(output.find("DummyObject{id=2, name=Bar}"), std::string::npos);
    }


    /**
     * @brief Verifies loading a JSON entry with a duplicate id throws instead of silently
     * overwriting the earlier entry.
     */
    TEST_F(RegistryTest, LoadFromJson_DuplicateIdThrows)
    {
        EXPECT_THROW(rpg::DummyRegistry::getInstance().loadFromString(
            R"({ "dummy": [ {"id": 1, "name": "Foo"}, {"id": 1, "name": "Other"} ] })"),
            error::MIAException);
    }


    /**
     * @brief Verifies loading a JSON entry whose name is already registered throws.
     */
    TEST_F(RegistryTest, LoadFromJson_DuplicateNameThrows)
    {
        EXPECT_THROW(rpg::DummyRegistry::getInstance().loadFromString(
            R"({ "dummy": [ {"id": 1, "name": "Foo"}, {"id": 2, "name": "Foo"} ] })"),
            error::MIAException);
    }


    /**
     * @brief Verifies loading a JSON entry missing its id or name key throws instead of
     * surfacing a raw nlohmann exception.
     */
    TEST_F(RegistryTest, LoadFromJson_MissingEntryKeysThrows)
    {
        EXPECT_THROW(rpg::DummyRegistry::getInstance().loadFromString(
            R"({ "dummy": [ {"name": "NoId"} ] })"),
            error::MIAException);

        EXPECT_THROW(rpg::DummyRegistry::getInstance().loadFromString(
            R"({ "dummy": [ {"id": 1} ] })"),
            error::MIAException);
    }
} // namespace rpg
