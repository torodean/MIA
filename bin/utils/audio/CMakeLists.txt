

set(Audio_SRC SystemSounds.cpp)
set(Audio_INC SystemSounds.hpp)

add_library(Audio_LIB ${Audio_SRC} ${Audio_INC})
target_link_libraries(Audio_LIB PRIVATE Core_LIB )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Audio_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
