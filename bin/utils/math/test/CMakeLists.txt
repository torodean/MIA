add_executable(Math_T D3CMath_T.cpp MathUtils_T.cpp)
target_link_libraries(Math_T PRIVATE Math_LIB GTest::gtest_main)
add_test(NAME Math_T COMMAND Math_T )
