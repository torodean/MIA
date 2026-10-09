# Create the RPG_Tools_LIB
set(RPG_Tools_SRC
    Modifier.cpp
    Modifies.cpp )
set(RPG_Tools_INC
    Registry.hpp
    Modifier.hpp
    Modifies.hpp
    RegistryHelper.hpp
    ModifierApplicator.hpp )
add_library(RPG_Tools_LIB STATIC ${RPG_Tools_SRC} ${RPG_Tools_INC})
target_link_libraries(RPG_Tools_LIB PUBLIC Error_CORE RPG_Currencies_LIB RPG_Data_LIB )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Tools_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${NLOHMANN_JSON_INCLUDE_DIR})

add_subdirectory( test )
