message(STATUS "Compiling Windows only libraries...")
set(Windows_SRC LOLANetUse.cpp WindowsUtils.cpp )
set(Windows_INC LOLANetUse.hpp WindowsUtils.hpp )

add_library(Windows_UTIL ${Windows_SRC} ${Windows_INC})
target_link_libraries(Windows_UTIL netapi32 Framework_CORE )

# Expose this library's source directory for #include access by dependent targets
target_include_directories(Windows_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

