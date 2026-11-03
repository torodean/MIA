/**
 * @file Error.hpp
 * @author Antonius Torode
 * @date 03/01/2021
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
        /*
         * Program and System level errors. These are errors which are specific to
         * the program flow or system connections. Most of these try to use the same
         * code values as the system would use.
         */
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

        /*
         * MIA Program/application specific errors. These are errors specific to MIA's
         * application design and features which are not fitting in a more specific
         * category. 31XXX
         */
        Feature_In_Dev = 31415,               ///< This feature is not finished.
        Nothing_set_For_Testing = 31418,      ///< Nothing set for testing.
        Issue_Setting_MIA_Variable = 31419,   ///< Issue setting MIA variable.
        Cannot_Find_Mapped_Value = 31423,     ///< Some map does not contain the map searched for.
        Invalid_Character_Input = 31424,      ///< There was an invalid character input.
        Invalid_Argument = 31428,             ///< Invalid application/command argument.
        Missing_Argument = 31429,             ///< The app/command argument was not found.
        Invalid_Type_Requested = 31432,       ///< Invalid type for template argument.
        Out_Of_Order_Method_Calls = 31434,    ///< A sequence of methods was called out of order.

        /*
         * Operating-system-specific related Errors. These define errors which are relevant to 
         * the os-specific features, and features being developed differently on different OS's.
         * 315XX error codes.
         */
        Linux_Only_Feature = 31500,           ///< This feature is only supported on Linux.
        Windows_Only_Feature = 31501,         ///< Windows only feature.
        OS_Not_Supported = 31502,             ///< Unsupported OS for this feature.

        /*
         * Configuration error codes. 316XX. These cover failures while loading or
         * interpreting configuration files, including MIA sequence files.
         */
        Config_File_Not_Found = 31600,        ///< The configuration file was not found.
        Config_File_Not_Set = 31601,          ///< The configuration file was never set.
        Config_Missing_Key = 31602,           ///< A required configuration key was not found.
        Config_Invalid_Option = 31603,        ///< An option in a configuration file was not recognized.
        Config_Invalid_Value = 31604,         ///< A configuration value was not valid for its setting.
        Config_Load_Failure = 31605,          ///< The application failed to load its configuration.
        Config_Invalid_Call = 31606,          ///< The configuration API was called incorrectly.

        /*
         * File-specific error codes. 317XX.
         */
        Required_File_Not_Found = 31700,      ///< A required file was not found.
        File_Open_Failure = 31701,            ///< Failed to open a file.

        /*
         * VLC-related errors. 361XX
         */
        VLC_Failed_To_Create_Instance = 36100,///< Failed to create an instance of libvlc.
        VLC_Failed_To_Create_Media = 36101,   ///< Failed to create media object for libvlc.
        VLC_Failed_To_Create_Player = 36102,  ///< Failed to create player object for libvlc.
        VLC_Failed_To_Play_Audio = 36103,     ///< Failed to play audio using libvlc.

        /*
         * JSON related errors. 362XX
         */
        JSON_Failed_To_Parse_String = 36200,  ///< Failed to parse a json string.
        JSON_Key_Not_Found = 36201,           ///< The json key was not found.
        JSON_Key_Not_Array_Type = 36202,      ///< The json key was not the expected array type.

        /*
         * RPG error codes. 371XX
         */
        Undefined_RPG_Value = 37100,          ///< A referenced value for the RPG system wasn't defined.
        Duplicate_RPG_Value = 37101,          ///< A duplicate RPG entry was added.
        Invalid_RPG_Data = 37102,             ///< An invalid combination/set of RPG data was used.
        Serialization_Key_Not_Found = 37103,  ///< A Serialization key was not found.
        Exceeded_RPG_Quantity = 37104,        ///< An RPG quantity exceeded the maximum storable value.
        Insufficient_RPG_Quantity = 37105,    ///< An RPG quantity removal exceeded the available amount.

        /*
         * Database error codes. 38XXX
         */ 
        Database_Features_Not_Built = 38000,  ///< Database is not setup.
        Database_Connection_Failure = 38001,  ///< Failed to connect to the database.

        /*
         * Python (embedded interpreter) error codes. 39XXX
         */
        Py_Module_Load_Failure = 39000,       ///< A Python module failed to load in the embedded interpreter.
        Py_Unexp_Return_Type_Request = 39001, ///< The expected return value requested for a python call does not match.
        Py_Module_Call_Error = 39002,         ///< A call to a module method returned an error.

        /*
         * CHECK app error codes. 400XX.
         */
        Check_No_Piece_On_Square = 40000,     ///< No piece was on the specified square.
        Check_Malformed_Event = 40001,        ///< A backend-ui event appears to be malformed.
        Check_Invalid_Start_Pos = 40002,      ///< An invalid starting position was configured.

        // A catch all for any un-programmed errors.
        Catastrophic_Failure = 9001           ///< Some unprogrammed error occured.
    };

    /**
     * Returns an error code witha a brief descriptive message.
     * @param errorCode The integer error code to return.
     * @param details A string with optional descriptive error info (default = "").
     */
    [[deprecated("Use MIAException instead!")]]
    int returnError(int errorCode = 9001, const std::string& details = "");

    /**
     * Prints information about a specified known error or all of the known errors.
     * @param error The error to return information about.
     */
    void errorInfo(int error);

    /**
     * Returns information about a specific error code.
     * @param error The error to return information about.
     */
    std::string errorInfo(ErrorCode error);

    /**
     * Inquires for an error code to gather info about.
     * @param all option to print all error info (default = false).
     */
    void errorInfoRun(bool all = false);

} // namespace error
