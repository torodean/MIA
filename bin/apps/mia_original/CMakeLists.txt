# Create the MIAOriginal executable.
set(MIAOriginal_SRC Commands.cpp MIAOriginal.cpp MIAOriginal_main.cpp )
set(MIAOriginal_INC Commands.hpp MIAOriginal.hpp )
add_executable(MIAOriginal ${MIAOriginal_SRC} ${MIAOriginal_INC} )
target_link_libraries(MIAOriginal PRIVATE Framework_CORE Types_UTIL Math_UTIL System_UTIL Files_UTIL Audio_UTIL Encryption_UTIL Windows_UTIL )

if(SYSTEM_INSTALL)
    install(TARGETS MIAOriginal DESTINATION ${APP_INSTALL_LOCATION})
endif()

if(RELEASE_BUILD)
    install(TARGETS MIAOriginal DESTINATION ${RELEASE_INSTALL_LOCATION})
endif()
