/**
 * @file MIAApplication.hpp
 * @author Antonius Torode
 * @date 05/22/2025
 * Description: Defines a base class for applications with virtual
 *              app methods. Handles verbose mode flag parsing.
 */
#pragma once

#include <string>

// Used for base configuration values.
#include "CommandOption.hpp"
#include "Logger.hpp"
#include "RuntimeContext.hpp"


/**
 * @class MIAApplication
 * @brief Base class for MIA applications, providing a standard interface
 *        for initialization and execution. Handles common flags such as
 *        verbose mode and help display.
 */
class MIAApplication
{
public:
    /// Default constructor.
    MIAApplication();

    /// Virtual destructor.
    virtual ~MIAApplication() = default;

    /**
     * Get verbose mode flag.
     */
    bool getVerboseMode() const;

    /**
     * Get the debugLevel.
     */
    unsigned int getdebugLevel() const;
    
    /**
     * Log a message using the logger object. This will automatically set the verbose
     * mode based on the verbose flag.
     * @param message The message to log.
     */
    void log(const std::string& message) const;
    
    /**
     * Log a message using the logger object with an optional verboseMode flag. This
     * overload allows the called to set the verbose flag without using the default
     * application verbose flag parsed from command line options.
     * @param message The message to log.
     * @param verbose Whether to print the message to stdout.
     */
    void log(const std::string& message, bool verbose) const;
    
    /**
     * Logs the name of the calling method and optional parameters using the logger object.
     * This is needed here to be called in the LOG_METHOD_CALL() and LOG_METHOD_CALL_WITH_PARAMS()
     * macros so that logger can stay a private member variable. Verbosity is handled internally
     * within the macros, but can be overloaded by calling this method directly.
     *
     * @param methodName Name of the calling method (typically passed via __func__).
     * @param params Optional string representing parameters to include in the log.
     * @param verbose Whether to print the message to stdout.
    */
    void logMethodCall(const std::string& methodName,
                       const std::string& params = "",
                       bool verbose = false);
    
protected:

    /**
     * Virtual initialize() method. This should be overridden by the inheriting app, but a call
     * to this method should be included. Parse command line arguments, handling common flags 
     * (-v, -h). Calls parseAppArguments for app-specific argument parsing.
     *
     * @param argc, argv Command line arguments
     * @throw Throws an exception if parsing the command line options for base arguments fail.
     */
    virtual void initialize(int argc, char* argv[]);

    /**
     * Runs the main application logic. Must be implemented by derived classes. When implemented, 
     * this should ideally return a valid return code from constants::ReturnCode. 
     *
     * @return int Exit status code.
     */
    virtual int run() = 0;
    
    /**
     * Prints common help info including verbose and help flags.
     */
    virtual void printHelp() const;
    
    /**
     * Returns the executable name if needed. 
     */
    std::string getExecutableName();
    
    /**
     * @brief Provides read-only access to the runtime context.
     * 
     * Returns a constant reference to the RuntimeContext instance,
     * allowing other classes to inspect runtime variables without
     * modifying them.
     * 
     * @return const RuntimeContext& Reference to the runtime context.
     */
    const RuntimeContext& getContext() const;
    
private:
  
    /// The RuntimeContext for this class - stores common runtime variables.
    RuntimeContext context;
    
    /// Base command options used by the MIAApplication.
    CommandOption verboseOpt;
    CommandOption debugOpt;
    CommandOption helpOpt;
    CommandOption logFileOpt;
    
    /// True if the user specified the help flag in command options.
    bool helpRequested{false};
    
    /// Store the executable name for use in the help message.
    std::string executableName;
}; // class MIAApplication

// TODO: Replace macros with an inline method using C++20 std::source_location
// to automatically capture the caller function name without needing __func__ or macros.
// This improves maintainability and type safety.

/**
 * @def LOG_METHOD_CALL
 * @brief Macro for logging the entry point of a method.
 *
 * Automatically logs the name of the calling method using the application's logger instance.
 * with an optional string representation of parameters.
 * Internally passes `__func__` to logger::Logger::logMethodCall() through the
 * MIAApplication::logMethodCall() method.
 * This macro should be used at the start of a method to aid debugging and traceability.
 *
 * @note This macro must be called after MIAApplication::initialize() has finished in order
 * to have access to the automatic verbose handling.
 *
 * Example:
 * @code
 * void MyClass::doWork() {
 *     LOG_METHOD_CALL();
 *     // method logic...
 * }
 * @endcode
 */
#define LOG_METHOD_CALL(...)                                                                         \
    do {                                                                                             \
        std::string _params = std::string(#__VA_ARGS__).empty() ? "" : std::string(__VA_ARGS__);     \
        this->logMethodCall(__func__, _params, this->getVerboseMode());                              \
    } while (0)                                      
