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
     * A list of error codes that can be returned. Some of these are mapped to typical
     * error codes for consistency. The ones that are specific to MIA (or yet to be 
     * mapped) begin at 31415 (int(pi*10000)) and increment. When values are added to
     * this enum, a description should also be added to ErrorDescriptions.hpp.
     *
     * @note Typically, constants within an enum are all caps. This one is an exception
     * so that acronyms and other specific information can be preserved.
     */
    enum ErrorCode
    {
        MIA_Success = 0,                      ///< Success - no error!
        Access_denied = 5,                    ///< Access denied when attempting an action.
        Gen_Failure = 31,                     ///< A device attached to the system is not functioning.
        Bad_Net_Path = 53,                    ///< The network path was not found.
        Invalid_Parameter = 87,               ///< A Parameter to a method was invalid.
        Invalid_Level = 124,                  ///< The system call level is not correct.
        File_Not_Found = 404,                 ///< A File was not found.
        Logon_Failure = 1326,                 ///< The user name or password is incorrect.
        RPC_S_Server_Unavailable = 1722,      ///< The RPC server is unavailable.
        NErr_User_Not_Found = 2221,           ///< The user name could not be found.
        No_Browser_Servers_Found = 6118,      ///< Unavailable list of servers for this workgroup.
        MIAConfig_File_Not_Found = 31403,     ///< The configuration file was not found.
        FATAL_File_Not_Found = 31404,         ///< A needed file was not found.
        Feature_In_Dev = 31415,               ///< This feature is not finished.
        Windows_Only_Feature = 31416,         ///< Windows only feature.
        Invalid_MIAConfig_Option = 31417,     ///< Invalid option in MIAConfig.
        Nothing_set_For_Testing = 31418,      ///< Nothing set for testing.
        Issue_Setting_MIA_Variable = 31419,   ///< Issue setting MIA variable.
        Issue_With_Sequence_File = 31420,     ///< Issue with MIA sequence file.
        MIA_Sequence_File_Not_Found = 31421,  ///< MIA sequence file not found.
        Invalid_Sequence_Option = 31422,      ///< Invalid option in MIASequences.
        Cannot_Find_Mapped_Value = 31423,     ///< Some map does not contain the map searched for.
        Invalid_Character_Input = 31424,      ///< There was an invalid character input.
        Linux_Only_Feature = 31425,           ///< This feature is only supported on Linux.
        Database_Features_Not_Built = 31426,  ///< Database is not setup.
        Database_Connection_Failure = 31427,  ///< Failed to connect to the database.
        Invalid_Argument = 31428,             ///< Invalid application/command argument.
        Missing_Argument = 31429,             ///< The app/command argument was not found.
        Config_File_Not_Set = 31430,          ///< Config File not set.
        Failed_To_Open_File = 31431,          ///< Failed to open a file.
        Invalid_Type_Requested = 31432,       ///< Invalid type for template argument.
        Invalid_Configuration_Call = 31433,   ///< Invalid configuration call.
        Undefined_RPG_Value = 31434,          ///< A referenced value for the RPG system wasn't defined.
        Duplicate_RPG_Value = 31435,          ///< A duplicate RPG entry was added.
        Invalid_RPG_Data = 31436,             ///< An invalid combination/set of RPG data was used. 
        Catastrophic_Failure = 9001           ///< Some unprogrammed error occured.
    };

    /**
     * Returns an error code witha a brief descriptive message.
     * @param errorCode[int] - The integer error code to return.
     * @param details[std::string&] - A string with optional descriptive error info (default = "").
     */
    [[deprecated("Use MIAException instead!")]]
    int returnError(int errorCode = 9001, const std::string& details = "");

    /**
     * Prints information about a specified known error or all of the known errors.
     * @param error[int] - The error to return information about.
     */
    void errorInfo(int error);

    /**
     * Returns information about a specific error code.
     * @param error[ErrorCode] - The error to return information about.
     */
    std::string errorInfo(ErrorCode error);

    /**
     * Inquires for an error code to gather info about.
     * @param all[bool] - option to print all error info (default = false).
     */
    void errorInfoRun(bool all = false);

} // namespace error
