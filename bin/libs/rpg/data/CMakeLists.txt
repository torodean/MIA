
add_subdirectory( currencies )
add_subdirectory( stats )
add_subdirectory( progression )
add_subdirectory( player )

# Create the RPG_Data_LIB
set(RPG_DataLoader_SRC 
    DataLoader.cpp)
set(RPG_DataLoader_INC 
    DataLoader.hpp)
add_library(RPG_DataLoader_LIB ${RPG_DataLoader_SRC} ${RPG_DataLoader_INC})
target_link_libraries(RPG_DataLoader_LIB PUBLIC RPG_Currencies_LIB RPG_Vitals_LIB RPG_Attributes_LIB)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_DataLoader_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



# Create the RPG_Data_LIB
set(RPG_Data_SRC )
set(RPG_Data_INC 
    DataType.hpp
    BaseDataObject.hpp
    BaseDataObjectStorage.hpp )
add_library(RPG_Data_LIB INTERFACE)
target_sources(RPG_Data_LIB INTERFACE ${RPG_Data_INC})

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Data_LIB INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
