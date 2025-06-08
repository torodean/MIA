
# Create the System_UTIL
find_library(XDO_LIB xdo)
set(System_SRC Timing.cpp VirtualKeyStrokes.cpp )
set(System_INC Timing.hpp VirtualKeyStrokes.hpp )
add_library(System_UTIL ${System_SRC} ${System_INC})
target_link_libraries(System_UTIL PUBLIC Framework_CORE)
target_link_libraries(System_UTIL PRIVATE ${XDO_LIB})

# Expose this library's source directory for #include access by dependent targets
target_include_directories(System_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )

if(WIN32 OR CYGWIN)
    add_subdirectory( windows )
endif()
