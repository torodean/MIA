
# Create the RPG_Attributes_LIB
set(RPG_Attributes_SRC 
    Attribute.cpp
    Attributes.cpp
    AttributeData.cpp )
set(RPG_Attributes_INC 
    Attribute.hpp
    Attributes.hpp
    AttributeData.hpp
    AttributeRegistry.hpp )
add_library(RPG_Attributes_LIB ${RPG_Attributes_SRC} ${RPG_Attributes_INC})
target_link_libraries( RPG_Attributes_LIB PUBLIC Framework_CORE RPG_Tools_LIB RPG_Data_LIB )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(RPG_Attributes_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )


