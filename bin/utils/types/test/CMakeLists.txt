add_executable(StringUtils_T StringUtils_T.cpp)
target_link_libraries(StringUtils_T PRIVATE Types_LIB GTest::gtest_main)
add_test(NAME StringUtils_T COMMAND StringUtils_T )
