/**
 * @file PythonModule.cpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Implements loading Python modules and calling their methods.
 */

// The associated header file.
#include "PythonModule.hpp"

#include <string>

#include <Python.h>


namespace
{
    /**
     * Extracts the pending Python exception into a C++ string.
     * This clears the Python error indicator, so it is only called when the error
     * has already been handled (or is about to be reported to the caller).
     *
     * @return The exception message, or a fallback message if one can't be read.
     */
    std::string fetchPythonError()
    {
        PyObject* type = nullptr;
        PyObject* value = nullptr;
        PyObject* traceback = nullptr;

        // PyErr_Fetch takes ownership of the exception objects it returns.
        PyErr_Fetch(&type, &value, &traceback);
        PyErr_NormalizeException(&type, &value, &traceback);
        Py_XDECREF(type);
        Py_XDECREF(traceback);

        if (!value)
            return "Unknown Python error.";

        PyObjectPtr valuePtr(value);
        PyObjectPtr message(PyObject_Str(valuePtr.get()));
        if (!message)
        {
            PyErr_Clear();
            return "Unknown Python error.";
        }

        const char* text = PyUnicode_AsUTF8(message.get());
        if (!text)
        {
            PyErr_Clear();
            return "Unknown Python error.";
        }

        return std::string(text);
    }


    /**
     * Converts a PyObject returned by a Python method call into a PythonResult.
     * The type of the Python object decides which PythonResult constructor fits.
     * Any object which is not an int, float, or str (including None) becomes a
     * void result, since the C++ side has no representation for it.
     *
     * @param result The Python object to convert. Must be a new reference owned
     *     by the caller; this function does not release it.
     * @return A PythonResult describing the object.
     */
    PythonResult toResult(PyObject* result)
    {
        if (PyLong_Check(result))
        {
            long value = PyLong_AsLong(result);
            if (PyErr_Occurred())
            {
                PyErr_Clear();
                return PythonResult::error("Python integer result does not fit in a long.");
            }
            return PythonResult(value);
        }

        if (PyFloat_Check(result))
        {
            double value = PyFloat_AsDouble(result);
            if (PyErr_Occurred())
            {
                PyErr_Clear();
                return PythonResult::error("Python float result could not be converted.");
            }
            return PythonResult(value);
        }

        if (PyUnicode_Check(result))
        {
            const char* text = PyUnicode_AsUTF8(result);
            if (!text)
            {
                PyErr_Clear();
                return PythonResult::error("Python string result could not be converted.");
            }
            return PythonResult(text);
        }

        return PythonResult();
    }


    /**
     * Creates a new Python int object from a C++ integer.
     *
     * @param value The value to convert.
     * @return The Python object, or null if creation failed.
     */
    PyObjectPtr toPython(long value)
    {
        return PyObjectPtr(PyLong_FromLong(value));
    }


    /**
     * Creates a new Python float object from a C++ double.
     *
     * @param value The value to convert.
     * @return The Python object, or null if creation failed.
     */
    PyObjectPtr toPython(double value)
    {
        return PyObjectPtr(PyFloat_FromDouble(value));
    }


    /**
     * Creates a new Python str object from a C++ string.
     *
     * @param value The value to convert.
     * @return The Python object, or null if creation failed.
     */
    PyObjectPtr toPython(const std::string& value)
    {
        return PyObjectPtr(PyUnicode_FromString(value.c_str()));
    }


    /**
     * Packs arguments into a Python tuple for a method call.
     * Each argument is a PyObjectPtr. The tuple takes its own reference to every
     * argument, so the arguments passed in keep their references and release them
     * as usual when they go out of scope.
     *
     * If any argument is null (its creation failed) or the tuple cannot be
     * created, the returned pointer is null, which invoke() reports as an error.
     * A call with no arguments produces an empty tuple.
     *
     * @param args Zero or more PyObjectPtr arguments to pack.
     * @return The argument tuple, or null on failure.
     */
    template<typename... Args>
    PyObjectPtr buildArgs(const Args&... args)
    {
        // The fold expression is true when at least one argument is null.
        if ((!args || ...))
            return nullptr;

        return PyObjectPtr(PyTuple_Pack(sizeof...(args), args.get()...));
    }
} // namespace


// The interpreter does not run until the first module is constructed.
int PythonModule::interpreterCount = 0;


void PythonModule::ensureInterpreter()
{
    if (interpreterCount > 0)
        return;

    Py_Initialize();

    /*
     * Add the current working directory to Python's module search path.
     * This allows Python to find modules which sit next to the executable.
     */
    PyRun_SimpleString(
        "import sys\n"
        "sys.path.insert(0, '.')\n"
    );
}


PythonModule::PythonModule(const std::string& moduleName)
    : name(moduleName)
{
    // The interpreter must exist before the module can be imported, and the
    // count must rise before any call can observe it.
    ensureInterpreter();
    ++interpreterCount;

    module.reset(PyImport_ImportModule(moduleName.c_str()));

    /*
     * A failed import is a configuration or deployment problem which every
     * caller would have to handle the same way, so it throws instead of
     * returning a usable object with an unusable handle. The Python traceback
     * is printed first since the exception can only carry the C++-side story.
     */
    if (!module)
    {
        PyErr_Print();
        MIA_THROW(error::ErrorCode::Python_Module_Load_Failure,
                  "Module '" + moduleName + "' could not be imported.");
    }
}


PythonModule::~PythonModule()
{
    // Release the module handle first so no Python object is destroyed after
    // the interpreter has shut down.
    module.reset();

    // Shut the interpreter down only when the last module is gone.
    if (--interpreterCount == 0)
        Py_FinalizeEx();
}


bool PythonModule::hasMethod(const std::string& methodName) const
{
    if (!module)
        return false;

    PyObjectPtr function(PyObject_GetAttrString(module.get(), methodName.c_str()));
    return function && PyCallable_Check(function.get());
}


PythonResult PythonModule::invoke(const std::string& methodName, PyObjectPtr args)
{
    // A null tuple means the caller's argument building failed.
    if (!args)
        return PythonResult::error("Failed to build arguments for '" + methodName + "'.");

    PyObjectPtr function(PyObject_GetAttrString(module.get(), methodName.c_str()));
    if (!function)
        return PythonResult::error("Method '" + methodName + "' not found: " + fetchPythonError());

    if (!PyCallable_Check(function.get()))
        return PythonResult::error("Attribute '" + methodName + "' is not callable.");

    PyObjectPtr result(PyObject_CallObject(function.get(), args.get()));
    if (!result)
        return PythonResult::error("Call to '" + methodName + "' failed: " + fetchPythonError());

    return toResult(result.get());
}


PythonResult PythonModule::call(const std::string& name)
{
    return invoke(name, buildArgs());
}


PythonResult PythonModule::call(const std::string& name, long a, long b)
{
    return invoke(name, buildArgs(toPython(a), toPython(b)));
}


PythonResult PythonModule::call(const std::string& name, double a, double b)
{
    return invoke(name, buildArgs(toPython(a), toPython(b)));
}


PythonResult PythonModule::call(const std::string& name, const std::string& a)
{
    return invoke(name, buildArgs(toPython(a)));
}


PythonResult PythonModule::call(const std::string& name, const std::string& a, long b)
{
    return invoke(name, buildArgs(toPython(a), toPython(b)));
}
