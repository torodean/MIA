# Checking if the libvlc library is available to build.
# This is used for audio playing on linux.
if(UNIX AND NOT APPLE AND NOT CYGWIN)
    # VLC is required on linux. It should be installed with the setup script.
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBVLC REQUIRED libvlc)
    set(BUILD_VLC_FEATURES ON)
    message(STATUS "Found libvlc. BUILDING vlc components!")
else()
    # Non-Linux platforms use their own audio APIs instead of libvlc.
    set(BUILD_VLC_FEATURES OFF)
endif()
