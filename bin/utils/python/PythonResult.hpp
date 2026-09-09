/**
 * @file PythonResult.hpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Declares the result type returned by PythonModule calls.
 */
#pragma once

#include <string>


/**
 * A result from a call into a Python module method.
 * This wraps the converted return value of the Python method along with the
 * error state of the call, so callers never handle raw PyObject pointers.
 */
class PythonResult
{
public:

    /// The type of value a Python method call produced.
    enum class Type
    {
        Void,       ///< The method returned None (or nothing usable).
        Integer,    ///< The method returned a Python int.
        Double,     ///< The method returned a Python float.
        String      ///< The method returned a Python str.
    }; // enum class Type

    /**
     * Constructs a successful void result.
     * Used for methods which return nothing usable to C++.
     */
    PythonResult();

    /**
     * Constructs a successful result holding an integer value.
     *
     * @param value The integer returned by the Python method.
     */
    explicit PythonResult(long value);

    /**
     * Constructs a successful result holding a double value.
     *
     * @param value The double returned by the Python method.
     */
    explicit PythonResult(double value);

    /**
     * Constructs a successful result holding a string value.
     *
     * @param value The string returned by the Python method.
     */
    explicit PythonResult(const std::string& value);

    /**
     * Constructs a successful result holding a string value.
     * This overload lets string literals build a String result directly.
     *
     * @param value The string returned by the Python method.
     */
    explicit PythonResult(const char* value);

    /**
     * Constructs a failed result with an error description.
     * This is a named factory rather than a constructor so that a call like
     * PythonResult("text") can never be mistaken for an error: constructing
     * from any string always means success, and only error() means failure.
     *
     * @param errorMessage A description of what went wrong during the call.
     * @return An invalid PythonResult carrying the error description.
     */
    static PythonResult error(const std::string& errorMessage);

    /// Default destructor.
    ~PythonResult() = default;

    /**
     * Checks whether the call succeeded and produced a usable value.
     *
     * @return True if the call succeeded, false otherwise.
     */
    bool isValid() const;

    /**
     * Gets the type of value this result holds.
     *
     * @return The Type of the underlying value.
     */
    Type getType() const;

    /**
     * Gets the result as an integer.
     *
     * @return The integer value, or 0 if the result is not an Integer.
     */
    long asInt() const;

    /**
     * Gets the result as a double.
     *
     * @return The double value, or 0.0 if the result is not a Double.
     */
    double asDouble() const;

    /**
     * Gets the result as a string.
     *
     * @return The string value, or an empty string if the result is not a String.
     */
    const std::string& asString() const;

    /**
     * Gets the error description of a failed result.
     *
     * @return The error message, or an empty string if the result is valid.
     */
    const std::string& getError() const;

private:

    /// The type of value held by this result.
    Type type{Type::Void};

    /// The integer value, when type is Integer.
    long intValue{0};

    /// The double value, when type is Double.
    double doubleValue{0.0};

    /// The string value, when type is String.
    std::string stringValue;

    /// The error message, when the result is not valid. Named errorMessage to
    /// avoid shadowing the error() factory within member functions.
    std::string errorMessage;
}; // class PythonResult
