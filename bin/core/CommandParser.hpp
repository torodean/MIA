/**
 * @file CommandParser.hpp
 * @author Antonius Torode
 * @date 05/22/2025
 * @brief Contains various utilities for parsing command options.
 */
#pragma once

#include <string>

namespace command_parser
{
    /**
     * @brief Parses a boolean flag from command-line arguments.
     *
     * @param argc Argument count.
     * @param argv Argument values.
     * @param shortArg Short option (e.g., "-v").
     * @param longArg Long option (e.g., "--verbose").
     * @param outValue Output boolean value to set.
     * @throws MIAException if the option is specified more than once.
     */
    void parseBoolFlag(int argc, char* argv[], 
                       const std::string& shortArg,
                       const std::string& longArg,
                       bool& outValue);

    /**
     * @brief Parses an integer option from command-line arguments.
     *
     * @param argc Argument count.
     * @param argv Argument values.
     * @param shortArg Short option (e.g., "-t").
     * @param longArg Long option (e.g., "--threads").
     * @param outValue Output integer value to set.
     * @param required If this option is required or not when parsing.
     * @throws MIAException if the option is not found, the value is not a valid integer,
     *         or the option is specified more than once.
     */
    void parseIntOption(int argc, char* argv[], 
                        const std::string& shortArg,
                        const std::string& longArg,
                        int& outValue,
                        bool required = false);

    /**
     * @brief Parses an unsigned integer option from command-line arguments.
     *
     * @param argc Argument count.
     * @param argv Argument values.
     * @param shortArg Short option (e.g., "-d").
     * @param longArg Long option (e.g., "--debug").
     * @param outValue Output unsigned integer value to set.
     * @param required If this option is required or not when parsing.
     * @throws MIAException if the option is not found, the value is not a valid integer,
     *         or the option is specified more than once.
     */
    void parseUnsignedIntOption(int argc, char* argv[], 
                                const std::string& shortArg,
                                const std::string& longArg,
                                unsigned int& outValue,
                                bool required = false);

    /**
     * @brief Parses a double option from command-line arguments.
     *
     * @param argc Argument count.
     * @param argv Argument values.
     * @param shortArg Short option (e.g., "-h").
     * @param longArg Long option (e.g., "--threshold").
     * @param outValue Output double value to set.
     * @param required If this option is required or not when parsing.
     * @throws MIAException if the option is not found, the value is not a valid double,
     *         or the option is specified more than once.
     */
    void parseDoubleOption(int argc, char* argv[], 
                           const std::string& shortArg,
                           const std::string& longArg,
                           double& outValue,
                           bool required = false);

    /**
     * @brief Parses a string option from command-line arguments.
     *
     * @param argc Argument count.
     * @param argv Argument values.
     * @param shortArg Short option (e.g., "-o").
     * @param longArg Long option (e.g., "--output").
     * @param outValue Output string value to set.
     * @param required If this option is required or not when parsing.
     * @throws MIAException if the option is not found or is specified more than once.
     */
    void parseStringOption(int argc, char* argv[], 
                           const std::string& shortArg,
                           const std::string& longArg,
                           std::string& outValue,
                           bool required = false);

} // namespace command_parser
