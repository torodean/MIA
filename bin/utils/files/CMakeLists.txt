
# Create the Files_UTIL
set(Files_SRC 
    FileUtils.cpp )
set(Files_INC 
    FileUtils.hpp )
add_library(Files_UTIL ${Files_SRC} ${Files_INC})
target_link_libraries(Files_UTIL PUBLIC Core_LIB)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Files_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
