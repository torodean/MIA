add_executable(FileUtils_T FileUtils_T.cpp)
target_link_libraries(FileUtils_T PRIVATE Files_UTIL GTest::gtest_main)
add_test(NAME FileUtils_T COMMAND FileUtils_T )
