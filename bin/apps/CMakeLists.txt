
# Add a template program for testing and to serve as a general app template.
add_subdirectory( template )

# Add various app folders.
add_subdirectory( maple )
if ( WIN32 OR CYGWIN OR BUILD_XDO_FEATURES )
    # These apps use the virtual key strokes, which use Windows APIs directly; on Linux
    # they need libxdo/X11, so the apps are only built when those are available.
    add_subdirectory( sara )
    add_subdirectory( mia_original )
endif()
add_subdirectory( mia_test )
add_subdirectory( old_game_tools )
add_subdirectory( workout )

add_subdirectory( rpg )
