
# Create the Math_UTIL
set(Math_SRC 
    MathUtils.cpp 
    D3CMath.cpp )
set(Math_INC 
    MathUtils.hpp
    D3CMath.hpp 
    MathTypes.hpp )
add_library(Math_UTIL ${Math_SRC} ${Math_INC})
target_link_libraries(Math_UTIL PUBLIC Types_UTIL)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Math_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
