

set(Audio_SRC SystemSounds.cpp)
set(Audio_INC SystemSounds.hpp)

add_library(Audio_UTIL ${Audio_SRC} ${Audio_INC})
target_link_libraries(Audio_UTIL PRIVATE Framework_CORE )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Audio_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
