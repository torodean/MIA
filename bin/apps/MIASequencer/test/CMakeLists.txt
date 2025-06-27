
# Add tests for the KeyListenerTask.
add_executable(KeyListenerTask_T KeyListenerTask_T.cpp ../KeyListenerTask.cpp ../KeyListenerTask.hpp )
target_link_libraries(KeyListenerTask_T PRIVATE Framework_CORE System_UTIL GTest::gtest_main)
target_include_directories(KeyListenerTask_T PRIVATE ../)
add_test(NAME KeyListenerTask_T COMMAND KeyListenerTask_T )

