
add_subdirectory( ml )

if ( BUILD_DATABASE_FEATURES )
	add_subdirectory( database )
endif()

# OS-Specific Path Configuration
if(WIN32 OR CYGWIN)
    message(STATUS "RPG Libraries not yet supported on Windows!")
elseif(UNIX AND NOT APPLE)
    add_subdirectory( rpg )
endif()
