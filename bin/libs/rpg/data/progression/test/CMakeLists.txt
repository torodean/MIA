
add_executable(ProgressMarker_T ProgressMarker_T.cpp)
target_link_libraries(ProgressMarker_T PRIVATE Framework_CORE RPG_Progression_LIB GTest::gtest_main)
add_test(NAME ProgressMarker_T COMMAND ProgressMarker_T )

add_executable(ProgressRegistry_T ProgressRegistry_T.cpp)
target_link_libraries(ProgressRegistry_T PRIVATE Framework_CORE RPG_Progression_LIB GTest::gtest_main)
add_test(NAME ProgressRegistry_T COMMAND ProgressRegistry_T )

add_executable(ProgressMarkers_T ProgressMarkers_T.cpp)
target_link_libraries(ProgressMarkers_T PRIVATE Framework_CORE RPG_Progression_LIB GTest::gtest_main)
add_test(NAME ProgressMarkers_T COMMAND ProgressMarkers_T )
