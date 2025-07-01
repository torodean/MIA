
# Create the Security_UTIL
set(System_SRC
    Credentials.cpp )
set(System_INC
    Credentials.hpp )
add_library(Security_UTIL ${System_SRC} ${System_INC})
target_link_libraries( Security_UTIL PUBLIC Framework_CORE )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Security_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
