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
    /**
     * This namespace defines internal helper methods which are meant to be used only by this file. 
     */
    namespace internal
    {
        /**
         * This is a helper method to throw an exception when required is true. Does nothing otherwise.
         * @param required[bool] - Whether the option is required or not.
         * @param longArg[const std::string&] - Long option (e.g., "--debug").
         * @param typeName[const std::string&] - A string describing the type to throw an error about.
         */
        void checkRequired(bool required,
                           const std::string& longArg,
                           const std::string& typeName)
        {
            if (required)
            {
                std::string err = typeName + std::string(" option ") + longArg + std::string(" not found");
                throw error::MIAException(error::ErrorCode::Missing_Argument, err);
            }
        }
    }

    void parseBoolFlag(int argc, char* argv[],
                       const std::string& shortArg,
                       const std::string& longArg,
                       bool& outValue)
    {
        for (int i = 1; i < argc; ++i)
        {
            if ( (shortArg != "" && argv[i] == shortArg) || argv[i] == longArg)
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
            if ( (shortArg != "" && argv[i] == shortArg) || argv[i] == longArg)
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

        internal::checkRequired(required, longArg, "Integer");
    }


    void parseUnsignedIntOption(int argc, char* argv[],
                                const std::string& shortArg,
                                const std::string& longArg,
                                unsigned int& outValue,
                                bool required)
    {
        for (int i = 1; i < argc - 1; ++i)
        {
            if ( (shortArg != "" && argv[i] == shortArg) || argv[i] == longArg)
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

        internal::checkRequired(required, longArg, "Unsigned Integer");
    }


    void parseDoubleOption(int argc, char* argv[],
                           const std::string& shortArg,
                           const std::string& longArg,
                           double& outValue,
                           bool required)
    {
        for (int i = 1; i < argc - 1; ++i)
        {
            if ( (shortArg != "" && argv[i] == shortArg) || argv[i] == longArg)
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

        internal::checkRequired(required, longArg, "Double");
    }


    void parseStringOption(int argc, char* argv[],
                           const std::string& shortArg,
                           const std::string& longArg,
                           std::string& outValue,
                           bool required)
    {
        for (int i = 1; i < argc - 1; ++i)
        {
            if ( (shortArg != "" && argv[i] == shortArg) || argv[i] == longArg)
            {
                outValue = argv[i + 1];
                return;
            }
        }

        internal::checkRequired(required, longArg, "String");
    }
} // namespace command_parser
