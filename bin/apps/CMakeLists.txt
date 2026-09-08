
# Add a template program for testing and to serve as a general app template.
add_subdirectory( template )

# Add various app folders.
add_subdirectory( maple )
add_subdirectory( mia_sequencer )

# TODO - GitHub (for some absolute stupid reason) removed my main branch protection.
# Therefore, I mistakenly pushed a bunch of in-test changes to main instead of the web branch.
# This needs commented out to let main pass the pipeline.
#add_subdirectory( mia_test )


add_subdirectory( mia_original )
add_subdirectory( old_game_tools )
add_subdirectory( workout )

# OS-Specific Path Configuration
if(WIN32 OR CYGWIN)
    message(STATUS "RPG Libraries not yet supported on Windows!")
elseif(UNIX AND NOT APPLE)
    add_subdirectory( rpg )
endif()
