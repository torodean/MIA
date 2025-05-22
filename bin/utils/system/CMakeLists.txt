
# Create the System_LIB
set(System_SRC Timing.cpp )
set(System_INC Timing.hpp )
add_library(System_LIB ${System_SRC} ${System_INC})

# Include the test directory.
add_subdirectory( test )
