add_executable(Timing_T Timing_T.cpp)
target_link_libraries(Timing_T PRIVATE System_LIB GTest::gtest_main)
add_test(NAME Timing_T COMMAND Timing_T )
