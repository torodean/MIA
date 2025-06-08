
# Create the Types_UTIL
set(Types_SRC StringUtils.cpp )
set(Types_INC 
    StringUtils.hpp
    VectorUtils.hpp
)
add_library(Types_UTIL ${Types_SRC} ${Types_INC})
target_link_libraries( Types_UTIL PUBLIC BasicUtilities_CORE )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Types_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
