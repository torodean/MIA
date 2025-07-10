# Create the RPGFiddler executable.
set(RPGFiddler_SRC RPGFiddler.cpp RPGFiddler_main.cpp RPGSimulator.cpp )
set(RPGFiddler_INC RPGFiddler.hpp RPGSimulator.hpp )
add_executable(RPGFiddler ${RPGFiddler_SRC} ${RPGFiddler_INC} )
target_link_libraries( RPGFiddler PRIVATE Framework_CORE Files_UTIL RPG_Data_LIB RPG_Player_LIB )

if(SYSTEM_INSTALL)
    install(TARGETS RPGFiddler DESTINATION ${APP_INSTALL_LOCATION})
endif()

if(RELEASE_BUILD)
    install(TARGETS RPGFiddler DESTINATION ${RELEASE_INSTALL_LOCATION})
endif()
