add_executable( PythonUIListener_T PythonUIListener_T.cpp )
target_link_libraries( PythonUIListener_T 
                   PRIVATE
                       Py_UI_LIB         # The code to test.
                       Framework_CORE    # For BackgroundTask.
                       Python_UTIL       # For python modules.
                       UI_UTIL           # For event storage.
                       System_UTIL       # For timing.    
                       GTest::gtest_main # Google test framework.
                     )
add_test( NAME PythonUIListener_T COMMAND PythonUIListener_T )
