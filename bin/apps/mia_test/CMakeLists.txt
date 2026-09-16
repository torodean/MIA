# This app currently requires the python and XDO features to be available.
if(BUILD_PYTHON_FEATURES && BUILD_XDO_FEATURES)
    # Create the test executable.
    set(test_SRC
        MIATest.cpp
        MIATest_main.cpp
        PythonModuleTests.cpp
        PythonPlotterTests.cpp
        PythonUITests.cpp 
        KeybindTests.cpp )
    set(test_INC
        MIATest.hpp
        PythonModuleTests.hpp
        PythonPlotterTests.hpp
        PythonUITests.hpp 
        KeybindTests.hpp )

    #  The target name "test" is reserved when CTest testing is enabled.
    add_executable(miatest ${test_SRC} ${test_INC} )
    target_link_libraries(miatest PRIVATE Framework_CORE Python_UTIL Py_Plotting_LIB Py_UI_LIB )

    # Install the app.
    install_app( miatest )
    
    # Install the python files if needed.
    install_python_files( MIATest.py testUI.py )
endif()
