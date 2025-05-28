message(STATUS "Compiling Windows only libraries...")
set(Windows_SRC LOLANetUse.cpp)
set(Windows_INC LOLANetUse.hpp)

add_library(Windows_LIB ${Windows_SRC} ${Windows_INC})
target_link_libraries(Windows_LIB netapi32)

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Windows_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

