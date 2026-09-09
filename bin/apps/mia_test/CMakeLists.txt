# This app currently requires the python features to be available.
if(BUILD_PYTHON_FEATURES)
    # Create the test executable.
    set(test_SRC MIATest.cpp MIATest_main.cpp )
    set(test_INC MIATest.hpp )

    #  The target name "test" is reserved when CTest testing is enabled.
    add_executable(miatest ${test_SRC} ${test_INC} )
    target_link_libraries(miatest PRIVATE Framework_CORE Python_UTIL Python_Plotting_LIB )

    # Python files are installed into the system python directory or collected
    # into the release resources folder, depending on the build mode.
    if(SYSTEM_INSTALL)
        install(TARGETS miatest DESTINATION ${APP_INSTALL_LOCATION})
        install(FILES MIATest.py DESTINATION ${PYTHON_INSTALL_LOCATION})
    endif()

    if(RELEASE_BUILD)
        install(TARGETS miatest DESTINATION ${RELEASE_INSTALL_LOCATION})
        install(FILES MIATest.py DESTINATION ${RELEASE_PYTHON_INSTALL_LOCATION})
    endif()
endif()
