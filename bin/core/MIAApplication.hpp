/**
 * File: MIAApplication.hpp
 * Author: Antonius Torode
 * Creation Date: 05/22/2025
 * Description: Defines a base class for applications with virtual
 *              app methods. Handles verbose mode flag parsing.
 */
#pragma once

#include <string>

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
    MIAApplication() = default;

    /// Virtual destructor.
    virtual ~MIAApplication() = default;

    /**
     * Virtual initialize() method. This should be overridden by the inheriting app, but a call
     * to this method should be included.
     * Parse command line arguments, handling common flags (-v, -h).
     * Calls parseAppArguments for app-specific argument parsing.
     * @param argc, argv Command line arguments
     * TODO - Add MIAException handling for failure.
     */
    virtual void initialize(int argc, char* argv[]);

    /**
     * Runs the main application logic.
     * Must be implemented by derived classes.
     *
     * @return int Exit status code.
     */
    virtual int run() = 0;

    /**
     * Get verbose mode flag.
     */
    bool getVerboseMode() const 
    { return verboseMode; }
    
protected:
    /**
     * Prints common help info including verbose and help flags.
     */
    virtual void printHelp() const;
    
    /// Stores verboseMode.
    bool verboseMode{false};
    
    /// True if the user specified the help flag in command options.
    bool helpRequested{false};
};
