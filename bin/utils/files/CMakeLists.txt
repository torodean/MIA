
# Create the Files_LIB
set(Files_SRC 
    FileUtils.cpp )
set(Files_INC 
    FileUtils.hpp )
add_library(Files_LIB ${Files_SRC} ${Files_INC})
target_link_libraries(Files_LIB PUBLIC Core_LIB)

# Include the test directory.
add_subdirectory( test )
