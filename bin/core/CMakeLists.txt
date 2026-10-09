
# These are needed before Framework_CORE is created.
add_subdirectory( basic_utilities )
add_subdirectory( error )

# Header-only Constants_CORE (no source files).
set( Constants_INC
     Paths.hpp
     Constants.hpp )
add_library( Constants_CORE INTERFACE )
target_link_libraries( Constants_CORE INTERFACE BasicUtilities_CORE )
target_include_directories( Constants_CORE INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} )

# Create the Framework_CORE.
set( Core_SRC
     MIAConfig.cpp
     KeyValueData.cpp
     RawLinesData.cpp
     CommandParser.cpp
     MIAApplication.cpp 
     Logger.cpp 
     BackgroundTask.cpp )
set( Core_INC
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
add_library( Framework_CORE ${Core_SRC} ${Core_INC} )
target_link_libraries( Framework_CORE
                   PUBLIC
                       Constants_CORE
                       Error_CORE
                       BasicUtilities_CORE )

# Expose this library's source directory for #include access by dependent targets.
target_include_directories( Framework_CORE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( test )
