add_executable( BaseDataObject_T BaseDataObject_T.cpp )
target_link_libraries( BaseDataObject_T 
                   PRIVATE
                       RPG_Data_LIB      # Classes to test.
                       GTest::gtest_main # For Google test framework.
                     )
add_test(NAME BaseDataObject_T COMMAND BaseDataObject_T )
