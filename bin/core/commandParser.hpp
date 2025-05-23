/**
 * File: CommandParser.hpp
 * Author: Antonius Torode
 * Creation Date: 05/22/2025
 * Description: This file contains various utilities for parsing command options.
 */
#pragma once

#include <string>

namespace command_parser
{
    /**
     * Parses a boolean flag from command-line arguments.
     * @param argc[int] - Argument count.
     * @param argv[char**] - Argument values.
     * @param shortArg[const std::string&] - Short option (e.g., "-v").
     * @param longArg[const std::string&] - Long option (e.g., "--verbose").
     * @param outValue[bool&] - Output boolean value to set.
     */
    void parseBoolFlag(int argc, char* argv[], 
                      const std::string& shortArg,
                      const std::string& longArg,
                      bool& outValue);

    /**
     * Parses an integer option from command-line arguments.
     * @param argc[int] - Argument count.
     * @param argv[char**] - Argument values.
     * @param shortArg[const std::string&] - Short option (e.g., "-t").
     * @param longArg[const std::string&] - Long option (e.g., "--threads").
     * @param outValue[int&] - Output integer value to set.
     * @param required[bool] - True if this option is required.
     * @throws MIAException - Thrown if the option is not found or the value is not a valid integer.
     */
    void parseIntOption(int argc, char* argv[], 
                        const std::string& shortArg,
                        const std::string& longArg,
                        int& outValue,
                        bool required = false);

    /**
     * Parses a double option from command-line arguments.
     * @param argc[int] - Argument count.
     * @param argv[char**] - Argument values.
     * @param shortArg[const std::string&] - Short option (e.g., "-h").
     * @param longArg[const std::string&] - Long option (e.g., "--threshold").
     * @param outValue[double&] - Output double value to set.
     * @param required[bool] - True if this option is required.
     * @throws MIAException - Thrown if the option is not found or the value is not a valid double.
     */
    void parseDoubleOption(int argc, char* argv[], 
                           const std::string& shortArg,
                           const std::string& longArg,
                           double& outValue,
                           bool required = false);

    /**
     * Parses a string option from command-line arguments.
     * @param argc[int] - Argument count.
     * @param argv[char**] - Argument values.
     * @param shortArg[const std::string&] - Short option (e.g., "-o").
     * @param longArg[const std::string&] - Long option (e.g., "--output").
     * @param outValue[std::string&] - Output string value to set.
     * @param required[bool] - True if this option is required.
     * @throws MIAException - Thrown if the option is not found.
     */
    void parseStringOption(int argc, char* argv[], 
                          const std::string& shortArg,
                          const std::string& longArg,
                          std::string& outValue,
                          bool required = false);
                          
} // namespace command_parser
