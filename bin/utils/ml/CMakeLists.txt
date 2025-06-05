# Create the ML_UTIL
set(ML_SRC )
set(ML_INC 
    MarkovModels.hpp )
add_library(ML_UTIL ${ML_SRC} ${ML_INC})
target_link_libraries(ML_UTIL PUBLIC Types_UTIL)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(ML_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
