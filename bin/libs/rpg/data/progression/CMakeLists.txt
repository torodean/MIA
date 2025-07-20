
# Create the RPG_Progression_LIB
set(RPG_Progression_SRC 
    ProgressMarker.cpp
    ProgressMarkers.cpp )
set(RPG_Progression_INC 
    ProgressMarker.hpp
    ProgressMarkers.hpp
    ProgressValue.hpp
    ProgressRegistry.hpp )
add_library(RPG_Progression_LIB ${RPG_Progression_SRC} ${RPG_Progression_INC})
target_link_libraries(RPG_Progression_LIB PUBLIC RPG_Tools_LIB RPG_Data_LIB )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Progression_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
