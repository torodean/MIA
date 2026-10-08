# Add tests for the MIAException features.
add_executable(MIAException_T MIAException_T.cpp)
target_link_libraries(MIAException_T PRIVATE Error_CORE GTest::gtest_main)
add_test(NAME MIAException_T COMMAND MIAException_T )
