
# Create the BasicUtilities_LIB.
set(BasicUtilities_SRC 
    BasicUtilities.cpp )
set(BasicUtilities_INC 
    BasicUtilities.hpp )
add_library(BasicUtilities_LIB ${BasicUtilities_SRC} ${BasicUtilities_INC})

# Expose this library's source directory for #include access by dependent targets
target_include_directories(BasicUtilities_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

