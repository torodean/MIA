# Create the RPG_Player_LIB
set(RPG_Player_SRC 
    Player.cpp )
set(RPG_Player_INC 
    Player.hpp )
add_library( RPG_Player_LIB ${RPG_Player_SRC} ${RPG_Player_INC} )
target_link_libraries( RPG_Player_LIB PUBLIC RPG_Currencies_LIB RPG_CharacterVitals_LIB )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Player_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
