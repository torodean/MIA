add_executable(Registry_T Registry_T.cpp)
target_link_libraries(Registry_T PRIVATE RPG_Tools_LIB GTest::gtest_main)
add_test(NAME Registry_T COMMAND Registry_T )

add_executable(Modifier_T Modifier_T.cpp)
target_link_libraries(Modifier_T PRIVATE RPG_Tools_LIB GTest::gtest_main)
add_test(NAME Modifier_T COMMAND Modifier_T )

add_executable(Modifies_T Modifies_T.cpp)
target_link_libraries(Modifies_T PRIVATE RPG_Tools_LIB GTest::gtest_main)
add_test(NAME Modifies_T COMMAND Modifies_T )

add_executable(ModifierApplicator_T ModifierApplicator_T.cpp)
target_link_libraries(ModifierApplicator_T PRIVATE RPG_Tools_LIB RPG_Attributes_LIB RPG_Vitals_LIB Constants_CORE GTest::gtest_main)
add_test(NAME ModifierApplicator_T COMMAND ModifierApplicator_T )
