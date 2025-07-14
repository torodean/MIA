
# Create the RPG_Vitals_LIB
set(RPG_Vitals_SRC 
    Vitals.cpp
    VitalData.cpp )
set(RPG_Vitals_INC 
    Vital.hpp
    Vitals.hpp
    VitalData.hpp
    VitalRegistry.hpp )
add_library(RPG_Vitals_LIB ${RPG_Vitals_SRC} ${RPG_Vitals_INC})
target_link_libraries( RPG_Vitals_LIB PUBLIC Framework_CORE RPG_Tools_LIB )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Vitals_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# Create the RPG_Attributes_LIB
set(RPG_Attributes_SRC 
    AttributeData.cpp
    Attributes.cpp )
set(RPG_Attributes_INC 
    Attribute.hpp
    Attributes.hpp
    AttributeData.hpp
    AttributeRegistry.hpp )
add_library(RPG_Attributes_LIB ${RPG_Attributes_SRC} ${RPG_Attributes_INC})
target_link_libraries( RPG_Attributes_LIB PUBLIC Framework_CORE RPG_Tools_LIB )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Attributes_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# Include the test directory.
add_subdirectory( test )


