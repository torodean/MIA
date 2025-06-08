# Create the MIAWorkout executable.
set(MIAWorkout_SRC MIAWorkout.cpp MIAWorkout_main.cpp )
set(MIAWorkout_INC MIAWorkout.hpp )
add_executable(MIAWorkout ${MIAWorkout_SRC} ${MIAWorkout_INC} )
target_link_libraries(MIAWorkout PRIVATE Framework_CORE Types_UTIL Math_UTIL)

if(SYSTEM_INSTALL)
    install(TARGETS MIAWorkout DESTINATION ${APP_INSTALL_LOCATION})
endif()

if(RELEASE_BUILD)
    install(TARGETS MIAWorkout DESTINATION ${RELEASE_INSTALL_LOCATION})
endif()
