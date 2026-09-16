# Create the UI_UTIL
set(UI_SRC
    Event.cpp
    EventQueue.cpp
    EventStorage.cpp )
set(UI_INC
    Event.hpp
    EventQueue.hpp
    EventStorage.hpp )

# Optionally add the keyListenerTask on environments that can use it
# (windows or when XDO is available)..
if ( WIN32 OR CYGWIN OR BUILD_XDO_FEATURES )
    list(APPEND UI_SRC SingleKeyListener.cpp)
    list(APPEND UI_INC SingleKeyListener.hpp)
endif()

add_library(UI_UTIL ${UI_SRC} ${UI_INC})

if ( WIN32 OR CYGWIN OR BUILD_XDO_FEATURES )
    target_link_libraries(UI_UTIL PRIVATE Framework_CORE System_UTIL)
endif()

# Expose this library's source directory for #include access by dependent targets
target_include_directories(UI_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add test directory.
add_subdirectory( test )
