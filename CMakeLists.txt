cmake_minimum_required(VERSION 3.17)

# The version of the current MIA code.
set(MIA_VERSION_VAL "2.000")

# Define the project name and version.
project(MIA VERSION ${MIA_VERSION_VAL})

# Add MIAVERSION as a preprocessor variable.
add_definitions ( -DMIAVERSION=\"${MIA_VERSION_VAL}\" )

# Define cpp standards and build flags.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall")
set(MIA_PROJECT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory(bin)
#add_subdirectory(documentation)
