
# Checking if Python development libraries are available to build.
find_package(Python3 COMPONENTS Development.Embed)

if (Python3_FOUND)
    set(BUILD_PYTHON_FEATURES ON)
    message(STATUS "Found Python development libraries. BUILDING Python components!")
else()
    set(BUILD_PYTHON_FEATURES OFF)
    message(STATUS "Python development libraries not found. SKIPPING Python components!")
endif()

# Non-OS-specific path configurations.
set(PYTHON_INSTALL_LOCATION "${APP_INSTALL_LOCATION}/python")

# Installs the specified Python files into the system Python directory when
# performing a system install, or into the release Python resources directory
# when building a release.
function(install_python_files)
    if(SYSTEM_INSTALL)
        install(FILES ${ARGV} DESTINATION ${PYTHON_INSTALL_LOCATION})
    elseif(RELEASE_BUILD)
        install(FILES ${ARGV} DESTINATION ${RELEASE_PYTHON_INSTALL_LOCATION})
    endif()
endfunction()
