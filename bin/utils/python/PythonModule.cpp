/**
 * @file PythonModule.cpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Implements loading Python modules and calling their methods.
 */

// The associated header file.
#include "PythonModule.hpp"

// Used for resolving the python module directory.
#include "Paths.hpp"

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
} // namespace


void PythonModule::ensureInterpreter()
{
    if (!Py_IsInitialized())
        Py_Initialize();
}


PythonModule::PythonModule(const std::string& moduleName, const std::string& callerFile)
    : name(moduleName)
{
    // The interpreter must exist before the module can be imported.
    ensureInterpreter();

    /*
     * Add the python directory for this construction to Python's module
     * search path. Doing it per construction lets callers in different
     * directories coexist: each one adds the directory holding its own
     * python files.
     */
    std::string setPath =
        "import sys\n"
        "sys.path.insert(0, r'" + paths::getPythonDirToUse(callerFile) + "')\n";
    PyRun_SimpleString(setPath.c_str());

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
