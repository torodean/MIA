
# This is needed before Framework_CORE is created.
add_subdirectory( basic_utilities )

# Header-only Constants_LIB (no source files)
set(Constants_INC
    Paths.hpp
    Constants.hpp
)
add_library(Constants_LIB INTERFACE)
target_include_directories(Constants_LIB INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Create the Framework_CORE.
set(Core_SRC 
    Error.cpp
    ErrorDescriptions.cpp
    MIAConfig.cpp
    KeyValueData.cpp
    RawLinesData.cpp
    CommandParser.cpp
    MIAApplication.cpp 
    Logger.cpp )
set(Core_INC 
    Error.hpp
    ErrorDescriptions.hpp
    MIAException.hpp
    MIAConfig.hpp
    ConfigData.hpp
    KeyValueData.hpp
    RawLinesData.hpp
    MIAApplication.hpp
    CommandParser.hpp
    CommandOption.hpp
    AppFramework.hpp 
    Logger.hpp 
    BackgroundTask.hpp
    RuntimeContext.hpp )
add_library(Framework_CORE ${Core_SRC} ${Core_INC})
target_link_libraries(Framework_CORE PUBLIC Constants_LIB BasicUtilities_CORE)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Framework_CORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory( test )
