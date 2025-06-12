if(WIN32 OR CYGWIN)
	# Create the fishbot executable.
	set(WoWFishbot_SRC WoWFishbot.cpp WoWFishbot_main.cpp )
	set(WoWFishbot_INC WoWFishbot.hpp )
	add_executable(WoWFishbot ${WoWFishbot_SRC} ${WoWFishbot_INC} )
	target_link_libraries(WoWFishbot PRIVATE Framework_CORE System_UTIL gdi32 )
	
	if(SYSTEM_INSTALL)
		install(TARGETS WoWFishbot DESTINATION ${APP_INSTALL_LOCATION})
	endif()

	if(RELEASE_BUILD)
		install(TARGETS WoWFishbot DESTINATION ${RELEASE_INSTALL_LOCATION})
	endif()
endif()

