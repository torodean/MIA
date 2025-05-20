/**
 * File: Error.hpp
 * Author: Antonius Torode
 * Created on: 03/01/2021
 * Description: Contains the MIAException class and Error information structures
 *              for handling error codes and exceptions across MIA applications.
 */
#pragma once

#include <exception>
#include <string>

/**
 * This class is for error handling and error code information.
 */
namespace error
{
    /**
     * A list of error codes that can be returned.
     */
    enum ErrorCode
    {
		MIA_Success = 0,
        Access_denied = 5,
        Gen_Failure = 31,
        Bad_Net_Path = 53,
        Invalid_Parameter = 87,
        Invalid_Level = 124,
        File_Not_Found = 404,
        Logon_Failure = 1326,
        RPC_S_Server_Unavailable = 1722,
        NErr_User_Not_Found = 2221,
        No_Browser_Servers_Found = 6118,
        MIAConfig_File_Not_Found = 31403,
        FATAL_File_Not_Found = 31404,
        Feature_In_Dev = 31415,
        Windows_Only_Feature = 31416,
        Invalid_MIAConfig_Option = 31417,
        Nothing_set_For_Testing = 31418,
        Issue_Setting_MIA_Variable = 31419,
        Issue_With_Sequence_File = 31420,
        MIA_Sequence_File_Not_Found = 31421,
        Invalid_Sequence_Option = 31422,
        Cannot_Find_Mapped_Value = 31423,
        Invalid_Character_Input = 31424,
        Linux_Only_Feature = 31425,
        Database_Features_Not_Built = 31426,
        Database_Connection_Failure = 31427,
        Catastrophic_Failure = 9001
    };

    /**
     * Returns an error code witha a brief descriptive message.
     * @param errorCode[int] - The integer error code to return.
     * @param details[std::string&] - A string with optional descriptive error info (default = "").
     */
    [[deprecated("Use MIAException instead!")]]
    static int returnError(int errorCode = 9001, const std::string& details = "");

    /**
     * Prints information about a specified known error or all of the known errors.
     * @param error[int] - The error to return information about.
     */
    [[deprecated("Use MIAException instead!")]]
    static void errorInfo(int error);

    /**
     * Returns information about a specific error code.
     * @param error[ErrorCode] - The error to return information about.
     */
    static std::string errorInfo(ErrorCode error);

    /**
     * Inquires for an error code to gather info about.
     * @param all[bool] - option to print all error info (default = false).
     */
    [[deprecated("Use MIAException instead!")]]
    static void errorInfoRun(bool all = false);

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
            : errorCode(code), errorDetails(details) { }
    
        /**
         * @brief Returns a C-style character string describing the error.
         * @return The error message as a null-terminated C string.
         */
        const char* what() const noexcept override
        { return errorDetails.c_str(); }
    
        /**
         * @brief Returns the associated ErrorCode for this exception.
         * @return The ErrorCode value representing the error.
         */
        ErrorCode code() const noexcept
        { return errorCode; }
    
    private:
        /// The specific error code associated with this exception.
        ErrorCode errorCode;
        /// Descriptive error message explaining details not included with the specific error code.
        std::string errorDetails;
    }; // class MIAException

} // namespace error
