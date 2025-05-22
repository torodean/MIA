
# Create the Types_LIB
set(Types_SRC StringUtils.cpp )
set(Types_INC StringUtils.hpp )
add_library(Types_LIB ${Types_SRC} ${Types_INC})
target_link_libraries( Types_LIB PUBLIC BasicUtilities_LIB )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Types_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
