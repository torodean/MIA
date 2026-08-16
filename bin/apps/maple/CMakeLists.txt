# Create the Maple executable.
set(Maple_SRC 
    Maple.cpp 
    Maple_main.cpp )
set(Maple_INC 
    Maple.hpp )
add_executable(Maple ${Maple_SRC} ${Maple_INC} )
target_link_libraries(Maple PRIVATE Framework_CORE System_UTIL Math_UTIL )

if(SYSTEM_INSTALL)
    install(TARGETS Maple DESTINATION ${APP_INSTALL_LOCATION})
endif()

if(RELEASE_BUILD)
    install(TARGETS Maple DESTINATION ${RELEASE_INSTALL_LOCATION})
endif()

add_subdirectory( test )
