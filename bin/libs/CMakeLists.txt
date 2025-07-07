
add_subdirectory( ml )

if ( BUILD_DATABASE_FEATURES )
	add_subdirectory( database )
endif()

add_subdirectory( rpg )
