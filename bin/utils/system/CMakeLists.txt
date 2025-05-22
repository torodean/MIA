
# Create the System_LIB
set(System_SRC Timing.cpp VirtualKeyStrokes.cpp )
set(System_INC Timing.hpp VirtualKeyStrokes.hpp )
add_library(System_LIB ${System_SRC} ${System_INC})
target_link_libraries(System_LIB PUBLIC Core_LIB)

# Include the test directory.
add_subdirectory( test )
