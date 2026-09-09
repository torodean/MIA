# This utility currently requires the python features to be available.
if(BUILD_PYTHON_FEATURES)
    set(Python_SRC PythonModule.cpp PythonResult.cpp )
    set(Python_INC PythonModule.hpp PythonResult.hpp )

    add_library(Python_UTIL ${Python_SRC} ${Python_INC})
    target_link_libraries(Python_UTIL PUBLIC Framework_CORE Python3::Python )
    
    # Expose this library's source directory for #include access by dependent targets
    target_include_directories(Python_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    
    # Add the unit tests.
    add_subdirectory( test )
endif()
