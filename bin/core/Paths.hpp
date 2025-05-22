/**
 * File: Paths.hpp
 * Author: Antonius Torode
 * Creation Date: 05/22/2025
 * Description: Declares project-wide directory paths used
 *     across the application. These paths provide centralized access to default
 *     resource locations such as configuration files, logs, and user data directories.
 */
#pragma once

#include <string>

/**
 * @namespace MIA_paths
 * @brief Provides global paths for key file and directory paths.
 *
 * The paths namespace encapsulates application-wide default paths 
 * for configuration files, log directories, and other system resources.
 * These constants are set at build time through CMake and ensure consistent 
 * usage of resource locations across the entire codebase.
 */
namespace paths
{
    // Path to the main configuration directory when installed.
    inline const std::string SYSTEM_CONFIG_FILE_DIR = DEFAULT_SYSTEM_CONFIG_FILE_DIR;
    
    // Path to the main configuration file when installed.
    inline const std::string SYSTEM_CONFIG_FILE = DEFAULT_SYSTEM_CONFIG_FILE;
    
    // Path to the configuration directory located in the repo directory (for testing).
    inline const std::string REPO_CONFIG_FILE_DIR = DEFAULT_REPO_CONFIG_FILE_DIR;    
    
    // Path to the configuration file from the repo directory (for testing).
    inline const std::string REPO_CONFIG_FILE = DEFAULT_REPO_CONFIG_FILE;

    // Path to the default log directory when installed.
    inline const std::string SYSTEM_LOG_DIR = DEFAULT_SYSTEM_LOG_DIR;

    // Path to the default log file when installed.
    inline const std::string SYSTEM_LOG = DEFAULT_SYSTEM_LOG;

    // Path to the default log directory in the repo directory (for testing).
    inline const std::string REPO_LOG_DIR = DEFAULT_REPO_LOG_DIR;

    // Path to the default log file from the repo directory (for testing).
    inline const std::string REPO_LOG = DEFAULT_REPO_LOG;
} // namespace paths
