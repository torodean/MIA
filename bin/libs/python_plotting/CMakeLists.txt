# Adding the py_plotting_LIB
set( PY_PLOTTING_SRC PythonPlotter.cpp )
set( PY_PLOTTING_INC PythonPlotter.hpp )
add_library( Py_Plotting_LIB ${PY_PLOTTING_SRC} ${PY_PLOTTING_INC} )
target_link_libraries( Py_Plotting_LIB PUBLIC Framework_CORE Python_UTIL )

# Expose this library's source directory for #include access by dependent targets
target_include_directories( Py_Plotting_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Install the python files if needed.
install_python_files( PythonPlotter.py )

# Add the directory where tests are located.
#add_subdirectory( test ) 
