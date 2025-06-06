add_executable(StringUtils_T StringUtils_T.cpp)
target_link_libraries(StringUtils_T PRIVATE Types_UTIL GTest::gtest_main)
add_test(NAME StringUtils_T COMMAND StringUtils_T )

add_executable(VectorUtils_T VectorUtils_T.cpp)
target_link_libraries(VectorUtils_T PRIVATE Types_UTIL GTest::gtest_main)
add_test(NAME VectorUtils_T COMMAND VectorUtils_T )
