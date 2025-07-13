/**
 * @file Error.cpp
 * @author Antonius Torode
 * @date 03/01/2021
 * Description: Defines the behavior of error reporting and lookup utilities,
 *     including support for descriptive messages associated with ErrorCode values.
 */

#include <iostream>
#include <string>

// The corresponding hpp file.
#include "Error.hpp"

// Used for getting the error descriptions in readable formats.
#include "ErrorDescriptions.hpp"

using std::cout;
using std::endl;

namespace error
{
    std::string errorInfo(ErrorCode code) 
    {
        auto it = errorDescriptions.find(code);
        if (it != errorDescriptions.end()) 
            return it->second;
        return "Unknown error code.";
    }


    int returnError(int errorCode, const std::string& details)
    {    
        const std::string& baseMessage = getErrorDescription(static_cast<ErrorCode>(errorCode));
    
        // Print error message with optional appended details
        if (!details.empty())
            cout << "...ERROR " << errorCode << ": " << baseMessage << " " << details << endl;
        else 
            cout << "...ERROR " << errorCode << ": " << baseMessage << endl;
    
        return errorCode;
    }
    
    void errorInfoRun(bool all)
    {
        cout << "..." << endl;
        int error = 0;
        
        if(all)
        {
            errorInfo(-1);
        } 
        else 
        {
            cout << "...Please enter an error code: ";
            std::cin >> error;
            std::cin.ignore();
            errorInfo(error);
        }
    }
    
    void errorInfo(int errorCode)
    {
        cout << "..." << endl;
    
        auto description = getErrorDescription(static_cast<ErrorCode>(errorCode));
        if (!description.empty()) 
        {
            cout << "...ERROR " << errorCode << ": " << description << endl;
        } 
        else 
        {
            cout << "...Invalid or unknown error code entered: " << errorCode << endl;
            cout << "...Full list of programmed error codes follows:" << endl;
            cout << "..." << endl;
    
            for (const auto& [code, desc] : errorDescriptions) 
            {
                cout << "..." << static_cast<int>(code) << ": " << desc << endl;
            }
        }
    }
} // namespace error
