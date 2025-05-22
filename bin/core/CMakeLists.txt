# Create the Core_LIB.
set(Core_SRC 
    Error.cpp 
    ErrorDescriptions.cpp )
set(Core_INC 
    Error.hpp 
    ErrorDescriptions.hpp 
    MIAException.hpp )
add_library(Core_LIB ${Core_SRC} ${Core_INC})

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Core_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
