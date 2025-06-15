add_executable(BasicUtilities_T BasicUtilities_T.cpp)
target_link_libraries(BasicUtilities_T PRIVATE BasicUtilities_CORE GTest::gtest_main)
add_test(NAME BasicUtilities_T COMMAND BasicUtilities_T )
