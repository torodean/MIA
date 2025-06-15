
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

# Add tests for the MIAConfig features.
add_executable(MIAConfig_T MIAConfig_T.cpp)
target_link_libraries(MIAConfig_T PRIVATE Framework_CORE GTest::gtest_main)
add_test(NAME MIAConfig_T COMMAND MIAConfig_T )

# Add tests for the Logger features.
add_executable(Logger_T Logger_T.cpp)
target_link_libraries(Logger_T PRIVATE Framework_CORE GTest::gtest_main)
add_test(NAME Logger_T COMMAND Logger_T )

# Add tests for the AppFramework features.
add_executable(AppFramework_T AppFramework_T.cpp)
target_link_libraries(AppFramework_T PRIVATE Framework_CORE GTest::gtest_main)
add_test(NAME AppFramework_T COMMAND AppFramework_T )
