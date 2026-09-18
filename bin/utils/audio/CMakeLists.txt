

set(Audio_SRC 
    SystemSounds.cpp
	SoundsFromFile.cpp )
set(Audio_INC 
    SystemSounds.hpp
	SoundsFromFile.hpp )

add_library(Audio_UTIL ${Audio_SRC} ${Audio_INC})
target_link_libraries(Audio_UTIL PRIVATE Framework_CORE Types_UTIL Files_UTIL System_UTIL)

if(WIN32 OR CYGWIN)
    target_link_libraries(Audio_UTIL PRIVATE winmm)
endif()

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Audio_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
