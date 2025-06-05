add_executable(MarkovModels_T MarkovModels_T.cpp)
target_link_libraries(MarkovModels_T PRIVATE ML_UTIL GTest::gtest_main)
add_test(NAME MarkovModels_T COMMAND MarkovModels_T )
