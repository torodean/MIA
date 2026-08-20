# Create the Maple executable.
set(MAPLE_SRC 
    Maple.cpp 
    Maple_main.cpp
    TaxConstants.cpp )
set(MAPLE_INC 
    Maple.hpp 
    TaxConstants.hpp )
add_executable(maple ${MAPLE_SRC} ${MAPLE_INC} )
target_link_libraries(maple PRIVATE Framework_CORE System_UTIL Math_UTIL )

if(SYSTEM_INSTALL)
    install(TARGETS maple DESTINATION ${APP_INSTALL_LOCATION})
endif()

if(RELEASE_BUILD)
    install(TARGETS maple DESTINATION ${RELEASE_INSTALL_LOCATION})
endif()

#add_subdirectory( test )
