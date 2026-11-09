/**
 * @file CommandParser.cpp
 * @author Antonius Torode
 * @date 05/22/2025
 * @brief Implements the utilities for parsing command options.
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
    namespace
    {
        /**
         * @brief Throws an exception when required is true. Does nothing otherwise.
         *
         * @param required Whether the option is required or not.
         * @param longArg Long option (e.g., "--debug").
         * @param typeName A string describing the type to throw an error about.
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


        /**
         * @brief Counts how many times an option's short or long form appears in the arguments.
         *
         * @param argc Argument count.
         * @param argv Argument values.
         * @param shortArg Short option (e.g., "-v"). An empty string matches no short form.
         * @param longArg Long option (e.g., "--verbose").
         * @return int The number of times the option appears in the arguments.
         */
        int countOptionOccurrences(int argc, char* argv[],
                                   const std::string& shortArg,
                                   const std::string& longArg)
        {
            int occurrences = 0;

            for (int i = 1; i < argc; ++i)
            {
                if ( (shortArg != "" && argv[i] == shortArg) || argv[i] == longArg)
                    ++occurrences;
            }

            return occurrences;
        }


        /**
         * @brief Throws an exception when a command option appears more than once. Does nothing otherwise.
         *
         * @param occurrences The number of times the option appears in the arguments.
         * @param longArg Long option (e.g., "--debug").
         */
        void checkDuplicateOption(int occurrences, const std::string& longArg)
        {
            if (occurrences > 1)
            {
                std::string err = "Duplicate command option " + longArg + " specified";
                throw error::MIAException(error::ErrorCode::Duplicate_Argument, err);
            }
        }
    } // namespace


    void parseBoolFlag(int argc, char* argv[],
                       const std::string& shortArg,
                       const std::string& longArg,
                       bool& outValue)
    {
        int occurrences = countOptionOccurrences(argc, argv, shortArg, longArg);

        checkDuplicateOption(occurrences, longArg);

        outValue = occurrences > 0;
    }


    void parseIntOption(int argc, char* argv[],
                        const std::string& shortArg,
                        const std::string& longArg,
                        int& outValue,
                        bool required)
    {
        int occurrences = countOptionOccurrences(argc, argv, shortArg, longArg);

        checkDuplicateOption(occurrences, longArg);

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

        checkRequired(required, longArg, "Integer");
    }


    void parseUnsignedIntOption(int argc, char* argv[],
                                const std::string& shortArg,
                                const std::string& longArg,
                                unsigned int& outValue,
                                bool required)
    {
        int occurrences = countOptionOccurrences(argc, argv, shortArg, longArg);

        checkDuplicateOption(occurrences, longArg);

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

        checkRequired(required, longArg, "Unsigned Integer");
    }


    void parseDoubleOption(int argc, char* argv[],
                           const std::string& shortArg,
                           const std::string& longArg,
                           double& outValue,
                           bool required)
    {
        int occurrences = countOptionOccurrences(argc, argv, shortArg, longArg);

        checkDuplicateOption(occurrences, longArg);

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

        checkRequired(required, longArg, "Double");
    }


    void parseStringOption(int argc, char* argv[],
                           const std::string& shortArg,
                           const std::string& longArg,
                           std::string& outValue,
                           bool required)
    {
        int occurrences = countOptionOccurrences(argc, argv, shortArg, longArg);

        checkDuplicateOption(occurrences, longArg);

        for (int i = 1; i < argc - 1; ++i)
        {
            if ( (shortArg != "" && argv[i] == shortArg) || argv[i] == longArg)
            {
                outValue = argv[i + 1];
                return;
            }
        }

        checkRequired(required, longArg, "String");
    }
} // namespace command_parser
