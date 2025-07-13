
# Add a template program for testing and to serve as a general app template.
add_subdirectory( template )

# Add various app folders.
add_subdirectory( old_game_tools )
add_subdirectory( workout )
add_subdirectory( MIASequencer )
add_subdirectory( mia_original )

# OS-Specific Path Configuration
if(WIN32 OR CYGWIN)
    message(STATUS "RPG Libraries not yet supported on Windows!")
elseif(UNIX AND NOT APPLE)
    add_subdirectory( rpg )
endif()
