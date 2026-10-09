add_executable( Player_T Player_T.cpp )
target_link_libraries( Player_T
                   PRIVATE
                       RPG_Player_LIB
                       RPG_Currencies_LIB
                       RPG_Vitals_LIB
                       RPG_Attributes_LIB
                       RPG_Progression_LIB
                       Error_CORE
                       GTest::gtest_main )
add_test( NAME Player_T COMMAND Player_T )
