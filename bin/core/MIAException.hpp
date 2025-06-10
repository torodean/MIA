/**
 * @file MIAException.hpp
 * @author Antonius Torode
 * @date 5/20/2025
 * @brief Declares the MIAException class for structured MIA error reporting.
 *
 * This header defines the MIAException class, a custom exception type that encapsulates
 * an ErrorCode along with a descriptive message. It integrates with standard C++ exception
 * handling and uses the error::getErrorDescription function to generate human-readable
 * explanations for known error codes. This class is intended to provide consistent,
 * structured error propagation across the MIA application.
 */
#pragma once

#include <exception>
#include <string>

#include "Error.hpp"
#include "ErrorDescriptions.hpp"

namespace error
{
    /**
     * @brief Custom exception class for MIA-related errors.
     * Encapsulates an ErrorCode and descriptive message. Inherits from std::exception
     * to integrate with standard exception handling mechanisms.
     */
    class MIAException : public std::exception
    {
    public:
        /**
         * @brief Constructs a MIAException with the given error code and message.
         * @param code The ErrorCode indicating the type of error.
         * @param details A descriptive message explaining the details of the error (default = "").
         */
        MIAException(ErrorCode code, const std::string& details = "")
            : errorCode(code), errorDetails(details) 
        { 
            cachedWhatMessage = getErrorDescription(code) + (details.empty() ? "" : (" " + details));
        }
    
        /**
         * @brief Returns a C-style character string describing the error.
         * @return The error message as a null-terminated C string.
         */
        const char* what() const noexcept override
        { return cachedWhatMessage.c_str(); }
    
        /**
         * @brief Returns the associated ErrorCode for this exception.
         * @return The ErrorCode value representing the error.
         */
        ErrorCode getCode() const noexcept
        { return errorCode; }
    
    private:
        std::string generateFullErrorMsg()
        { return getErrorDescription(errorCode) + " " + errorDetails; }

        /// The specific error code associated with this exception.
        ErrorCode errorCode;
        /// Descriptive error message explaining details not included with the specific error code.
        std::string errorDetails;
        /// Cached full message used in what()
        std::string cachedWhatMessage;
    }; // class MIAException
} // namespace error


/**
 * @def MIA_THROW
 * @brief Throws a MIAException with contextual debugging information.
 *
 * This macro simplifies exception throwing by automatically appending the file name,
 * line number, and function name to the provided error message. It wraps construction
 * and throwing of a `MIAException` with a given `ErrorCode` and optional message string.
 * The macro ensures consistent error reporting across the application, helping to
 * quickly identify the origin of thrown exceptions during debugging or logging.
 *
 * @param code The ErrorCode representing the type of error.
 * @param ... (Optional) Additional error message details. If omitted, only context information is included.
 *
 * @note Internally wraps its logic in a `do { ... } while(0)` block to ensure safe usage in any scope.
 *
 * @example
 * @code
 * if (someFailureCondition) {
 *     MIA_THROW(ErrorCode::InvalidInput, "Input value was null");
 * }
 * @endcode
 */
#define MIA_THROW(code, ...)                                                                   \
    do {                                                                                       \
        std::string _msg = std::string(#__VA_ARGS__).empty() ? "" : std::string(__VA_ARGS__);  \
        _msg += " [at " + std::string(__FILE__) + ":" + std::to_string(__LINE__)               \
                    + " in " + std::string(__func__) + "]";                                    \
        throw error::MIAException((code), _msg);                                               \
    } while (0)
