
# Create the RPG_Vitals_LIB
set(RPG_Vitals_SRC 
    Vitals.cpp )
set(RPG_Vitals_INC 
    Vital.hpp
    Vitals.hpp )
add_library(RPG_Vitals_LIB ${RPG_Vitals_SRC} ${RPG_Vitals_INC})
target_link_libraries( RPG_Vitals_LIB PUBLIC Framework_CORE RPG_Tools_LIB )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Vitals_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )
