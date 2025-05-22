
# Create the System_LIB
set(System_SRC Timing.cpp VirtualKeyStrokes.cpp )
set(System_INC Timing.hpp VirtualKeyStrokes.hpp )
add_library(System_LIB ${System_SRC} ${System_INC})
target_link_libraries(System_LIB PUBLIC Core_LIB)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(System_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
