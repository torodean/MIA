
# Create the System_UTIL
set(System_SRC Timing.cpp VirtualKeyStrokes.cpp )
set(System_INC Timing.hpp VirtualKeyStrokes.hpp )
add_library(System_UTIL ${System_SRC} ${System_INC})
target_link_libraries(System_UTIL PUBLIC Framework_CORE)
if(UNIX AND NOT APPLE AND NOT CYGWIN)
    find_library(XDO_LIB xdo REQUIRED)    
    find_package(X11 REQUIRED)
    find_library(XTST_LIB Xtst REQUIRED)
    
    target_link_libraries(System_UTIL PRIVATE ${XDO_LIB} ${X11_LIBRARIES} ${XTST_LIB})
    target_include_directories(System_UTIL PRIVATE ${X11_INCLUDE_DIR})
elseif(WIN32 OR CYGWIN)
	target_link_libraries(System_UTIL PRIVATE gdi32 )
endif()


# Expose this library's source directory for #include access by dependent targets
target_include_directories(System_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )

if(WIN32 OR CYGWIN)
    add_subdirectory( windows )
endif()
