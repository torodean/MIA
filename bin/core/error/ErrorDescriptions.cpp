/**
 * @file ErrorDescriptions.cpp
 * @author Antonius Torode
 * @date 5/20/2025
 * @brief Contains the mapping and lookup logic for error code descriptions.
 */

#include "ErrorDescriptions.hpp"

namespace error
{
    const std::unordered_map<ErrorCode, std::string> errorDescriptions = {

        /*
         * Program and System level errors. 
         */
        { MIA_Success, "The operation completed successfully." },
        { Access_denied, "Access denied." },
        { Gen_Failure, "A device attached to the system is not functioning." },
        { Bad_Net_Path, "The network path was not found." },
        { Invalid_Parameter, "The parameter is incorrect." },
        { Invalid_Level, "The system call level is not correct." },
        { File_Not_Found, "File not found." },
        { Logon_Failure, "The user name or password is incorrect." },
        { RPC_S_Server_Unavailable, "The RPC server is unavailable." },
        { NErr_User_Not_Found, "The user name could not be found." },
        { No_Browser_Servers_Found, "The list of servers for this workgroup is not currently available." }, 

        /*
         * MIA Program/application specific errors.
         */
        { Feature_In_Dev, "Feature still in development." },
        { Nothing_set_For_Testing, "Nothing set for testing." },
        { Issue_Setting_MIA_Variable, "Issue setting MIA variable." },
        { Cannot_Find_Mapped_Value, "Error finding mapped value." },
        { Invalid_Character_Input, "Invalid character input." },
        { Invalid_Argument, "Invalid application/command argument." },
        { Missing_Argument, "The app/command argument was not found." },
        { Invalid_Type_Requested, "Invalid type for template argument." },
        { Out_Of_Order_Method_Calls, "A sequence of methods was called out of order." },

        /*
         * Operating-system-specific related Errors.
         * 315XX error codes.
         */        
        { Windows_Only_Feature, "This feature is only implemented for Windows." },
        { Linux_Only_Feature, "This feature is only implemented for Linux." },
        { OS_Not_Supported, "This feature is not supported on this OS." },

        /*
         * Configuration error codes. 316XX.
         */
        { Config_File_Not_Found, "The configuration file was not found." },
        { Config_File_Not_Set, "The configuration file was never set." },
        { Config_Missing_Key, "A required configuration key was not found." },
        { Config_Invalid_Option, "An option in a configuration file was not recognized." },
        { Config_Invalid_Value, "A configuration value was not valid for its setting." },
        { Config_Load_Failure, "The application failed to load its configuration." },
        { Config_Invalid_Call, "The configuration API was called incorrectly." },

        /*
         * File-specific error codes. 317XX.
         */
        { Required_File_Not_Found, "A required file was not found." },
        { File_Open_Failure, "Failed to open a file." },

        /*
         * VLC-related errors. 361XX
         */
        {VLC_Failed_To_Create_Instance, "Failed to create an instance of libvlc."},
        {VLC_Failed_To_Create_Media, "Failed to create media object for libvlc."},
        {VLC_Failed_To_Create_Player, "Failed to create player object for libvlc."},
        {VLC_Failed_To_Play_Audio, "Failed to play audio using libvlc."},

        /*
         * JSON related errors. 362XX
         */
        {JSON_Failed_To_Parse_String, "Failed to parse a json string."},
        {JSON_Key_Not_Found, "The json key was not found."},
        {JSON_Key_Not_Array_Type, "The json key was not the expected array type."},

        /*
         * RPG error codes. 371XX
         */
        { Undefined_RPG_Value, "A referenced value for the RPG system wasn't defined." },
        { Duplicate_RPG_Value, "A duplicate RPG entry was added." },
        { Invalid_RPG_Data, "An invalid combination/set of RPG data was used." },
        { Serialization_Key_Not_Found, "A Serialization key was not found." },
        { Exceeded_RPG_Quantity, "An RPG quantity exceeded the maximum storable value." },
        { Insufficient_RPG_Quantity, "An RPG quantity removal exceeded the available amount." },

        /*
         * Database error codes. 38XXX
         */
        { Database_Features_Not_Built, "Database features are not built." },
        { Database_Connection_Failure, "Failed to connect to the database." },

        /*
         * Python error codes. 39XXX
         */
        { Py_Module_Load_Failure, "A Python module failed to load in the embedded interpreter." },
        { Py_Unexp_Return_Type_Request, "The expected return value requested for a python call does not match." },
        { Py_Module_Call_Error, "A call to a module method returned an error." },

        /*
         * CHECK app error codes. 400XX.
         */
        { Check_No_Piece_On_Square, "No piece was on the specified square." },
        { Check_Malformed_Event, "A backend-ui event appears to be malformed." },
        { Check_Invalid_Start_Pos, "An invalid starting position was configured." },

        // A catch all for any un-programmed errors.
        { Catastrophic_Failure, "A catastrophic failure occurred." }
    };

    const std::string& getErrorDescription(ErrorCode code)
    {
        static const std::string unknown = "Unknown error.";
        auto it = errorDescriptions.find(code);
        return it != errorDescriptions.end() ? it->second : unknown;
    }
}
