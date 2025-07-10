
add_subdirectory( currencies )
add_subdirectory( stats )
add_subdirectory( player )

# Create the RPG_Data_LIB
set(RPG_Data_SRC 
    DataLoader.cpp)
set(RPG_Data_INC 
    DataLoader.hpp)
add_library(RPG_Data_LIB ${RPG_Data_SRC} ${RPG_Data_INC})
target_link_libraries(RPG_Data_LIB PUBLIC RPG_Currencies_LIB RPG_Vitals_LIB)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Data_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
