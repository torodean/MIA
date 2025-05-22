cmake_minimum_required(VERSION 3.17)

# The version of the current MIA code.
set(MIA_VERSION_VAL "2.000")

# Define the project name and version.
project(MIA VERSION ${MIA_VERSION_VAL})

# Add MIAVERSION as a preprocessor variable.
add_definitions ( -DMIAVERSION=\"${MIA_VERSION_VAL}\" )

# Define cpp standards and build flags.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall")
set(MIA_PROJECT_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# OS-Specific Path Configuration
if(WIN32)
    set(DEFAULT_SYSTEM_CONFIG_FILE "C:/ProgramData/mia/MIAConfig.MIA")
    set(DEFAULT_SYSTEM_LOG_DIR "C:/ProgramData/mia/logs")
elseif(UNIX AND NOT APPLE)
    set(DEFAULT_SYSTEM_CONFIG_FILE "/etc/mia/MIAConfig.MIA")
    set(DEFAULT_SYSTEM_LOG_DIR "/var/log/MIALog.log")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Non-OS-specific path configurations.
set(DEFAULT_REPO_CONFIG_FILE "${CMAKE_SOURCE_DIR}/resources/MIAConfig.MIA")
set(DEFAULT_REPO_LOG_DIR "${CMAKE_SOURCE_DIR}/logs")

# Pass definitions to the compiler
add_compile_definitions(
    DEFAULT_SYSTEM_CONFIG_FILE=\"${DEFAULT_SYSTEM_CONFIG_FILE}\"
    DEFAULT_SYSTEM_LOG_DIR=\"${DEFAULT_SYSTEM_LOG_DIR}\"
    DEFAULT_REPO_CONFIG_FILE=\"${DEFAULT_REPO_CONFIG_FILE}\"
    DEFAULT_REPO_LOG_DIR=\"${DEFAULT_REPO_LOG_DIR}\"
)

add_subdirectory(bin)
#add_subdirectory(documentation)
