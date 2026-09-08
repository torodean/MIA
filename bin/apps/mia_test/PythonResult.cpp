/**
 * @file PythonResult.cpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Implements the result type returned by PythonModule calls.
 */

// The associated header file.
#include "PythonResult.hpp"


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
    return intValue;
}


double PythonResult::asDouble() const
{
    return doubleValue;
}


const std::string& PythonResult::asString() const
{
    return stringValue;
}


const std::string& PythonResult::getError() const
{
    return errorMessage;
}
