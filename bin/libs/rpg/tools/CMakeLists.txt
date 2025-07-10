
# Create the RPG_Tools_LIB
set(RPG_Tools_SRC )
set(RPG_Tools_INC 
    Registry.hpp
    Modifier.hpp )
add_library(RPG_Tools_LIB INTERFACE)
target_sources(RPG_Tools_LIB INTERFACE ${RPG_Tools_INC})
target_link_libraries(RPG_Tools_LIB INTERFACE RPG_Currencies_LIB)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Tools_LIB INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory( test )
