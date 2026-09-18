# Create the System_UTIL
set(System_SRC
    Timing.cpp )
set(System_INC
    Timing.hpp
    TerminalColors.hpp )

# The virtual key strokes use Windows APIs directly; on Linux they need libxdo/X11,
# so they are only built when those are available.
if ( WIN32 OR CYGWIN OR BUILD_XDO_FEATURES )
    list(APPEND System_SRC VirtualKeyStrokes.cpp)
    list(APPEND System_INC VirtualKeyStrokes.hpp)
endif()

add_library(System_UTIL ${System_SRC} ${System_INC})
target_link_libraries(System_UTIL PRIVATE Framework_CORE)

# Math_UTIL is only used by the virtual key strokes (random number presses).
if(BUILD_XDO_FEATURES)
    target_link_libraries(System_UTIL 
        PRIVATE  Math_UTIL ${XDO_LIB} ${X11_LIBRARIES} ${XTST_LIB} )
    target_include_directories(System_UTIL PRIVATE ${X11_INCLUDE_DIR})

    # Lets dependent targets (and this library) know the xdo features are compiled in.
    target_compile_definitions(System_UTIL PUBLIC BUILD_XDO_FEATURES)
elseif(WIN32 OR CYGWIN)
	target_link_libraries(System_UTIL 
        PRIVATE Math_UTIL gdi32 )
endif()

# Expose this library's source directory for #include access by dependent targets
target_include_directories(System_UTIL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Include the test directory.
add_subdirectory( test )

if(WIN32 OR CYGWIN)
    add_subdirectory( windows )
endif()
