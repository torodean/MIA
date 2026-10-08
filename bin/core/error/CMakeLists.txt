# Create the Error_CORE.
set(Error_SRC
    Error.cpp
    ErrorDescriptions.cpp )
set(Error_INC
    Error.hpp
    ErrorDescriptions.hpp
    MIAException.hpp )
add_library(Error_CORE ${Error_SRC} ${Error_INC})

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Error_CORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory( test )
