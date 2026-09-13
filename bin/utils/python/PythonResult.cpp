/**
 * @file PythonResult.cpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Implements the result type returned by PythonModule calls.
 */

// The associated header file.
#include "PythonResult.hpp"
// Used for error handling and exception throws.
#include "MIAException.hpp"
#include "Error.hpp"

// Members not named in an init list take their default values from the header.
PythonResult::PythonResult()
{ }


PythonResult::PythonResult(long value)
    : type(Type::Integer),
      intValue(value)
{ }


PythonResult::PythonResult(double value)
    : type(Type::Double),
      doubleValue(value)
{ }


PythonResult::PythonResult(const std::string& value)
    : type(Type::String),
      stringValue(value)
{ }


PythonResult::PythonResult(const std::vector<std::string>& values)
    : type(Type::Strings),
      stringValues(values)
{ }


PythonResult::PythonResult(const char* value)
    : PythonResult(std::string(value))
{ }


PythonResult PythonResult::error(const std::string& errorMessage)
{
    PythonResult result;
    result.errorMessage = errorMessage;
    return result;
}


bool PythonResult::isValid() const
{
    return errorMessage.empty();
}


PythonResult::Type PythonResult::getType() const
{
    return type;
}


long PythonResult::asInt() const
{
    if (type != Type::Integer)
        MIA_THROW(error::ErrorCode::Py_Unexp_Return_Type_Request, 
                  "An integer was requested but not returned by the python module.");
    return intValue;
}


double PythonResult::asDouble() const
{
    if (type != Type::Double)
        MIA_THROW(error::ErrorCode::Py_Unexp_Return_Type_Request, 
                  "A double was requested but not returned by the python module.");
    return doubleValue;
}


const std::string& PythonResult::asString() const
{
    if (type != Type::String)
        MIA_THROW(error::ErrorCode::Py_Unexp_Return_Type_Request, 
                  "A string was requested but not returned by the python module.");
    return stringValue;
}


const std::vector<std::string>& PythonResult::asStrings() const
{
    if (type != Type::Strings)
        MIA_THROW(error::ErrorCode::Py_Unexp_Return_Type_Request, 
                  "A vector of strings was requested but not returned by the python module.");
    return stringValues;
}


const std::string& PythonResult::getError() const
{
    return errorMessage;
}
