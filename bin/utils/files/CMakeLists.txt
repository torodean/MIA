
# Create the Files_LIB
set(Files_SRC 
    FileUtils.cpp )
set(Files_INC 
    FileUtils.hpp )
add_library(Files_LIB ${Files_SRC} ${Files_INC})
target_link_libraries(Files_LIB PUBLIC Core_LIB)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Files_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
