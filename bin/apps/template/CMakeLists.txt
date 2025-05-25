# Create the MIATemplate executable.
set(MIATemplate_SRC MIATemplate.cpp MIATemplate_main.cpp )
set(MIATemplate_INC MIATemplate.hpp )
add_executable(MIATemplate ${MIATemplate_SRC} ${MIATemplate_INC} )
target_link_libraries(MIATemplate PRIVATE Core_LIB )

if(SYSTEM_INSTALL)
    install(TARGETS MIATemplate DESTINATION ${APP_INSTALL_LOCATION})
endif()

if(RELEASE_BUILD)
    install(TARGETS MIATemplate DESTINATION ${RELEASE_INSTALL_LOCATION})
endif()
