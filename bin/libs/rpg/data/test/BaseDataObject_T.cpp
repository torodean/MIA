/**
 * @file BaseDataObject_T.cpp
 * @author Antonius Torode
 * @date 10/12/2026
 * @brief Unit tests for the rpg::BaseDataObject class.
 */

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "BaseDataObject.hpp"


namespace rpg
{
    class BaseDataObject_T : public BaseDataObject
    {
    public:
        /// Constructor. This will set all of the object values contained in the base class.
        BaseDataObject_T(uint32_t id,
                         const std::string& name,
                         const std::string& description) :
            BaseDataObject(id, name, description) {};

        /// Wrappers around protected members to make them accessible to the tests.
        nlohmann::json toJson() const override
        { return BaseDataObject::toJson(); }
        static BaseDataObject fromJson(const nlohmann::json& json)
        { return BaseDataObject::fromJson(json); }
    };


    /**
     * @brief Test that the constructor works with the various getters.
     */
    TEST(BaseDataObject_T, baseDataObjectGetters)
    {
        BaseDataObject data(3, "testData", "Test Description");

        ASSERT_EQ(data.getID(), 3);
        ASSERT_EQ(data.getName(), "testData");
        ASSERT_EQ(data.getDescription(), "Test Description");
    }


    /**
     * @brief Tests whether the to/from JSON round trip behaves as expected.
     */
    TEST(BaseDataObject_T, baseDataObjectRoundTrip)
    {
        BaseDataObject_T data(0, "testData", "Test Description");

        nlohmann::json dataAsJson = data.toJson();

        BaseDataObject newData = BaseDataObject_T::fromJson(dataAsJson);

        // Test all fields for equality. 
        ASSERT_EQ(data.getID(), newData.getID());
        ASSERT_EQ(data.getName(), newData.getName());
        ASSERT_EQ(data.getDescription(), newData.getDescription());
    }
} // namespace rpg
