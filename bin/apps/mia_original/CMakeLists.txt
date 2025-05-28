# Create the MIAOriginal executable.
set(MIAOriginal_SRC Commands.cpp MIAOriginal.cpp MIAOriginal_main.cpp )
set(MIAOriginal_INC Commands.hpp MIAOriginal.hpp )
add_executable(MIAOriginal ${MIAOriginal_SRC} ${MIAOriginal_INC} )
target_link_libraries(MIAOriginal PRIVATE Core_LIB Types_LIB Math_LIB System_LIB Files_LIB Audio_LIB Encryption_LIB )

if(SYSTEM_INSTALL)
    install(TARGETS MIAOriginal DESTINATION ${APP_INSTALL_LOCATION})
endif()

if(RELEASE_BUILD)
    install(TARGETS MIAOriginal DESTINATION ${RELEASE_INSTALL_LOCATION})
endif()
