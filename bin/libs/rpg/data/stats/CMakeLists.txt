
# Create the RPG_CharacterVitals_LIB
set(RPG_CharacterVitals_SRC 
    CharacterVitals.cpp )
set(RPG_CharacterVitals_INC 
    CharacterVitals.hpp )
add_library(RPG_CharacterVitals_LIB ${RPG_CharacterVitals_SRC} ${RPG_CharacterVitals_INC})

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_CharacterVitals_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
# add_subdirectory( test )
