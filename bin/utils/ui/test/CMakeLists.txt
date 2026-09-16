
# Add tests for the SingleKeyListener.
add_executable(SingleKeyListener_T SingleKeyListener_T.cpp ../SingleKeyListener.cpp ../SingleKeyListener.hpp )
target_link_libraries(SingleKeyListener_T PRIVATE Framework_CORE System_UTIL GTest::gtest_main)
target_include_directories(SingleKeyListener_T PRIVATE ../)
add_test(NAME SingleKeyListener_T COMMAND SingleKeyListener_T )

