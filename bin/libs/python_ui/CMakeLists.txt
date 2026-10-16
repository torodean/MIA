# Adding the Py_UI_LIB
set( PY_UI_SRC PythonUIListener.cpp )
set( PY_UI_INC PythonUIListener.hpp )
add_library( Py_UI_LIB ${PY_UI_SRC} ${PY_UI_INC} )
target_link_libraries( Py_UI_LIB 
                   PRIVATE
                       Framework_CORE # For the BackgroundTask functionality.
                       System_UTIL    # For timing components.
                   PUBLIC # Any code using this needs the following:
                       Python_UTIL # For interfacing with python files.
                       UI_UTIL     # For handling UI events.
                     )

# Expose this library's source directory for #include access by dependent targets
target_include_directories( Py_UI_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Add the directory where tests are located.
add_test_dir()
