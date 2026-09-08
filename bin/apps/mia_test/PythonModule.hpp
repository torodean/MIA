/**
 * @file PythonModule.hpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Declares a utility for loading Python modules and calling their methods.
 */
#pragma once

#include <memory>
#include <string>

#include <Python.h>

// Used for throwing on module load failure.
#include "MIAException.hpp"
// Used for the ErrorCode of the load failure exception.
#include "Error.hpp"

#include "PythonResult.hpp"


/**
 * Python objects are reference-counted instead of deleted directly: every API
 * which returns a PyObject* hands back a "new reference", and the caller is
 * responsible for releasing it later with Py_DECREF. This struct teaches
 * unique_ptr what "delete" means for such a pointer, since the default delete
 * keyword cannot be used on Python objects.
 *
 * It is a callable type (the operator() makes it behave like a function) rather
 * than a class which owns anything. The unique_ptr holds the pointer and calls
 * this deleter exactly where a raw-pointer implementation would call Py_DECREF.
 * Because the struct has no data members, the unique_ptr costs the same as a
 * raw pointer.
 *
 * Only wrap pointers you own (new references from PyImport_Import,
 * PyObject_GetAttrString, PyObject_CallObject, ...). Some Python APIs return
 * "borrowed references" which are NOT decrefed by the caller; wrapping one of
 * these would release an object the caller never owned.
 */
struct PyObjectDeleter
{
    /**
     * Releases one reference to the object.
     *
     * @param object The object to release. Never null: unique_ptr only calls
     *     the deleter on a non-null pointer, so Py_DECREF is safe here.
     */
    void operator()(PyObject* object) const
    {
        Py_DECREF(object);
    }
}; // struct PyObjectDeleter


/**
 * A unique_ptr for owned Python references.
 * Use this anywhere a PyObject* returned as a new reference needs automatic
 * Py_DECREF when it goes out of scope.
 */
using PyObjectPtr = std::unique_ptr<PyObject, PyObjectDeleter>;


/**
 * A loaded Python module which can be called from C++.
 * This owns both the interpreter lifetime and the imported module handle: the
 * first module constructed initializes the embedded interpreter, and destroying
 * the last module shuts it down. Code using this class never touches the
 * Python C API directly.
 *
 * call() methods invoke Python methods with typed C++ arguments. Each overload
 * covers one argument signature; new signatures are added as they are needed.
 * All calls return a PythonResult, so expected call failures (missing method,
 * Python exception, conversion failure) surface as values without raw pointers
 * or exceptions. An app which wants to treat a call failure as fatal can check
 * isValid() and throw or exit on its own terms.
 *
 * The interpreter reference count is not thread-safe; constructing and
 * destroying modules is assumed to happen on one thread.
 */
class PythonModule
{
public:

    /**
     * Constructs and loads a Python module by name.
     * The module must be reachable on Python's module search path. The
     * embedded interpreter is initialized by this constructor if no other
     * module is currently alive.
     *
     * @param moduleName The name of the Python module to import.
     * @throws error::MIAException with ErrorCode::Python_Module_Load_Failure if
     *     the module could not be imported. An app which prefers to continue
     *     without the module can catch this exception and handle it gracefully.
     */
    explicit PythonModule(const std::string& moduleName);

    /**
     * Destructor. Releases the module handle and, if this is the last module
     * alive, shuts down the embedded interpreter.
     */
    ~PythonModule();

    /*
     * Copying is deleted because a PythonModule owns exactly one reference to
     * a Python module object. If two objects shared that pointer, whichever
     * was destroyed first would release the reference and leave the other
     * holding a handle Python has already freed (a use-after-free on the next
     * call). A copy would also corrupt the interpreter reference count, since
     * one construction would become two live objects. If a module ever needs
     * to be shared, share the PythonModule itself through a shared_ptr so
     * ownership stays with one object.
     */
    PythonModule(const PythonModule&) = delete;
    PythonModule& operator=(const PythonModule&) = delete;

    /**
     * Checks whether the module defines a callable method with the given name.
     *
     * @param name The name of the method to look up.
     * @return True if the method exists and is callable, false otherwise.
     */
    bool hasMethod(const std::string& name) const;

    /**
     * Calls a Python method which takes no arguments.
     *
     * @param name The name of the method to call.
     * @return A PythonResult holding the return value of the call.
     */
    PythonResult call(const std::string& name);

    /**
     * Calls a Python method which takes two integers.
     *
     * @param name The name of the method to call.
     * @param a The first integer argument.
     * @param b The second integer argument.
     * @return A PythonResult holding the return value of the call.
     */
    PythonResult call(const std::string& name, long a, long b);

    /**
     * Calls a Python method which takes two doubles.
     *
     * @param name The name of the method to call.
     * @param a The first double argument.
     * @param b The second double argument.
     * @return A PythonResult holding the return value of the call.
     */
    PythonResult call(const std::string& name, double a, double b);

    /**
     * Calls a Python method which takes one string.
     *
     * @param name The name of the method to call.
     * @param a The string argument.
     * @return A PythonResult holding the return value of the call.
     */
    PythonResult call(const std::string& name, const std::string& a);

    /**
     * Calls a Python method which takes a string and an integer.
     *
     * @param name The name of the method to call.
     * @param a The string argument.
     * @param b The integer argument.
     * @return A PythonResult holding the return value of the call.
     */
    PythonResult call(const std::string& name, const std::string& a, long b);

private:

    /**
     * Initializes the embedded interpreter if it is not running yet.
     * This is a no-op while at least one module is alive. Besides starting
     * the interpreter, it puts the current working directory on Python's
     * module search path so modules next to the executable are importable.
     */
    static void ensureInterpreter();

    /**
     * Calls a method with a pre-built argument tuple.
     * This is the shared implementation behind the public call() overloads.
     *
     * @param name The name of the method to call.
     * @param args The argument tuple. An empty tuple calls the method with no
     *     arguments; a null tuple reports a failed argument build.
     * @return A PythonResult holding the return value of the call.
     */
    PythonResult invoke(const std::string& name, PyObjectPtr args);

    /// The name of the loaded module.
    std::string name;

    /**
     * The imported module handle. The PyObjectDeleter releases the reference
     * when this object is destroyed.
     */
    PyObjectPtr module;

    /**
     * The number of PythonModule objects currently alive. The embedded
     * interpreter runs while this count is above zero.
     */
    static int interpreterCount;
}; // class PythonModule
