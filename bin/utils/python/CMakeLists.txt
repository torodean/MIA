set(Python_SRC PythonModule.cpp PythonResult.cpp )
set(Python_INC PythonModule.hpp PythonResult.hpp )

add_library( Python_UTIL ${Python_SRC} ${Python_INC} )
target_link_libraries( Python_UTIL 
                   PRIVATE
                       Framework_CORE 
                   PUBLIC
                       Python3::Python )

# Expose this library's source directory for #include access by dependent targets.
target_include_directories( Python_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Add the unit tests.
add_test_dir()
