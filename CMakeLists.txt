cmake_minimum_required(VERSION 3.17)

# The version of the current MIA code.
set(MIA_VERSION_VAL "2.001")

# Define the project name and version.
project(MIA VERSION ${MIA_VERSION_VAL})

# Add MIAVERSION as a preprocessor variable.
add_definitions ( -DMIA_VERSION_VAL=\"${MIA_VERSION_VAL}\" )

# Define cpp standards and build flags.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall")
set(MIA_PROJECT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Specifies a release build.
option(RELEASE_BUILD "Enable release-specific behavior" OFF)
if(RELEASE_BUILD)
    get_filename_component(RELEASE_INSTALL_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/../release" ABSOLUTE)
    set(RELEASE_CONFIG_INSTALL_LOCATION "${RELEASE_INSTALL_LOCATION}/resources")
    message(STATUS "Release build enabled. Install path: ${RELEASE_INSTALL_LOCATION}")
endif()

# OS-Specific Path Configuration
if(WIN32)
    set(APP_INSTALL_LOCATION "C:/Program Files/mia")
    set(DEFAULT_SYSTEM_CONFIG_FILE_DIR "C:/ProgramData/mia")
    set(DEFAULT_SYSTEM_LOG_DIR "C:/ProgramData/mia/logs")
elseif(UNIX AND NOT APPLE)
    set(APP_INSTALL_LOCATION "/usr/local/mia")
    set(DEFAULT_SYSTEM_CONFIG_FILE_DIR "/etc/mia")
    set(DEFAULT_SYSTEM_LOG_DIR "/var/log/mia")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Specifies a system install.
option(SYSTEM_INSTALL "Enable system-installation behavior" OFF)
if(SYSTEM_INSTALL)
    message(STATUS "System install enabled. Install path: ${APP_INSTALL_LOCATION}")
endif()

# Non-OS-specific path configurations.
set(DEFAULT_SYSTEM_CONFIG_FILE "${DEFAULT_SYSTEM_CONFIG_FILE_DIR}/MIAConfig.MIA")
set(DEFAULT_SYSTEM_LOG "${DEFAULT_SYSTEM_LOG_DIR}/MIALog.log")
set(DEFAULT_REPO_CONFIG_FILE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(DEFAULT_REPO_CONFIG_FILE "${DEFAULT_REPO_CONFIG_FILE_DIR}/MIAConfig.MIA")
set(DEFAULT_REPO_LOG_DIR "${CMAKE_SOURCE_DIR}/logs")
set(DEFAULT_REPO_LOG "${DEFAULT_REPO_LOG_DIR}/MIALog.log")

# Pass definitions to the compiler
add_compile_definitions(
    DEFAULT_SYSTEM_CONFIG_FILE_DIR=\"${DEFAULT_SYSTEM_CONFIG_FILE_DIR}\"
    DEFAULT_SYSTEM_CONFIG_FILE=\"${DEFAULT_SYSTEM_CONFIG_FILE}\"
    DEFAULT_REPO_CONFIG_FILE_DIR=\"${DEFAULT_REPO_CONFIG_FILE_DIR}\"
    DEFAULT_REPO_CONFIG_FILE=\"${DEFAULT_REPO_CONFIG_FILE}\"
    DEFAULT_SYSTEM_LOG_DIR=\"${DEFAULT_SYSTEM_LOG_DIR}\"
    DEFAULT_SYSTEM_LOG=\"${DEFAULT_SYSTEM_LOG}\"
    DEFAULT_REPO_LOG_DIR=\"${DEFAULT_REPO_LOG_DIR}\"
    DEFAULT_REPO_LOG=\"${DEFAULT_REPO_LOG}\"
    APP_INSTALL_LOCATION=\"${APP_INSTALL_LOCATION}\"
)

add_subdirectory( bin )
#add_subdirectory( documentation )
add_subdirectory( resources )
