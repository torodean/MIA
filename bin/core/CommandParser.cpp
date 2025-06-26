/**
 * File: CommandParser.cpp
 * Author: Antonius Torode
 * Creation Date: 05/22/2025
 * Description: This file implements the utilities for parsing command options.
 */
 
#include <string>
#include <stdexcept>
#include <cstring>
#include <cstdlib>

// Used for exception throws.
#include "MIAException.hpp"
// The associated header file with the method definitions.
#include "CommandParser.hpp"

namespace command_parser
{
    void parseBoolFlag(int argc, char* argv[],
                       const std::string& shortArg,
                       const std::string& longArg,
                       bool& outValue)
    {
        for (int i = 1; i < argc; ++i)
        {
            if (argv[i] == shortArg || argv[i] == longArg)
            {
                outValue = true;
                return;
            }
        }
        outValue = false;
    }


    void parseIntOption(int argc, char* argv[],
                        const std::string& shortArg,
                        const std::string& longArg,
                        int& outValue,
                        bool required)
    {
        for (int i = 1; i < argc - 1; ++i)
        {
            if (argv[i] == shortArg || argv[i] == longArg)
            {
                char* end = nullptr;
                long val = std::strtol(argv[i + 1], &end, 10);
                if (*end != '\0')
                {
                    std::string err = std::string("Invalid integer value for ") + argv[i];
                    throw error::MIAException(error::ErrorCode::Invalid_Argument, err);
                }
                outValue = static_cast<int>(val);
                return;
            }
        }
        
        if (required)
        {
            std::string err = std::string("Integer option ") + longArg + std::string(" not found");
            throw error::MIAException(error::ErrorCode::Missing_Argument, err);
        }
    }


    void parseUnsignedIntOption(int argc, char* argv[],
                                const std::string& shortArg,
                                const std::string& longArg,
                                unsigned int& outValue,
                                bool required)
    {
        for (int i = 1; i < argc - 1; ++i)
        {
            if (argv[i] == shortArg || argv[i] == longArg)
            {
                char* end = nullptr;
                long val = std::strtol(argv[i + 1], &end, 10);
                if (*end != '\0')
                {
                    std::string err = std::string("Invalid unsigned integer value for ") + argv[i];
                    throw error::MIAException(error::ErrorCode::Invalid_Argument, err);
                }
                outValue = static_cast<unsigned int>(val);
                return;
            }
        }
        
        if (required)
        {
            std::string err = std::string("Integer option ") + longArg + std::string(" not found");
            throw error::MIAException(error::ErrorCode::Missing_Argument, err);
        }
    }


    void parseDoubleOption(int argc, char* argv[],
                           const std::string& shortArg,
                           const std::string& longArg,
                           double& outValue,
                           bool required)
    {
        for (int i = 1; i < argc - 1; ++i)
        {
            if (argv[i] == shortArg || argv[i] == longArg)
            {
                char* end = nullptr;
                double val = std::strtod(argv[i + 1], &end);
                if (*end != '\0')
                {
                    std::string err = std::string("Invalid double value for ") + argv[i];
                    throw error::MIAException(error::ErrorCode::Invalid_Argument, err);
                }
                outValue = val;
                return;
            }
        }

        if (required)
        {
            std::string err = std::string("Double option ") + longArg + std::string(" not found");
            throw error::MIAException(error::ErrorCode::Missing_Argument, err);
        }
    }


    void parseStringOption(int argc, char* argv[],
                           const std::string& shortArg,
                           const std::string& longArg,
                           std::string& outValue,
                           bool required)
    {
        for (int i = 1; i < argc - 1; ++i)
        {
            if (argv[i] == shortArg || argv[i] == longArg)
            {
                outValue = argv[i + 1];
                return;
            }
        }

        if (required)
        {
            std::string err = std::string("String option ") + longArg + std::string(" not found");
            throw error::MIAException(error::ErrorCode::Missing_Argument, err);
        }
    }
} // namespace command_parser
