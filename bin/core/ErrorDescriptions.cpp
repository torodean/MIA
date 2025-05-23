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
        { MIAConfig_File_Not_Found, "MIAConfig file not found." },
        { FATAL_File_Not_Found, "FATAL: File not found." },
        { Feature_In_Dev, "Feature still in development." },
        { Windows_Only_Feature, "This feature is only implemented for Windows." },
        { Invalid_MIAConfig_Option, "Invalid option in MIAConfig." },
        { Nothing_set_For_Testing, "Nothing set for testing." },
        { Issue_Setting_MIA_Variable, "Issue setting MIA variable." },
        { Issue_With_Sequence_File, "Issue with MIA sequence file." },
        { MIA_Sequence_File_Not_Found, "MIA sequence file not found." },
        { Invalid_Sequence_Option, "Invalid option in MIASequences." },
        { Cannot_Find_Mapped_Value, "Error finding mapped value." },
        { Invalid_Character_Input, "Invalid character input." },
        { Linux_Only_Feature, "This feature is only implemented for Linux." },
        { Database_Features_Not_Built, "Database features are not built." },
        { Database_Connection_Failure, "Failed to connect to the database." },
        { Invalid_Argument, "Invalid application/command argument." },
        { Missing_Argument, "The app/command argument was not found." },
        { Failed_To_Open_File, "Failed to open file." },
        { Catastrophic_Failure, "A catastrophic failure occurred." }
    };

    const std::string& getErrorDescription(ErrorCode code)
    {
        static const std::string unknown = "Unknown error.";
        auto it = errorDescriptions.find(code);
        return it != errorDescriptions.end() ? it->second : unknown;
    }
}
