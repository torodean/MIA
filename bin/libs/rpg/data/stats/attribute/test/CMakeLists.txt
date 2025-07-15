add_executable(Attribute_T Attribute_T.cpp)
target_link_libraries(Attribute_T PRIVATE Framework_CORE RPG_Attributes_LIB GTest::gtest_main)
add_test(NAME Attribute_T COMMAND Attribute_T )

add_executable(AttributeData_T AttributeData_T.cpp)
target_link_libraries(AttributeData_T PRIVATE Framework_CORE RPG_Attributes_LIB GTest::gtest_main)
add_test(NAME AttributeData_T COMMAND AttributeData_T )

add_executable(AttributeRegistry_T AttributeRegistry_T.cpp)
target_link_libraries(AttributeRegistry_T PRIVATE Framework_CORE RPG_Attributes_LIB GTest::gtest_main)
add_test(NAME AttributeRegistry_T COMMAND AttributeRegistry_T )
