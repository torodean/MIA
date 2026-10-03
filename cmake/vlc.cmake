# Checking if the libvlc library is available to build.
# This is used for audio playing on Linux.
if(UNIX AND NOT APPLE AND NOT CYGWIN)
    # VLC is ususally required on linux. It should be installed with the setup script.
    find_library(LIBVLC_LIB vlc)
    find_path(LIBVLC_INCLUDE_DIR
        NAMES vlc/vlc.h
        PATH_SUFFIXES include
    )

    if(LIBVLC_LIB AND LIBVLC_INCLUDE_DIR)
        set(BUILD_VLC_FEATURES ON)
        message(STATUS "Found libvlc. BUILDING vlc components!")
    else()
        set(BUILD_VLC_FEATURES OFF)
        list(APPEND SKIPPED_FEATURES "vlc")
        message(STATUS "libvlc or its headers not found. SKIPPING vlc components!")
    endif()
else()
    # Non-Linux platforms use their own audio APIs instead of libvlc.
    set(BUILD_VLC_FEATURES OFF)
endif()
