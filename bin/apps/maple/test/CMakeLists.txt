
# Add tests for the TaxConstants storage and associated methods.
add_executable(TaxConstants_T TaxConstants_T.cpp ../TaxConstants.cpp ../TaxConstants.hpp )
target_link_libraries(TaxConstants_T PRIVATE Framework_CORE Math_UTIL GTest::gtest_main)
target_include_directories(TaxConstants_T PRIVATE ../)
add_test(NAME TaxConstants_T COMMAND TaxConstants_T )

