
# Checking if libxdo and X11 libraries are available to build.
# These are used for the virtual key stroke features on Linux.
if(UNIX AND NOT APPLE AND NOT CYGWIN)
    find_library(XDO_LIB xdo)
    find_package(X11)
    find_library(XTST_LIB Xtst)

    if(XDO_LIB AND X11_FOUND AND XTST_LIB)
        set(BUILD_XDO_FEATURES ON)
        message(STATUS "Found libxdo and X11. BUILDING xdo components!")
    else()
        set(BUILD_XDO_FEATURES OFF)
        list(APPEND SKIPPED_FEATURES "xdo")
        message(STATUS "libxdo or X11 not found. SKIPPING xdo components!")
    endif()
else()
    # Windows uses its own input APIs instead of xdo.
    set(BUILD_XDO_FEATURES OFF)
endif()
