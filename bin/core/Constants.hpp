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
} // namespace constants
