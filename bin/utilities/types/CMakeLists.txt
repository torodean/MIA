
# Create the Types_LIB
set(Types_SRC StringUtils.cpp )
set(Types_INC StringUtils.hpp )
add_library(Types_LIB ${Types_SRC} ${Types_INC})

# Include the test directory.
add_subdirectory( test )
