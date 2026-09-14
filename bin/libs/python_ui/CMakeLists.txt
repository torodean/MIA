# Adding the Py_UI_LIB
set( PY_UI_SRC Event.cpp EventStorage.cpp PythonUIListener.cpp )
set( PY_UI_INC Event.hpp EventStorage.hpp PythonUIListener.hpp )
add_library( Py_UI_LIB ${PY_UI_SRC} ${PY_UI_INC} )
target_link_libraries( Py_UI_LIB PUBLIC Framework_CORE Python_UTIL System_UTIL )

# Expose this library's source directory for #include access by dependent targets
target_include_directories( Py_UI_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

# Add the directory where tests are located.
#add_subdirectory( test )
