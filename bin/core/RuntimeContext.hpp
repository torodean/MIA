/**
 * @file RuntimeContext.hpp
 * @author Antonius Torode
 * @date 05/22/2025
 * Description: Defines a class for storing application runtime context information. This
 *     is used for accessing information core to all applications (verbose, debug, etc).
 */
#pragma once

#include "Logger.hpp"

/**
 * @brief Holds runtime configuration and shared services for application components.
 *
 * This struct encapsulates context information that is useful throughout the execution 
 * of the application, such as the global logger instance and runtime flags.
 * It is intended to be passed to subsystems or utility classes that require 
 * consistent access to application-wide settings.
 *
 * Members:
 * - logger: A shared logger instance for uniform logging across components.
 * - verboseMode: Enables verbose output when true.
 * - debugLevel: Indicates the level of debug verbosity (0 = none).
 */
struct RuntimeContext
{
    /// The basic logger for this an application to use.
    logger::Logger logger;
    
    /// Stores verboseMode.
    bool verboseMode{false};
    
    /// Stores debug level.
    unsigned int debugLevel{0};
};
