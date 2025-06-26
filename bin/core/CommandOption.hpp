/**
 * @file CommandOption.hpp
 * @author Antonius Torode
 * @date 05/22/2025
 * @brief Defines the CommandOption class for encapsulating a command-line option.
 *
 * This header defines the CommandOption class, which stores a short and long argument
 * along with a human-readable description. It provides methods to display a formatted
 * help string and parse the argument using command_parser utilities, returning typed results.
 */
#pragma once

#include <string>
#include <iostream>
#include <algorithm> // for std::max

// Used for command option parsing
#include "CommandParser.hpp"
//Used for exception handling
#include "MIAException.hpp"

class CommandOption
{
public:

   /**
     * @enum commandOptionType
     * @brief Enumerates the supported data types for command-line options.
     *
     * This enumeration defines the type of value that a CommandOption expects,
     * allowing typed parsing and validation of command-line arguments.
     */
    enum commandOptionType
    {
        boolOption,         ///< bool
        intOption,          ///< int
        unsignedIntOption,  ///< unsigned int
        doubleOption,       ///< double
        stringOption,       ///< std::string
        unknownOption       ///< unknown type
    };
    
    /**
     * Constructs a CommandOption with specified short option, long option, description, and option type.
     * @param shortArg[const std::string&] - Short form of the option (e.g., "-c").
     * @param longArg[const std::string&] - Long form of the option (e.g., "--config").
     * @param description[const std::string&] - Human-readable description of the option.
     * @param type[commandOptionType] - The expected data type for this option (bool, int, double, string).
     * @param required[bool] - Whether this command option is required or not.
     */
    CommandOption(const std::string& shortArg,
                  const std::string& longArg,
                  const std::string& description,
                  commandOptionType type,
                  bool required = false) : 
        shortArg(shortArg), longArg(longArg), description(description), type(type), requiredOption(required)
    { }

    /**
     * Returns the help string for this command option.
     * Format: "  -s, --long    Description"
     * @return std::string - The formatted help string.
     */
    std::string getHelp() const
    {
        const size_t paddingWidth = 24; // total space reserved for shortArg + longArg + spaces
        size_t currentLength = shortArg.size() + 2 /*", "*/ + longArg.size();

        // Calculate spaces needed, at least 2 spaces for readability
        size_t spaces = std::max<size_t>(2, paddingWidth > currentLength ? paddingWidth - currentLength : 2);

        return "  " + shortArg + ", " + longArg + std::string(spaces, ' ') + description;
    }
    
    /**
     * Templated method to retrieve the parsed command-line option value.
     * Dispatches to the correct command_parser method based on the template type.
     * @tparam Type - The expected return type (bool, int, double, or std::string).
     * @param argc[int] - Argument count.
     * @param argv[char**] - Argument values.
     * @param out[Type&] - The parsed and typed value - this is modified by the parsing.
     * @param requiredOption[bool] - Whether or not the method is required when called.
     * @throws MIAException - If parsing fails or the type is unsupported for the option.
     */
    template<typename Type>
    void getOptionVal(int argc, char* argv[], Type& out, bool requiredOption = false) const
    {
        if constexpr (std::is_same_v<Type, bool>)
        {
            if (type != boolOption)
            {
                std::string err = std::string("Expected bool for option: ") + longArg;
                throw error::MIAException(error::ErrorCode::Invalid_Type_Requested, err);
            }
            command_parser::parseBoolFlag(argc, argv, shortArg, longArg, out);
        }
        else if constexpr (std::is_same_v<Type, int>)
        {
            if (type != intOption)
            {
                std::string err = std::string("Expected int for option: ") + longArg;
                throw error::MIAException(error::ErrorCode::Invalid_Type_Requested, err);
            }
            command_parser::parseIntOption(argc, argv, shortArg, longArg, out, requiredOption);
        }
        else if constexpr (std::is_same_v<Type, unsigned int>)
        {
            if (type != unsignedIntOption)
            {
                std::string err = std::string("Expected unsigned int for option: ") + longArg;
                throw error::MIAException(error::ErrorCode::Invalid_Type_Requested, err);
            }
            command_parser::parseUnsignedIntOption(argc, argv, shortArg, longArg, out, requiredOption);
        }
        else if constexpr (std::is_same_v<Type, double>)
        {
            if (type != doubleOption)
            {
                std::string err = std::string("Expected double for option: ") + longArg;
                throw error::MIAException(error::ErrorCode::Invalid_Type_Requested, err);
            }
            command_parser::parseDoubleOption(argc, argv, shortArg, longArg, out, requiredOption);
        }
        else if constexpr (std::is_same_v<Type, std::string>)
        {
            if (type != stringOption)
            {
                std::string err = std::string("Expected string for option: ") + longArg;
                throw error::MIAException(error::ErrorCode::Invalid_Type_Requested, err);
            }
            command_parser::parseStringOption(argc, argv, shortArg, longArg, out, requiredOption);
        }
        else
        {
            std::string err = "Unsupported template type requested";
            throw error::MIAException(error::ErrorCode::Invalid_Type_Requested, err);
        }
    }

private:
    /// Short form of the command-line option (e.g., "-v").
    std::string shortArg;

    /// Long form of the command-line option (e.g., "--verbose").
    std::string longArg;

    /// Human-readable description of the command-line option, shown in help output.
    std::string description;
    
    /// The type of command option value this argument expects.
    commandOptionType type;
    
    // Is this command option required where it is used?
    bool requiredOption{false};
};
