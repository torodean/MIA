
# Add tests for the MIAException features.
add_executable(MIAException_T MIAException_T.cpp)
target_link_libraries(MIAException_T PRIVATE Framework_CORE GTest::gtest_main)
add_test(NAME MIAException_T COMMAND MIAException_T )

# Add tests for the CommandOption features.
add_executable(CommandOption_T CommandOption_T.cpp)
target_link_libraries(CommandOption_T PRIVATE Framework_CORE GTest::gtest_main)
add_test(NAME CommandOption_T COMMAND CommandOption_T )

# Add tests for the CommandParser features.
add_executable(CommandParser_T CommandParser_T.cpp)
target_link_libraries(CommandParser_T PRIVATE Framework_CORE GTest::gtest_main)
add_test(NAME CommandParser_T COMMAND CommandParser_T )
