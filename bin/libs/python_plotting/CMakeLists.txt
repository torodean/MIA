# Adding the Python_Plotting_LIB
set( Python_Plotting_SRC PythonPlotter.cpp )
set( Python_Plotting_INC PythonPlotter.hpp )
add_library( Python_Plotting_LIB ${Python_Plotting_SRC} ${Python_Plotting_INC} )
target_link_libraries( Python_Plotting_LIB PUBLIC Framework_CORE Python_UTIL )

# Expose this library's source directory for #include access by dependent targets
target_include_directories( Python_Plotting_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Install the python files if needed.
install_python_files( PythonPlotter.py )

# Add the directory where tests are located.
#add_subdirectory( test ) 
