
add_subdirectory( ml )

if ( BUILD_DATABASE_FEATURES )
	add_subdirectory( database )
endif()

# Only build this is the json requirements are met.
if( BUILD_NLOHMANN_JSON_FEATURES )
    add_subdirectory( rpg )
endif()

if ( BUILD_PYTHON_FEATURES )
    add_subdirectory( python_plotting )
	add_subdirectory( python_ui )
endif()
