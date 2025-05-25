/**
 * File: Constants.hpp
 * Author: Antonius Torode
 * Creation Date: 05/22/2025
 * Description: Declares project-wide Constants used across the application. These Constants 
 *     provide centralized access to certain variables such as the MIA version and other values
 *     set in the CMake.
 */
#pragma once

#include <string>

/**
 * @namespace constants
 * @brief Provides global constants.
 *
 * The constants namespace encapsulates application-wide default constants.
 */
namespace constants
{
    // The MIA Version value gathered from CMake.
    inline const std::string MIA_VERSION = MIA_VERSION_VAL;
    
    /**
     * Enum to specify the type of configuration data stored in an implementation of 
     * the ConfigData interface.
     * @see ConfigData
     */
    enum class ConfigType
    {
        KEY_VALUE,  // Key-value pairs separated by '='.
        RAW_LINES   // Raw file lines without parsing.
        // Future types (e.g., JSON, YAML) can be added here
    };
        
    /**
     * Alias for a key-value pair representing a single configuration entry.
     * Represents a key-value pair from the configuration,
     * where both key and value are strings.
     */
    using KeyValuePair = std::pair<std::string, std::string>;
    
    /**
     * @brief Represents a 2D coordinate with named integer components.
     */
    struct Coordinate 
    {
        Coordinate(int x_val = 0, int y_val = 0) : x(x_val), y(y_val) {}
        
        int x;
        int y;
    };
} // namespace constants
