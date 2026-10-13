# One of the purposes of the Testing_CORE library is that the various MIA tests
# can link to it without having to also link to the google test framework.
# This set of tests also tests that component/feature.
add_executable( TestingUtils_T TestingUtils_T.cpp )
target_link_libraries( TestingUtils_T PRIVATE Testing_CORE )
add_test( NAME TestingUtils_T COMMAND TestingUtils_T )
