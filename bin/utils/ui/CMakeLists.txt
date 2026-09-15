# Create the UI_UTIL
set(UI_SRC
    Event.cpp
    EventQueue.cpp
    EventStorage.cpp )
set(UI_INC
    Event.hpp
    EventQueue.hpp
    EventStorage.hpp )
add_library(UI_UTIL ${UI_SRC} ${UI_INC})

# Expose this library's source directory for #include access by dependent targets
target_include_directories(UI_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
