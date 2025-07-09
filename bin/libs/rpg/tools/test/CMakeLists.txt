add_executable(Registry_T Registry_T.cpp)
target_link_libraries(Registry_T PRIVATE RPG_Tools_LIB GTest::gtest_main)
add_test(NAME Registry_T COMMAND Registry_T )
