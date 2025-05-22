
# This is needed before Core_LIB is created.
add_subdirectory( basic_utilities )

# Header-only Constants_LIB (no source files)
set(Constants_INC
    Paths.hpp
    Constants.hpp
)
add_library(Constants_LIB INTERFACE)
target_include_directories(Constants_LIB INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Create the Core_LIB.
set(Core_SRC 
    Error.cpp 
    ErrorDescriptions.cpp 
    MIAConfig.cpp 
    MIAApplication.cpp )
set(Core_INC 
    Error.hpp 
    ErrorDescriptions.hpp 
    MIAException.hpp 
    MIAConfig.hpp 
    MIAApplication.hpp 
    AppFramework.hpp )
add_library(Core_LIB ${Core_SRC} ${Core_INC})
target_link_libraries(Core_LIB PUBLIC Constants_LIB BasicUtilities_LIB)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Core_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

