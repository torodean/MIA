# This app currently requires the python features to be available.
if(BUILD_PYTHON_FEATURES)
    # Create the test executable.
    set(test_SRC MIATest.cpp MIATest_main.cpp )
    set(test_INC MIATest.hpp )

    #  The target name "test" is reserved when CTest testing is enabled.
    add_executable(miatest ${test_SRC} ${test_INC} )
    target_link_libraries(miatest PRIVATE Framework_CORE Python_UTIL Py_Plotting_LIB )

    # Install the app.
    install_app( miatest )
    
    # Install the python files if needed.
    install_python_files( MIATest.py )
endif()
