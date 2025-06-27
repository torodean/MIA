# Create the MIASequencer executable.
set(MIASequencer_SRC 
    MIASequencer.cpp 
    MIASequencer_main.cpp 
    KeyListenerTask.cpp )
set(MIASequencer_INC 
    MIASequencer.hpp 
    KeyListenerTask.hpp )
add_executable(MIASequencer ${MIASequencer_SRC} ${MIASequencer_INC} )
target_link_libraries(MIASequencer PRIVATE Framework_CORE System_UTIL Types_UTIL Math_UTIL )

if(SYSTEM_INSTALL)
    install(TARGETS MIASequencer DESTINATION ${APP_INSTALL_LOCATION})
endif()

if(RELEASE_BUILD)
    install(TARGETS MIASequencer DESTINATION ${RELEASE_INSTALL_LOCATION})
endif()

add_subdirectory( test )
