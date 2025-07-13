/**
 * @file Registry_T.cpp
 * @author Antonius Torode
 * @date 07/09/2025
 * Description: Unit tests for the templated Registry base class in the MIA RPG system using Google Test.
 */

#include <gtest/gtest.h>
#include "Registry.hpp"

using json = nlohmann::json;

namespace rpg 
{
    struct DummyObject 
    {
        uint32_t id;
        std::string name;
    };

    class DummyRegistry : public Registry<DummyRegistry, DummyObject> 
    {
    public:
        static DummyRegistry& getInstance() 
        {
            static DummyRegistry instance;
            return instance;
        }

    protected:
        std::string getJsonKey() const override { return "dummy"; }
        
        DummyObject parseJson(const json& j) override 
        {
            return DummyObject{ j.at("id").get<uint32_t>(), j.at("name").get<std::string>() };
        }

        std::string toString(const DummyObject& obj) const override 
        {
            return "DummyObject{id=" + std::to_string(obj.id) + ", name=" + obj.name + "}";
        }
    };

} // namespace rpg

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
};

TEST_F(RegistryTest, GetById_ReturnsCorrectObject) 
{
    const auto* obj = rpg::DummyRegistry::getInstance().getById(1);
    ASSERT_NE(obj, nullptr);
    EXPECT_EQ(obj->name, "Foo");
}

TEST_F(RegistryTest, GetByName_ReturnsCorrectObject) 
{
    const auto* obj = rpg::DummyRegistry::getInstance().getByName("Bar");
    ASSERT_NE(obj, nullptr);
    EXPECT_EQ(obj->id, 2);
}

TEST_F(RegistryTest, GetById_InvalidIdReturnsNullptr) 
{
    const auto* obj = rpg::DummyRegistry::getInstance().getById(999);
    EXPECT_EQ(obj, nullptr);
}

TEST_F(RegistryTest, GetByName_InvalidNameReturnsNullptr) 
{
    const auto* obj = rpg::DummyRegistry::getInstance().getByName("Invalid");
    EXPECT_EQ(obj, nullptr);
}

TEST_F(RegistryTest, Dump_OutputIsNotEmpty) 
{
    std::stringstream ss;
    rpg::DummyRegistry::getInstance().dump(ss);
    std::string output = ss.str();
    EXPECT_NE(output.find("DummyObject{id=1, name=Foo}"), std::string::npos);
    EXPECT_NE(output.find("DummyObject{id=2, name=Bar}"), std::string::npos);
}
