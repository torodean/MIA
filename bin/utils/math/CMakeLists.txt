
# Create the Math_LIB
set(Math_SRC 
    MathUtils.cpp 
    D3CMath.cpp )
set(Math_INC 
    MathUtils.hpp
    D3CMath.hpp )
add_library(Math_LIB ${Math_SRC} ${Math_INC})
target_link_libraries(Math_LIB PUBLIC Types_LIB)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Math_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
