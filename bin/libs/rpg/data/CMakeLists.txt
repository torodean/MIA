
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



# Create the RPG_DataType_LIB
set(RPG_DataType_SRC )
set(RPG_DataType_INC 
    DataType.hpp )
add_library(RPG_DataType_LIB INTERFACE)
target_sources(RPG_DataType_LIB INTERFACE ${RPG_DataType_INC})

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_DataType_LIB INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
