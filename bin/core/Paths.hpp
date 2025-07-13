/**
 * @file Paths.hpp
 * @author Antonius Torode
 * @date 05/22/2025
 * Description: Declares project-wide directory paths used
 *     across the application. These paths provide centralized access to default
 *     resource locations such as configuration files, logs, and user data directories.
 */
#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
#include <windows.h>  // for GetModuleFileName
#elif __linux__
#include <unistd.h>   // for readlink
#include <limits.h>   // for PATH_MAX
#endif

#include <filesystem>
#include <string>
#include <filesystem>

// Used for creating directories.
#include "BasicUtilities.hpp"

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

    // Path to the system level installation directory.
    inline const std::string INSTALL_LOCATION = APP_INSTALL_LOCATION;
    
    /**
     * @brief Retrieves the directory of the currently running executable.
     * For Linux:
     * Uses the /proc/self/exe symbolic link to obtain the full path of the
     * executable, then extracts the directory portion.
     * For windows:
     * Uses Windows API GetModuleFileName to obtain the full path of the
     * executable, then extracts the directory portion.
     * 
     * @return std::string The directory path of the executable.
     */
    inline std::string getExecutableDir()
    {
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        std::string fullPath(path);
        return fullPath.substr(0, fullPath.find_last_of("\\/"));  
    #elif __linux__
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        std::string path(result, (count > 0) ? count : 0);
        return path.substr(0, path.find_last_of('/'));
    #endif
    }
    
    /**
     * Determines whether the application is running from a system-installed location
     * or from the repository (development/testing) directory.
     *
     * This checks whether the executable path is located within the system installation directory.
     *
     * @return [bool] - true if running from an installed system location; false if running from the repo.
     */
    inline bool isInstalled()
    {
        namespace fs = std::filesystem;
        std::error_code errCode;

        fs::path execPath = fs::canonical(fs::current_path(), errCode);
        if (errCode) 
        {
            return false;
        }

        fs::path installPath = fs::canonical(fs::path(INSTALL_LOCATION), errCode);
        if (errCode) 
        {
            return false;
        }

        return std::mismatch(installPath.begin(), installPath.end(), execPath.begin()).first == installPath.end();
    }
    
    /**
     * Returns the default configuration directory path based on the runtime context.
     *
     * If the application is running from a system-installed location, this returns the
     * system configuration directory. If a 'resources' folder exists in the same direcrory
     * as the executable, that will be returned second. Otherwise, it returns the git-repository 
     * configuration directory, typically used for development or testing.
     *
     * @return [std::string] - Path to the appropriate configuration directory.
     */
    inline std::string getDefaultConfigDirToUse()
    {
        if (isInstalled())
        {
            BasicUtilities::ensureDirectoryExists(SYSTEM_CONFIG_FILE_DIR, true);
            return SYSTEM_CONFIG_FILE_DIR;
        }
        else
        {
            std::string resourcesFolder = getExecutableDir() + "/resources";
            if (std::filesystem::exists(resourcesFolder))
                return resourcesFolder;
            else
            {
                BasicUtilities::ensureDirectoryExists(REPO_CONFIG_FILE_DIR, true);
                return REPO_CONFIG_FILE_DIR;
            }
        }
    }    
    
    /**
     * Returns the default log directory path based on the runtime context.
     *
     * If the application is running from a system-installed location, this returns the
     * system log directory. If a 'resources' folder exists in the same direcrory
     * as the executable, that will be returned second. Otherwise, it returns the git-repository 
     * log directory, typically used for development or testing.
     *
     * @return [std::string] - Path to the appropriate configuration directory.
     */
    inline std::string getDefaultLogDirToUse()
    {
        if (isInstalled())
        {
            BasicUtilities::ensureDirectoryExists(SYSTEM_LOG_DIR, true);
            return SYSTEM_LOG_DIR;
        }
        else
        {
            std::string resourcesFolder = getExecutableDir() + "/resources";
            if (std::filesystem::exists(resourcesFolder))
                return resourcesFolder;
            else
            {
                BasicUtilities::ensureDirectoryExists(REPO_LOG_DIR, true);
                return REPO_LOG_DIR;
            }
        }
    }

} // namespace paths







