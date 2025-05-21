
# Create the Files_LIB
set(Files_SRC 
    FilesUtils.cpp )
set(Files_INC 
    FilesUtils.hpp )
add_library(Files_LIB ${Files_SRC} ${Files_INC})
target_link_libraries(Files_LIB PUBLIC Math_LIB Program_LIB)

# Include the test directory.
add_subdirectory( test )
